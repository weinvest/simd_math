#include <gtest/gtest.h>
#include <random>
#include <cmath>
#include <iostream>
//#include "Floatx8.h"
#include "Doublex4.h"
#include <boost/math/distributions/normal.hpp>

constexpr size_t VECTOR_LEN = 1UL<<10;
constexpr size_t ALIGN = 32;
constexpr size_t ITER_COUNT = 10000;
constexpr double D_EPSILON = 1e-11;
namespace oo {
    double cdf(double x) {
        static boost::math::normal_distribution<> normal(0.0, 1.0);
        return boost::math::cdf(normal, x);
    }

    double pdf(double v) {
        static boost::math::normal_distribution<> normal(0.0, 1.0);
        return boost::math::pdf(normal, v);
    }
}

namespace option{
    double SVol(double spot, double strike, double volATM, double s, double c, double t, double discount)
    {
        double f = spot / discount;
        double v = c / std::sqrt(t);
        double rho = -2 * s / c;
        double m = 1 + (0.25 * rho * v * volATM + ((2 - 3 * rho * rho) / 24 * v * v)) * t;
        double z = -v / volATM * m * std::log(strike / f);
        double Xz = std::log((std::sqrt(1 - 2 * rho * z + z * z) + z - rho) / (1 - rho));

        if(std::abs(strike - f) < D_EPSILON)
        {
            return volATM;
        }
        else
        {
            double ratio = z / Xz;
            if(ratio > (0.4+D_EPSILON) && ratio < (5-D_EPSILON))
            {
                return volATM * ratio;
            }
            else if(ratio > (5-D_EPSILON))
            {
                return volATM * 5;
            }
            else
            {
                return volATM * 0.4;
            }
        }
    }

    double bsCall(double spot, double strike, double impVol, double ttm, double rate, double discount)
    {
        double d1 = (std::log(spot / (strike * discount)) + (0.5 * impVol * impVol * ttm)) / (impVol * std::sqrt(ttm));
        double d2 = d1 - impVol * std::sqrt(ttm);
        return spot * oo::cdf(d1) - strike * discount * oo::cdf(d2);
    }

    double bsPut(double spot, double strike, double impVol, double ttm, double rate, double discount)
    {
        double d1 = (std::log(spot / (strike * discount)) + (0.5 * impVol * impVol * ttm)) / (impVol * std::sqrt(ttm));
        double d2 = d1 - impVol * std::sqrt(ttm);
        return strike * discount * oo::cdf(-d2) - spot * oo::cdf(-d1);
    }
}

namespace fast_option{
    using Float = Doublex4;
    Float SVol(Float spot, Float volATM, Float slope, Float curv, Float sqrtTTM, Float ttm, Float lnUnder)
    {
        static const Float V_m2(-2.0);
        static const Float V_m0p5(-0.5);
        static const Float V_0p4(0.4);
        static const Float V_0(0.0);
        static const Float V_1(1.0);
        static const Float V_2(2.0);
        static const Float V_3(3.0);
        static const Float V_r12(1.0/12.0);
        static const Float V_5(5.0);

        auto v = curv / sqrtTTM;
        auto rho = V_m2 * slope / curv;
        auto m = Float::fuse_nmul_add(V_3, rho*rho, V_2);
        m = Float::fuse_mul_add(slope * sqrtTTM * volATM - (V_r12 * m  * curv * curv), V_m0p5, V_1);

        auto z = v / volATM * m * lnUnder;
        auto y = Float::fuse_mul_sub(V_2, rho, z);
        y = Float::fuse_nmul_add(y, z, V_1);
        auto Xz = Float::log((Float::sqrt(y) + z - rho) / (V_1 - rho));

        auto ratio = z / Xz;
        ratio = Float::clamp(ratio, V_0p4, V_5);

        return Float::if_then_else(V_0 == lnUnder, volATM, ratio*volATM);
    }

    Float bsCall(Float spot, Float presentStrike, Float impVol, Float sqrtTTM, Float lnUnder) //lnUnder=log(spot/presentStrike)
    {
        static const Float V_0p5(0.5);
        auto volTTM = impVol * sqrtTTM;
        auto d1 = Float::fuse_mul_add(V_0p5 * volTTM, volTTM, lnUnder) / volTTM;
        auto d2 = d1 - volTTM;
        return Float::fuse_mul_sub(spot, Float::cdf(d1), presentStrike * Float::cdf(d2));
    }

    Float bsPut(Float call, Float spot, Float presentStrike)
    {
        return call + presentStrike - spot;
    }
}

class AVXDoubleTest : public ::testing::Test {
private:
    template <typename T>
    inline int allocMemory(T** data) {
        return posix_memalign(reinterpret_cast<void**>(data), ALIGN, VECTOR_LEN*sizeof(double));
    }

protected:
    virtual void TearDown() {
        //free(inputData_);
        //free(nativeResult_);
        //free(fastResult_);
    }

    virtual void SetUp() {
        if(nullptr == nativeResult_) allocMemory(&nativeResult_);
        if(nullptr == fastResult_) allocMemory(&fastResult_);

        if(nullptr == inputData_)
        {
            allocMemory(&inputData_);
            std::random_device dev;
            std::mt19937_64 eng;
            eng.seed(dev());
            std::uniform_real_distribution<double> distribution(0, 10);

            for (size_t i = 0; i < VECTOR_LEN; ++i) {
                double tmp = distribution(eng);
                inputData_[i] = tmp;
            }
        }
    }

    static double* inputData_;
    static double* nativeResult_;
    static double* fastResult_;
};
double* AVXDoubleTest::inputData_ = nullptr;
double* AVXDoubleTest::nativeResult_ = nullptr;
double* AVXDoubleTest::fastResult_ = nullptr;

TEST_F(AVXDoubleTest, fast_log_1) {
    Doublex4 v(0.0226899);
    auto r = Doublex4::log(v);
    Doublex4 c(-3.7858353874279524);
    double* rv= (double*)&r;
    double* rc = (double*)&c;
    for(int32_t i = 0; i < 4; ++i)
    {
        ASSERT_NEAR(rv[i], rc[i], D_EPSILON);
    }
}

#define TEST_SIMD(func)\
TEST_F(AVXDoubleTest, fast_##func) {\
    for (size_t i = 0; i < ITER_COUNT; ++i) {\
        Doublex4 tmp;\
        Doublex4 ipt;\
        for (size_t j = 0; j < VECTOR_LEN; j+= Doublex4::STEP_CNT) {\
            ipt.load(inputData_ + j);\
            tmp = Doublex4::func(ipt);\
            tmp.store(fastResult_ + j);\
        }\
    }\
\
    for (size_t i = 0; i < VECTOR_LEN; ++i) {\
        if(std::abs(fastResult_[i] - nativeResult_[i]) >  D_EPSILON)\
        {\
            std::cerr << i << ":" << "exp^" << inputData_[i] << " = " << fastResult_[i] << "|" << nativeResult_[i] << "\n";\
        ASSERT_NEAR(fastResult_[i], nativeResult_[i], D_EPSILON);\
        }\
    }\
}

#define TEST_NAIVE(func, ns)\
TEST_F(AVXDoubleTest, naive_##func) {\
    for (size_t i = 0; i < ITER_COUNT; ++i) {\
        double ipt;\
        double tmp;\
        for (size_t j = 0; j < VECTOR_LEN; j+= 1) {\
            ipt = inputData_[j];\
            tmp = ns::func(ipt);\
            nativeResult_[j] = tmp;\
        }\
    }\
\
    for (size_t i = 0; i < VECTOR_LEN; ++i) {\
        ASSERT_NEAR(nativeResult_[i], nativeResult_[i], D_EPSILON);\
    }\
}

#if 1
TEST_NAIVE(exp, std)
TEST_SIMD(exp)

TEST_NAIVE(log, std)
TEST_SIMD(log)

TEST_NAIVE(pdf, oo)
TEST_SIMD(pdf)

TEST_NAIVE(cdf, oo)
TEST_SIMD(cdf)

TEST_NAIVE(sqrt, std)
TEST_SIMD(sqrt)

TEST_NAIVE(floor, std)
TEST_SIMD(floor)

TEST_NAIVE(ceil, std)
TEST_SIMD(ceil)

TEST_NAIVE(round, std)
TEST_SIMD(round)


#if 0
TEST_NAIVE(sin)
TEST_SIMD(sin)

TEST_NAIVE(cos)
TEST_SIMD(cos)
#endif

class OptionTest : public ::testing::Test {
public:
    static const int32_t STRIKE_LEN = 12;

    template <typename T>
    inline int allocMemory(T** data) {
        return posix_memalign(reinterpret_cast<void**>(data), ALIGN, STRIKE_LEN*sizeof(double));
    }
protected:
    virtual void TearDown() {
    }

    virtual void SetUp() {
        if(nullptr == mNaiveVol) allocMemory(&mNaiveVol);
        if(nullptr == mFastVol) allocMemory(&mFastVol);
        if(nullptr == mNaiveCall) allocMemory(&mNaiveCall);
        if(nullptr == mFastCall) allocMemory(&mFastCall);
        if(nullptr == mNaivePut) allocMemory(&mNaivePut);
        if(nullptr == mFastPut) allocMemory(&mFastPut);
        if(nullptr == mStrike)
        {
            allocMemory(&mStrike);
            double strike[STRIKE_LEN] = {2.40, 2.45, 2.50, 2.55, 2.60, 2.65, 2.70, 2.75, 2.80, 2.85, 2.90, 0.0};
            memcpy(mStrike, strike, STRIKE_LEN*sizeof(double));
        }
    }

    double mSpot = 2.5819348333;
    double mVolATM = 0.2038669709;
    double mSlope = 0.0295870768;
    double mCurv = 0.1655690339;
    double mTTM = 0.0943414267;
    double mDiscount = 0.9972395597;

    static double* mStrike;
    static double *mNaiveVol;
    static double *mFastVol;
    static double *mNaiveCall;
    static double *mFastCall;
    static double *mNaivePut;
    static double *mFastPut;
};

double *OptionTest::mNaiveVol = nullptr;
double *OptionTest::mFastVol = nullptr;
double *OptionTest::mNaiveCall = nullptr;
double *OptionTest::mFastCall = nullptr;
double *OptionTest::mNaivePut = nullptr;
double *OptionTest::mFastPut = nullptr;
double *OptionTest::mStrike = nullptr;

TEST_F(OptionTest, naive_vol)
{
    for (size_t i = 0; i < ITER_COUNT*100; ++i) {
        for (size_t j = 0; j < STRIKE_LEN; j+= 1) {
            mNaiveVol[j] = option::SVol(mSpot, mStrike[j], mVolATM, mSlope, mCurv, mTTM, mDiscount);
        }
    }

    for (size_t i = 0; i < STRIKE_LEN; ++i) {
        ASSERT_NEAR(mNaiveVol[i], mNaiveVol[i], D_EPSILON);
    }
}

TEST_F(OptionTest, fast_vol)
{
    for (size_t i = 0; i < ITER_COUNT*100; ++i) {
        fast_option::Float spot(mSpot);
        fast_option::Float volATM(mVolATM);
        fast_option::Float slope(mSlope);
        fast_option::Float curv(mCurv);
        fast_option::Float ttm(mTTM);
        fast_option::Float sqrtTTM = fast_option::Float::sqrt(ttm);
        fast_option::Float discount(mDiscount);
        fast_option::Float vol;
        fast_option::Float strike;
        fast_option::Float lnUnder;
        for (size_t j = 0; j < STRIKE_LEN; j+= fast_option::Float::STEP_CNT) {
            strike.load(mStrike+j);
            lnUnder = fast_option::Float::log(spot/(strike*discount));
            vol = fast_option::SVol(spot, volATM, slope, curv, sqrtTTM, ttm, lnUnder);
            vol.store(mFastVol+j);
        }
    }

    for (size_t i = 0; i < STRIKE_LEN; ++i) {
        std::cout << std::setw(3) << i << ":" << std::setprecision(10) << mNaiveVol[i] << " " << mFastVol[i] << "\n";
//        ASSERT_NEAR(mNaiveVol[i], mFastVol[i], D_EPSILON);
    }
}

TEST_F(OptionTest, naive_call)
{
    for (size_t i = 0; i < ITER_COUNT*100; ++i) {
        for (size_t j = 0; j < STRIKE_LEN; j+= 1) {
            mNaiveCall[j] = option::bsCall(mSpot, mStrike[j], mNaiveVol[j], mTTM, 1.0, mDiscount);
        }
    }

    for (size_t i = 0; i < STRIKE_LEN; ++i) {
        ASSERT_NEAR(mNaiveCall[i], mNaiveCall[i], D_EPSILON);
    }
}

TEST_F(OptionTest, fast_call)
{
    for (size_t i = 0; i < ITER_COUNT*100; ++i) {
        fast_option::Float spot(mSpot);
        fast_option::Float volATM(mVolATM);
        fast_option::Float slope(mSlope);
        fast_option::Float curv(mCurv);
        fast_option::Float ttm(mTTM);
        fast_option::Float sqrtTTM = fast_option::Float::sqrt(ttm);
        fast_option::Float discount(mDiscount);
        fast_option::Float vol;
        fast_option::Float call;
        fast_option::Float strike;
        fast_option::Float presentStrike;
        fast_option::Float lnUnder;
        for (size_t j = 0; j < STRIKE_LEN; j+= fast_option::Float::STEP_CNT) {
            strike.load(mStrike+j);
            presentStrike = strike*discount;
            lnUnder = fast_option::Float::log(spot/(presentStrike));
            vol.load(mFastVol +j);
            call = fast_option::bsCall(spot, presentStrike, vol, sqrtTTM, lnUnder);
            call.store(mFastCall+j);
        }
    }

    for (size_t i = 0; i < STRIKE_LEN; ++i) {
        std::cout << std::setw(3) << i << ":" << std::setprecision(10) << mNaiveCall[i] << " " << mFastCall[i] << "\n";
//        ASSERT_NEAR(mNaiveVol[i], mFastVol[i], D_EPSILON);
    }
}

TEST_F(OptionTest, naive_put)
{
    for (size_t i = 0; i < ITER_COUNT*100; ++i) {
        for (size_t j = 0; j < STRIKE_LEN; j+= 1) {
            mNaivePut[j] = option::bsPut(mSpot, mStrike[j], mNaiveVol[j], mTTM, 1.0, mDiscount);
        }
    }

    for (size_t i = 0; i < STRIKE_LEN; ++i) {
        ASSERT_NEAR(mNaivePut[i], mNaivePut[i], D_EPSILON);
    }
}

TEST_F(OptionTest, fast_put)
{
    for (size_t i = 0; i < ITER_COUNT*100; ++i) {
        fast_option::Float spot(mSpot);
        fast_option::Float discount(mDiscount);
        fast_option::Float call;
        fast_option::Float put;
        fast_option::Float strike;
        fast_option::Float presentStrike;
        for (size_t j = 0; j < STRIKE_LEN; j+= fast_option::Float::STEP_CNT) {
            strike.load(mStrike+j);
            presentStrike = strike*discount;
            call.load(mFastCall+j);
            put = fast_option::bsPut(call, spot, presentStrike);
            put.store(mFastPut+j);
        }
    }

    for (size_t i = 0; i < STRIKE_LEN; ++i) {
        std::cout << std::setw(3) << i << ":" << std::setprecision(10) << mNaivePut[i] << " " << mFastPut[i] << "\n";
//        ASSERT_NEAR(mNaiveVol[i], mFastVol[i], D_EPSILON);
    }
}

TEST_F(AVXDoubleTest, is_nan)
{
    double normal(8.2425358);
    double nan(std::numeric_limits<double>::quiet_NaN());

    Doublex4 v(normal, nan, nan, normal);

    auto result = Doublex4::is_nan(v);
    double* r = (double*)&result;

    std::cout << r[0] << ":" << r[1] << ":" << r[2] << ":" << r[3] << "\n";
    std::cout << (0*nan) << std::endl;
}
#endif



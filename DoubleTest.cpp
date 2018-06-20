#include <gtest/gtest.h>
#include <random>
#include <cmath>
#include <iostream>
//#include "Floatx8.h"
#include "Doublex4.h"
#include "FastMath.h"
#include <boost/math/distributions/normal.hpp>

constexpr size_t VECTOR_LEN = 1UL<<16;
constexpr size_t ALIGN = 32;
constexpr size_t ITER_COUNT = 10000;
constexpr double D_EPSILON = 1e-11;
using namespace fast_math;
namespace oo {
    double cdf(double x) {
#if 1
        static boost::math::normal_distribution<> normal(0.0, 1.0);
        return boost::math::cdf(normal, v);
#else
        static double a1 =  0.254829592;
        static double a2 = -0.284496736;
        static double a3 =  1.421413741;
        static double a4 = -1.453152027;
        static double a5 =  1.061405429;
        static double p  =  0.3275911;
        static double s = sqrt(2.0);

        // Save the sign of x
        int sign = 1;
        if (x < 0)
        {
            sign = -1;
            x /= -s;
        }
        else
        {
            x /= s;
        }

        // A&S formula 7.1.26
        double t = 1.0/(1.0 + p*x);
        double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

        return 0.5*(1.0 + sign*y);
#endif
    }

    double pdf(double v) {
        static boost::math::normal_distribution<> normal(0.0, 1.0);
        return boost::math::pdf(normal, v);
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
            std::uniform_real_distribution<double> distribution(0, 1);

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

#define TEST_SIMD(func)\
TEST_F(AVXDoubleTest, fast_##func) {\
    for (size_t i = 0; i < ITER_COUNT; ++i) {\
        Doublex4 tmp;\
        Doublex4 ipt;\
        for (size_t j = 0; j < VECTOR_LEN; j+= Doublex4::STEP_CNT) {\
            ipt.load(inputData_ + j);\
            tmp = func(ipt);\
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

TEST_NAIVE(exp, std)
TEST_SIMD(exp)

TEST_NAIVE(log, std)
TEST_SIMD(log)

TEST_NAIVE(pdf, oo)
TEST_SIMD(pdf)

TEST_NAIVE(cdf, oo)
TEST_SIMD(cdf)
#if 0
TEST_NAIVE(sin)
TEST_SIMD(sin)

TEST_NAIVE(cos)
TEST_SIMD(cos)
#endif

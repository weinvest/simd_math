#include <gtest/gtest.h>
#include <random>
#include <cmath>
#include <iostream>
#include "Floatx8.h"
#include "FastMath.h"
#include <boost/math/distributions/normal.hpp>

constexpr size_t VECTOR_LEN = 1UL<<16;
constexpr size_t ALIGN = 32;
constexpr size_t ITER_COUNT = 10000;
constexpr float F_EPSILON = 1e-6;
using namespace fast_math;
namespace oo {
    float cdf(float v) {
        static boost::math::normal_distribution<> normal(0.0, 1.0);
        return boost::math::cdf(normal, v);
    }

    float pdf(float v) {
        static boost::math::normal_distribution<> normal(0.0, 1.0);
        return boost::math::pdf(normal, v);
    }
}

class AVXFloatTest : public ::testing::Test {
private:
    template <typename T>
    inline int allocMemory(T** data) {
        return posix_memalign(reinterpret_cast<void**>(data), ALIGN, VECTOR_LEN*sizeof(float));
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
            std::uniform_real_distribution<float> distribution(0, 1);

            for (size_t i = 0; i < VECTOR_LEN; ++i) {
                float tmp = distribution(eng);
                inputData_[i] = tmp;
            }
        }
    }

    static float* inputData_;
    static float* nativeResult_;
    static float* fastResult_;
};
float* AVXFloatTest::inputData_ = nullptr;
float* AVXFloatTest::nativeResult_ = nullptr;
float* AVXFloatTest::fastResult_ = nullptr;

#define TEST_SIMD(func)\
TEST_F(AVXFloatTest, fast_##func) {\
    for (size_t i = 0; i < ITER_COUNT; ++i) {\
        Floatx8 tmp;\
        Floatx8 ipt;\
        for (size_t j = 0; j < VECTOR_LEN; j+= Floatx8::STEP_CNT) {\
            ipt.load(inputData_ + j);\
            tmp = func(ipt);\
            tmp.store(fastResult_ + j);\
        }\
    }\
\
    for (size_t i = 0; i < VECTOR_LEN; ++i) {\
        if(std::abs(fastResult_[i] - nativeResult_[i]) >  F_EPSILON)\
        {\
            std::cerr << i << ":" << "exp^" << inputData_[i] << " = " << fastResult_[i] << "|" << nativeResult_[i] << "\n";\
        ASSERT_NEAR(fastResult_[i], nativeResult_[i], F_EPSILON);\
        }\
    }\
}

#define TEST_NAIVE(func, ns)\
TEST_F(AVXFloatTest, naive_##func) {\
    for (size_t i = 0; i < ITER_COUNT; ++i) {\
        float ipt;\
        float tmp;\
        for (size_t j = 0; j < VECTOR_LEN; j+= 1) {\
            ipt = inputData_[j];\
            tmp = ns::func(ipt);\
            nativeResult_[j] = tmp;\
        }\
    }\
\
    for (size_t i = 0; i < VECTOR_LEN; ++i) {\
        ASSERT_NEAR(nativeResult_[i], nativeResult_[i], F_EPSILON);\
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



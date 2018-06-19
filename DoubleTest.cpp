#include <gtest/gtest.h>
#include <random>
#include <cmath>
#include <iostream>
//#include "Floatx8.h"
#include "Doublex4.h"
#include "FastMath.h"

constexpr size_t VECTOR_LEN = 1UL<<16;
constexpr size_t ALIGN = 32;
constexpr size_t ITER_COUNT = 10000;
constexpr float F_EPSILON = 1e-6;
constexpr float D_EPSILON = 1e-12;
using namespace fast_math;
class AVXDoubleTest : public ::testing::Test {
private:
    template <typename T>
    inline int allocMemory(T** data) {
        return posix_memalign(reinterpret_cast<void**>(data), ALIGN, VECTOR_LEN*sizeof(double));
    }

protected:
    virtual void TearDown() {
        free(inputData_);
        free(nativeResult_);
        free(fastResult_);
    }

    virtual void SetUp() {
        allocMemory(&inputData_);
        allocMemory(&nativeResult_);
        allocMemory(&fastResult_);

        std::random_device dev;
        std::mt19937_64 eng;
        eng.seed(dev());
        std::uniform_real_distribution<double> distribution(0, 1);

        for (size_t i = 0; i < VECTOR_LEN; ++i) {
            double tmp = distribution(eng);
            inputData_[i] = tmp;
        }
    }

    double* inputData_;
    double* nativeResult_;
    double* fastResult_;
};

#define TEST_SIMD(func)\
TEST_F(AVXDoubleTest, fast_##func) {\
    for (size_t i = 0; i < ITER_COUNT; ++i) {\
        Doublex4 tmp;\
        Doublex4 ipt;\
        for (size_t j = 0; j < VECTOR_LEN; j+= 8) {\
            ipt.load(inputData_ + j);\
            tmp = func(ipt);\
            tmp.store(fastResult_);\
        }\
    }\
\
    for (size_t i = 0; i < VECTOR_LEN; ++i) {\
        ASSERT_NEAR(fastResult_[i], nativeResult_[i], D_EPSILON);\
    }\
}

#define TEST_NAIVE(func)\
TEST_F(AVXDoubleTest, naive_##func) {\
    for (size_t i = 0; i < ITER_COUNT; ++i) {\
        float ipt;\
        float tmp;\
        for (size_t j = 0; j < VECTOR_LEN; j+= 1) {\
            ipt = inputData_[j];\
            tmp = std::func(ipt);\
            nativeResult_[j] = tmp;\
        }\
    }\
\
    for (size_t i = 0; i < VECTOR_LEN; ++i) {\
        ASSERT_NEAR(nativeResult_[i], nativeResult_[i], D_EPSILON);\
    }\
}

TEST_NAIVE(exp)
TEST_SIMD(exp)

TEST_NAIVE(log)
TEST_SIMD(log)

TEST_NAIVE(sin)
TEST_SIMD(sin)

TEST_NAIVE(cos)
TEST_SIMD(cos)
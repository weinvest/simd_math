#include <gtest/gtest.h>
#include <random>
#include <cmath>
#include <iostream>
#include "Int32x4.h"
#include <boost/math/distributions/uniform.hpp>
using AVXInt = Int32x4;
class AVXIntTest : public ::testing::Test {
public:
    template<typename T>
    inline int allocMemory(T **data, int32_t cnt) {
        return posix_memalign(reinterpret_cast<void **>(data), 32, cnt * sizeof(double));
    }

protected:
    static constexpr int32_t INPUT_DATA_LEN = 100;

    virtual void TearDown() {
        free(input1_);
        free(input2_);
        free(naive_);
        free(avxo_);
    }

    virtual void SetUp() {
        allocMemory(&input1_, INPUT_DATA_LEN);
        allocMemory(&input2_, INPUT_DATA_LEN);
        allocMemory(&naive_, INPUT_DATA_LEN);
        allocMemory(&avxo_, INPUT_DATA_LEN);

        std::random_device dev;
        std::mt19937_64 eng;
        eng.seed(dev());

        std::uniform_int_distribution<int32_t> intd(-100, 100);
        for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)
        {
            input1_[i] = intd(eng);
            input2_[i] = intd(eng);
        }
    }

    int32_t *input1_;
    int32_t *input2_;
    int32_t *naive_;
    int32_t *avxo_;
};

#define TEST_INTx4_BIN_OP(fun, naive_op)\
TEST_F(AVXIntTest, test_##fun)\
{\
    for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)\
    {\
        naive_[i] = input1_[i] naive_op input2_[i];\
    }\
    AVXInt a, b, c;\
    for(int32_t i = 0; i < INPUT_DATA_LEN; i += AVXInt::STEP_CNT)\
    {\
        a.load(input1_ + i);\
        b.load(input2_ + i);\
        c = AVXInt::fun(a, b);\
        c.store(avxo_ + i);\
    }\
    for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)\
    {\
        EXPECT_EQ(naive_[i], avxo_[i]);\
    }\
}

#define TEST_INTx4_SIG_OP(fun, naive_op)\
TEST_F(AVXIntTest, test_##fun)\
{\
    for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)\
    {\
        naive_[i] = naive_op input1_[i];\
    }\
    AVXInt a,c;\
    for(int32_t i = 0; i < INPUT_DATA_LEN; i += AVXInt::STEP_CNT)\
    {\
        a.load(input1_ + i);\
        c = AVXInt::fun(a);\
        c.store(avxo_ + i);\
    }\
    for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)\
    {\
        EXPECT_EQ(naive_[i], avxo_[i]);\
    }\
}

#define TEST_INTx4_BIN_FUN(fun, naive_op)\
TEST_F(AVXIntTest, test_##fun)\
{\
    for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)\
    {\
        naive_[i] = naive_op(input1_[i], input2_[i]);\
    }\
    AVXInt a, b, c;\
    for(int32_t i = 0; i < INPUT_DATA_LEN; i += AVXInt::STEP_CNT)\
    {\
        a.load(input1_ + i);\
        b.load(input2_ + i);\
        c = AVXInt::fun(a, b);\
        c.store(avxo_ + i);\
    }\
    for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)\
    {\
        EXPECT_EQ(naive_[i], avxo_[i]);\
    }\
}

#define TEST_INTx4_SIG_FUN(fun, naive_op)\
TEST_F(AVXIntTest, test_##fun)\
{\
    for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)\
    {\
        naive_[i] = naive_op(input1_[i]);\
    }\
    AVXInt a,c;\
    for(int32_t i = 0; i < INPUT_DATA_LEN; i += AVXInt::STEP_CNT)\
    {\
        a.load(input1_ + i);\
        c = AVXInt::fun(a);\
        c.store(avxo_ + i);\
    }\
    for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)\
    {\
       if(naive_[i] != avxo_[i])\
       {std::cout << input1_[i] << ":" << naive_[i] << ":" << avxo_[i] << std::endl; }\
        EXPECT_EQ(naive_[i], avxo_[i]);\
    }\
}

TEST_INTx4_BIN_OP(add, +)
TEST_INTx4_BIN_OP(sub, -)
TEST_INTx4_BIN_OP(mul, *)
TEST_INTx4_BIN_OP(_and, &)
TEST_INTx4_BIN_OP(_or, |)
TEST_INTx4_BIN_OP(_xor, ^)
TEST_INTx4_SIG_OP(_not, ~)

TEST_INTx4_BIN_OP(less_than, <)
TEST_INTx4_BIN_OP(less_equal, <=)
TEST_INTx4_BIN_OP(great_than, >)
TEST_INTx4_BIN_OP(great_equal, >=)
TEST_INTx4_BIN_OP(equal, ==)
TEST_INTx4_BIN_OP(not_equal, !=)

TEST_INTx4_SIG_FUN(abs, std::abs)
TEST_INTx4_BIN_FUN(min, std::min)
TEST_INTx4_BIN_FUN(max, std::max)

TEST_F(AVXIntTest, test_abs1)
{
    AVXInt a(-52, -73, -18, -8);
    AVXInt b = AVXInt::abs(a);
    int32_t* pB = reinterpret_cast<int32_t*>(&b);
    std::cout << pB[0] << "," << pB[1] << "," << pB[2] << "," << pB[3] << std::endl;
    AVXInt m = AVXInt::less_than(a, AVXInt::CONST_0);
    int32_t* pM = reinterpret_cast<int32_t*>(&m);
    std::cout << pM[0] << "," << pM[1] << "," << pM[2] << "," << pM[3] << std::endl;

    AVXInt m_a = -a.v;
    int32_t* pM_a = reinterpret_cast<int32_t*>(&pM_a);
    std::cout << pM_a[0] << "," << pM_a[1] << "," << pM_a[2] << "," << pM_a[3] << std::endl;


}

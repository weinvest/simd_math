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

namespace IntCompWraper
{
    AVXInt less_than(AVXInt a, AVXInt b) { return AVXInt::pred_2_num(AVXInt::less_than(a,b)); }
    AVXInt less_equal(AVXInt a, AVXInt b) { return AVXInt::pred_2_num(AVXInt::less_equal(a,b)); }
    AVXInt great_than(AVXInt a, AVXInt b) { return AVXInt::pred_2_num(AVXInt::great_than(a,b)); }
    AVXInt great_equal(AVXInt a, AVXInt b) { return AVXInt::pred_2_num(AVXInt::great_equal(a,b)); }
    AVXInt equal(AVXInt a, AVXInt b) { return AVXInt::pred_2_num(AVXInt::equal(a,b)); }
    AVXInt not_equal(AVXInt a, AVXInt b) { return AVXInt::pred_2_num(AVXInt::not_equal(a,b)); }
}

#define TEST_INTx4_BIN_OP(ns, fun, naive_op)\
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
        c = ns::fun(a, b);\
        c.store(avxo_ + i);\
    }\
    for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)\
    {\
        EXPECT_EQ(naive_[i], avxo_[i]);\
    }\
}

#define TEST_INTx4_SIG_OP(ns, fun, naive_op)\
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
        c = ns::fun(a);\
        c.store(avxo_ + i);\
    }\
    for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)\
    {\
        EXPECT_EQ(naive_[i], avxo_[i]);\
    }\
}

#define TEST_INTx4_BIN_FUN(ns, fun, naive_op)\
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
        c = ns::fun(a, b);\
        c.store(avxo_ + i);\
    }\
    for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)\
    {\
        EXPECT_EQ(naive_[i], avxo_[i]);\
    }\
}

#define TEST_INTx4_SIG_FUN(ns, fun, naive_op)\
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
        c = ns::fun(a);\
        c.store(avxo_ + i);\
    }\
    for(int32_t i = 0; i < INPUT_DATA_LEN; ++i)\
    {\
       if(naive_[i] != avxo_[i])\
       {std::cout << input1_[i] << ":" << naive_[i] << ":" << avxo_[i] << std::endl; }\
        EXPECT_EQ(naive_[i], avxo_[i]);\
    }\
}

TEST_INTx4_BIN_OP(AVXInt, add, +)
TEST_INTx4_BIN_OP(AVXInt, sub, -)
TEST_INTx4_BIN_OP(AVXInt, mul, *)
TEST_INTx4_BIN_OP(AVXInt, _and, &)
TEST_INTx4_BIN_OP(AVXInt, _or, |)
TEST_INTx4_BIN_OP(AVXInt, _xor, ^)
TEST_INTx4_SIG_OP(AVXInt, _not, ~)

TEST_INTx4_BIN_OP(IntCompWraper,less_than, <)
TEST_INTx4_BIN_OP(IntCompWraper,less_equal, <=)
TEST_INTx4_BIN_OP(IntCompWraper,great_than, >)
TEST_INTx4_BIN_OP(IntCompWraper,great_equal, >=)
TEST_INTx4_BIN_OP(IntCompWraper,equal, ==)
TEST_INTx4_BIN_OP(IntCompWraper,not_equal, !=)

TEST_INTx4_SIG_FUN(AVXInt, abs, std::abs)
TEST_INTx4_BIN_FUN(AVXInt, min, std::min)
TEST_INTx4_BIN_FUN(AVXInt, max, std::max)

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

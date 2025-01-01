
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class PlaceHolder {
public:
    virtual void Encrypt() = 0;
    virtual void Decrypt() = 0;
};

class StrategyMock : public PlaceHolder {
public:

    MOCK_METHOD(void, Encrypt, (), (override));
    MOCK_METHOD(void, Decrypt, (), (override));

};

class StrategyTestsSuite : public testing::Test {
public:


};

TEST_F(StrategyTestsSuite, Strategy_Encryption) {

    EXPECT_EQ(1,1);
}

TEST_F(StrategyTestsSuite, Strategy_Decryption) {

    EXPECT_EQ(1,1);
}

auto main(int argc, char *argv[]) -> int
{
  ::testing::InitGoogleTest();
 
  return RUN_ALL_TESTS();
}

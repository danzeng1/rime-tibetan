#include <gtest/gtest.h>
#include "tibetan.h"

class TibetanTest : public ::testing::Test {
protected:
    void SetUp() override {
        RimeSetupLogging("rime.tibetan.test");
        rime_ = RimeGetApi();
        
        Ticket ticket;
        ticket.schema = new Schema();
        translator_ = new TibetanTranslator(ticket);
    }
    
    void TearDown() override {
        delete translator_;
    }
    
    RimeApi* rime_;
    TibetanTranslator* translator_;
};

TEST_F(TibetanTest, BasicConsonants) {
    auto result = translator_->Query("ka");
    ASSERT_TRUE(result);
    EXPECT_EQ(result->Peek()->text, "ཀ ");
    
    result = translator_->Query("kha");
    ASSERT_TRUE(result);
    EXPECT_EQ(result->Peek()->text, "ཁ ");
}

TEST_F(TibetanTest, SubjoinedLetters) {
    auto result = translator_->Query("kya");
    ASSERT_TRUE(result);
    EXPECT_EQ(result->Peek()->text, "ཀྱ ");
    
    result = translator_->Query("tra");
    ASSERT_TRUE(result);
    EXPECT_EQ(result->Peek()->text, "ཏྲ ");
}

TEST_F(TibetanTest, InvalidInput) {
    auto result = translator_->Query("123");
    ASSERT_FALSE(result);
    
    result = translator_->Query("ka@");
    ASSERT_FALSE(result);
}

TEST_F(TibetanTest, MultipleSyllables) {
    auto result = translator_->Query("bkra shis");
    ASSERT_TRUE(result);
    EXPECT_EQ(result->Peek()->text, "བཀྲ་ ཤིས ");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
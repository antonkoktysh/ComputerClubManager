#include "gtest/gtest.h"
#include "entities/table.h"

using namespace computer_club::entities;

class TableTest : public ::testing::Test {
protected:
    void SetUp() override {
        table_ = std::make_unique<Table>(1);
    }

    std::unique_ptr<Table> table_;
    const int kHourlyRate = 10;
};

TEST_F(TableTest, InitialState) {
    EXPECT_EQ(table_->GetNumber(), 1);
    EXPECT_FALSE(table_->IsOccupied());
    EXPECT_EQ(table_->GetClientName(), std::nullopt);
    EXPECT_EQ(table_->GetOccupiedSince(), std::nullopt);
    EXPECT_EQ(table_->GetTotalMinutes().count(), 0);
    EXPECT_EQ(table_->GetRevenue(), 0);
}

TEST_F(TableTest, OccupyAndRelease) {
    Time start_time(10, 0);
    std::string client_name = "client1";

    table_->Occupy(client_name, start_time);

    EXPECT_TRUE(table_->IsOccupied());
    ASSERT_TRUE(table_->GetClientName().has_value());
    EXPECT_EQ(table_->GetClientName().value(), client_name);
    ASSERT_TRUE(table_->GetOccupiedSince().has_value());
    EXPECT_EQ(table_->GetOccupiedSince().value(), start_time);

    Time end_time(12, 30);
    table_->Release(end_time, kHourlyRate);

    EXPECT_EQ(table_->GetRevenue(), 30);
    EXPECT_EQ(table_->GetTotalMinutes().count(), 150);
    EXPECT_FALSE(table_->IsOccupied());
    EXPECT_EQ(table_->GetClientName(), std::nullopt);
}

TEST_F(TableTest, RevenueCalculationRoundsUp) {
    table_->Occupy("client1", Time(9, 0));
    table_->Release(Time(9, 59), kHourlyRate);
    EXPECT_EQ(table_->GetRevenue(), 10);
    EXPECT_EQ(table_->GetTotalMinutes().count(), 59);

    table_->Occupy("client2", Time(10, 0));
    table_->Release(Time(11, 0), kHourlyRate);
    EXPECT_EQ(table_->GetRevenue(), 20);
    EXPECT_EQ(table_->GetTotalMinutes().count(), 59 + 60);

    table_->Occupy("client3", Time(12, 0));
    table_->Release(Time(13, 1), kHourlyRate);
    EXPECT_EQ(table_->GetRevenue(), 40);
    EXPECT_EQ(table_->GetTotalMinutes().count(), 59 + 60 + 61);

    table_->Occupy("client4", Time(14, 0));
    table_->Release(Time(14, 0), kHourlyRate);
    EXPECT_EQ(table_->GetRevenue(), 40);
    EXPECT_EQ(table_->GetTotalMinutes().count(), 59 + 60 + 61 + 0);
}
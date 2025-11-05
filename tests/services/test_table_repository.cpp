#include "gtest/gtest.h"
#include "services/table_repository.h"
#include "utils/exceptions.h"
#include "entities/time.h"

using namespace computer_club::services;
using namespace computer_club::entities;

class TableRepositoryTest : public ::testing::Test {
protected:
    const int kTableCount = 3;
    const int kHourlyRate = 10;
    TableRepository repo_{kTableCount};
};

TEST_F(TableRepositoryTest, InitialState) {
    EXPECT_EQ(repo_.GetTableCount(), kTableCount);
    EXPECT_EQ(repo_.GetAvailableTableCount(), kTableCount);

    auto tables = repo_.GetAllTables();
    ASSERT_EQ(tables.size(), kTableCount);
    EXPECT_EQ(tables[0]->GetNumber(), 1);
    EXPECT_EQ(tables[1]->GetNumber(), 2);
    EXPECT_EQ(tables[2]->GetNumber(), 3);
}

TEST_F(TableRepositoryTest, InvalidConstruction) {
    EXPECT_THROW(TableRepository(0), std::invalid_argument);
    EXPECT_THROW(TableRepository(-1), std::invalid_argument);
}

TEST_F(TableRepositoryTest, GetTable) {
    auto table2 = repo_.GetTable(2);
    ASSERT_NE(table2, nullptr);
    EXPECT_EQ(table2->GetNumber(), 2);

    EXPECT_THROW(repo_.GetTable(0), std::out_of_range);
    EXPECT_THROW(repo_.GetTable(kTableCount + 1), std::out_of_range);
}

TEST_F(TableRepositoryTest, OccupyAndReleaseTable) {
    Time time1(10, 0);
    Time time2(11, 0);
    int table_num = 1;

    EXPECT_FALSE(repo_.IsTableOccupied(table_num));
    EXPECT_EQ(repo_.GetAvailableTableCount(), kTableCount);

    repo_.OccupyTable(table_num, "client1", time1);

    EXPECT_TRUE(repo_.IsTableOccupied(table_num));
    EXPECT_EQ(repo_.GetAvailableTableCount(), kTableCount - 1);
    auto table = repo_.GetTable(table_num);
    EXPECT_EQ(table->GetClientName().value(), "client1");

    repo_.ReleaseTable(table_num, time2, kHourlyRate);

    EXPECT_FALSE(repo_.IsTableOccupied(table_num));
    EXPECT_EQ(repo_.GetAvailableTableCount(), kTableCount);
    EXPECT_EQ(table->GetRevenue(), kHourlyRate);
    EXPECT_EQ(table->GetTotalMinutes().count(), 60);
}

TEST_F(TableRepositoryTest, AvailableTableCount) {
    EXPECT_EQ(repo_.GetAvailableTableCount(), 3);

    repo_.OccupyTable(1, "client1", Time(10, 0));
    EXPECT_EQ(repo_.GetAvailableTableCount(), 2);

    repo_.OccupyTable(2, "client2", Time(10, 0));
    EXPECT_EQ(repo_.GetAvailableTableCount(), 1);

    repo_.OccupyTable(3, "client3", Time(10, 0));
    EXPECT_EQ(repo_.GetAvailableTableCount(), 0);

    repo_.ReleaseTable(2, Time(11, 0), kHourlyRate);
    EXPECT_EQ(repo_.GetAvailableTableCount(), 1);

    repo_.ReleaseTable(1, Time(11, 0), kHourlyRate);
    EXPECT_EQ(repo_.GetAvailableTableCount(), 2);
}
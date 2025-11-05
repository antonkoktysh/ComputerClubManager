#include "gtest/gtest.h"
#include "entities/time.h"

using namespace computer_club::entities;

TEST(TimeTest, ConstructionAndAccessors) {
    Time t(9, 30);
    EXPECT_EQ(t.Hours(), 9);
    EXPECT_EQ(t.Minutes(), 30);
    EXPECT_EQ(t.ToString(), "09:30");

    Time t_midnight(0, 0);
    EXPECT_EQ(t_midnight.ToString(), "00:00");

    Time t_noon(12, 0);
    EXPECT_EQ(t_noon.ToString(), "12:00");

    Time t_single_digit(1, 5);
    EXPECT_EQ(t_single_digit.ToString(), "01:05");
}

TEST(TimeTest, Comparison) {
    Time t1(10, 0);
    Time t2(12, 0);
    Time t3(10, 0);

    EXPECT_LT(t1, t2);
    EXPECT_LE(t1, t2);
    EXPECT_LE(t1, t3);
    EXPECT_GT(t2, t1);
    EXPECT_GE(t2, t1);
    EXPECT_GE(t1, t3);
    EXPECT_EQ(t1, t3);
    EXPECT_NE(t1, t2);
}

TEST(TimeTest, Arithmetic) {
    Time t1(9, 0);
    Time t2(10, 30);

    Time t3 = t1 + std::chrono::minutes(90);
    EXPECT_EQ(t3, t2);

    Time t4(23, 50);
    Time t5 = t4 + std::chrono::minutes(20);
    EXPECT_EQ(t5.Hours(), 0);
    EXPECT_EQ(t5.Minutes(), 10);
    EXPECT_EQ(t5.ToString(), "00:10");
}

TEST(TimeTest, WorkingTimeIsOpenAt) {
    WorkingTime wt{Time(9, 0), Time(18, 0)};

    EXPECT_TRUE(wt.IsOpenAt(Time(9, 0)));
    EXPECT_TRUE(wt.IsOpenAt(Time(12, 0)));
    EXPECT_TRUE(wt.IsOpenAt(Time(18, 0)));

    EXPECT_FALSE(wt.IsOpenAt(Time(8, 59)));
    EXPECT_FALSE(wt.IsOpenAt(Time(18, 1)));
}
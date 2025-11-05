#include "gtest/gtest.h"
#include "utils/file_parser.h"
#include "utils/exceptions.h"
#include <sstream>

using namespace computer_club::utils;
using namespace computer_club::entities;

TEST(FileParserTest, ParseValidData) {
    std::string valid_input = R"(
3
09:00 18:00
10
10:00 1 client1
11:00 2 client1 2)";
    std::istringstream stream(valid_input);

    auto [config, events] = FileParser::Parse(stream);

    EXPECT_EQ(config.GetTableCount(), 3);
    EXPECT_EQ(config.GetOpenTime(), Time(9, 0));
    EXPECT_EQ(config.GetCloseTime(), Time(18, 0));
    EXPECT_EQ(config.GetHourlyRate(), 10);

    ASSERT_EQ(events.size(), 2);
    EXPECT_EQ(events[0].time, Time(10, 0));
    EXPECT_TRUE(std::holds_alternative<EventInClientArrives>(events[0].payload));

    EXPECT_EQ(events[1].time, Time(11, 0));
    EXPECT_TRUE(std::holds_alternative<EventInClientSits>(events[1].payload));
    auto payload = std::get<EventInClientSits>(events[1].payload);
    EXPECT_EQ(payload.client, "client1");
    EXPECT_EQ(payload.table_id, 2);
}

TEST(FileParserTest, InvalidTimeFormat) {
    std::string invalid_input = R"(
3
09:00 1800
10)";
    std::istringstream stream(invalid_input);
    EXPECT_THROW(FileParser::Parse(stream), ParsingException);
}

TEST(FileParserTest, InvalidInteger) {
    std::string invalid_input = R"(
-5
09:00 18:00
10)";
    std::istringstream stream(invalid_input);
    EXPECT_THROW(FileParser::Parse(stream), ParsingException);
}

TEST(FileParserTest, InvalidEventId) {
    std::string invalid_input = R"(
3
09:00 18:00
10
10:00 99 client1)";
    std::istringstream stream(invalid_input);
    EXPECT_THROW(FileParser::Parse(stream), InvalidFormatException);
}

TEST(FileParserTest, TrailingData) {
    std::string invalid_input = R"(
3
09:00 18:00
10
10:00 1 client1 some_junk)";
    std::istringstream stream(invalid_input);
    EXPECT_THROW(FileParser::Parse(stream), InvalidFormatException);
}

TEST(FileParserTest, NonMonotonousTime) {
    std::string invalid_input = R"(
3
09:00 18:00
10
11:00 1 client1
10:59 1 client2)";
    std::istringstream stream(invalid_input);
    EXPECT_THROW(FileParser::Parse(stream), InvalidEventException);
}
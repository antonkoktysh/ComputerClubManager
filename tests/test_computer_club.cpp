#include "gtest/gtest.h"
#include "core/computer_club_manager_impl.h"
#include "utils/file_parser.h"
#include "utils/result_printer.h"
#include <sstream>
#include <string>

const std::string kTestInput = R"(3
09:00 19:00
10
08:48 1 client1
09:41 1 client1
09:48 1 client2
09:52 3 client1
09:54 2 client1 1
10:25 2 client2 2
10:58 1 client3
10:59 2 client3 3
11:30 1 client4
11:35 2 client4 2
11:45 3 client4
12:33 4 client1
12:43 4 client2
15:52 4 client4
)";

const std::string kExpectedOutput = R"(09:00
08:48 1 client1
08:48 13 NotOpenYet
09:41 1 client1
09:48 1 client2
09:52 3 client1
09:52 13 ICanWaitNoLonger
09:54 2 client1 1
10:25 2 client2 2
10:58 1 client3
10:59 2 client3 3
11:30 1 client4
11:35 2 client4 2
11:35 13 PlaceIsBusy
11:45 3 client4
12:33 4 client1
12:33 12 client4 1
12:43 4 client2
15:52 4 client4
19:00 11 client3
19:00
1 70 05:58
2 30 02:18
3 90 08:01
)";

std::string CleanLineEndings(const std::string& str) {
    std::string result;
    result.reserve(str.length());
    for (char c : str) {
        if (c != '\r') {
            result += c;
        }
    }
    return result;
}

TEST(ComputerClubLogic, TestInput1) {
    std::istringstream input_stream(kTestInput);

    auto [config, input_events] = computer_club::utils::FileParser::Parse(input_stream);

    auto manager = std::make_unique<computer_club::core::ComputerClubManagerImpl>(config);
    manager->ProcessEvents(input_events);
    manager->Close();

    std::ostringstream output_stream;
    computer_club::utils::PrintResults(output_stream, *manager);

    std::string actual_output = CleanLineEndings(output_stream.str());
    std::string expected_output = CleanLineEndings(kExpectedOutput);

    ASSERT_EQ(actual_output, expected_output);
}
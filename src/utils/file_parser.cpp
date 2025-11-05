#include "utils/file_parser.h"
#include <sstream>
#include <utility>
#include "utils/exceptions.h"
#include <charconv>
#include <cstdint>
#include <string_view>
#include <fstream>

namespace computer_club::utils {

static std::string MakeError(std::string_view message, size_t line_num, const std::string& line) {
    return "Error at line " + std::to_string(line_num) + ": " + std::string(message) + "\n> " +
           line;
}

void FileParser::CheckForTrailingData(std::istringstream& event_stream, size_t line_num,
                                      const std::string& line) {
    std::string junk;
    if (event_stream >> junk) {
        throw InvalidFormatException(MakeError("Trailing data detected", line_num, line));
    }
}

bool FileParser::GetNextLine(std::istream& stream, std::string& line, size_t& line_number) {
    while (std::getline(stream, line)) {
        line_number++;
        if (!line.empty()) {
            return true;
        }
    }
    return false;
}

ClubData FileParser::Parse(std::istream& stream) {
    size_t line_number = 0;
    entities::ClubConfiguration config = ParseConfiguration(stream, line_number);
    std::vector<entities::IncomingEvent> events = ParseEvents(stream, line_number);
    return {config, events};
}

entities::ClubConfiguration FileParser::ParseConfiguration(std::istream& stream,
                                                           size_t& line_number) {
    std::string line;

    if (!GetNextLine(stream, line, line_number)) {
        throw IOException("Failed to read table count");
    }
    int table_count = ParseInteger(line, line_number, line);

    if (!GetNextLine(stream, line, line_number)) {
        throw IOException(MakeError("Failed to read working hours", line_number, line));
    }

    std::istringstream iss_time(line);
    std::string open_time_str, close_time_str;

    if (!(iss_time >> open_time_str >> close_time_str)) {
        throw IOException(MakeError("Invalid working hours format", line_number, line));
    }
    CheckForTrailingData(iss_time, line_number, line);

    entities::Time open_time = ParseTime(open_time_str, line_number, line);
    entities::Time close_time = ParseTime(close_time_str, line_number, line);

    if (open_time >= close_time) {
        throw InvalidConfigurationException(MakeError(
            "Invalid working hours: open time must be less than close time", line_number, line));
    }

    if (!GetNextLine(stream, line, line_number)) {
        throw IOException(MakeError("Failed to read hourly rate", line_number, line));
    }
    int hourly_rate = ParseInteger(line, line_number, line);

    return {table_count, open_time, close_time, hourly_rate};
}

ClubData FileParser::Parse(const std::filesystem::path& file_path) {
    std::ifstream file_stream(file_path);

    if (!file_stream.is_open()) {
        throw IOException("Failed to open file: " + file_path.string());
    }

    return Parse(file_stream);
}
std::vector<entities::IncomingEvent> FileParser::ParseEvents(std::istream& stream,
                                                             size_t& line_number) {
    std::vector<entities::IncomingEvent> events;
    std::string line;

    while (GetNextLine(stream, line, line_number)) {
        std::istringstream event_stream(line);
        std::string time_str;
        int event_id;
        std::string client_name;

        if (!(event_stream >> time_str >> event_id)) {
            throw InvalidFormatException(MakeError("Invalid event format", line_number, line));
        }

        entities::Time event_time = ParseTime(time_str, line_number, line);

        if (!events.empty() && event_time < events.back().time) {
            throw InvalidEventException(
                MakeError("Invalid event order (time is not monotonous)", line_number, line));
        }

        switch (event_id) {
            case 1: {  // EventInClientArrives
                if (!(event_stream >> client_name)) {
                    throw InvalidFormatException(
                        MakeError("Expected client name", line_number, line));
                }
                events.push_back(
                    {event_time, entities::EventInClientArrives{std::move(client_name)}});
                break;
            }
            case 2: {  // EventInClientSits
                int table_number;
                if (!(event_stream >> client_name >> table_number)) {
                    throw InvalidFormatException(
                        MakeError("Expected <client> <table>", line_number, line));
                }
                events.push_back({event_time, entities::EventInClientSits{std::move(client_name),
                                                                          table_number}});
                break;
            }
            case 3: {  // EventInClientWaits
                if (!(event_stream >> client_name)) {
                    throw InvalidFormatException(MakeError("Expected <client>", line_number, line));
                }
                events.push_back(
                    {event_time, entities::EventInClientWaits{std::move(client_name)}});
                break;
            }
            case 4: {  // EventInClientLeaves
                if (!(event_stream >> client_name)) {
                    throw InvalidFormatException(MakeError("Expected <client>", line_number, line));
                }
                events.push_back(
                    {event_time, entities::EventInClientLeaves{std::move(client_name)}});
                break;
            }
            default:
                throw InvalidFormatException(MakeError("Unsupported Event Id", line_number, line));
        }

        CheckForTrailingData(event_stream, line_number, line);
    }

    return events;
}

int FileParser::ParseInteger(std::string_view int_str, size_t line_num, const std::string& line) {
    int value;

    auto [ptr, ec] = std::from_chars(int_str.data(), int_str.data() + int_str.size(), value);

    if (ec != std::errc() || ptr != int_str.data() + int_str.size()) {
        throw ParsingException(MakeError("Invalid integer format", line_num, line));
    }

    if (value <= 0) {
        throw ParsingException(MakeError("Integer must be positive", line_num, line));
    }

    return value;
}

entities::Time FileParser::ParseTime(std::string_view time_str, size_t line_num,
                                     const std::string& line) {
    if (time_str.size() != 5 || time_str[2] != ':') {
        throw ParsingException(MakeError("Invalid time format (must be HH:MM)", line_num, line));
    }

    int hours = -1, minutes = -1;
    const char* start = time_str.data();
    const char* end = start + time_str.size();
    auto [ptr1, ec1] = std::from_chars(start, start + 2, hours);
    auto [ptr2, ec2] = std::from_chars(start + 3, end, minutes);

    if (ec1 != std::errc() || ec2 != std::errc() || ptr1 != start + 2 || ptr2 != end) {
        throw ParsingException(MakeError("Invalid characters in time", line_num, line));
    }
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
        throw ParsingException(MakeError("Invalid time range", line_num, line));
    }
    return entities::Time(hours, minutes);
}

}  // namespace computer_club::utils
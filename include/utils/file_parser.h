#ifndef FILE_PARSER_H_
#define FILE_PARSER_H_

#include <string>
#include <vector>
#include "entities/club_configuration.h"
#include "entities/event.h"
#include <istream>
#include <filesystem>

namespace computer_club::utils {

struct ClubData {
    entities::ClubConfiguration configuration;
    std::vector<entities::IncomingEvent> events;
};

class FileParser {
public:
    FileParser() = delete;

    [[nodiscard]] static ClubData Parse(std::istream& stream);
    [[nodiscard]] static ClubData Parse(const std::filesystem::path& file_path);

private:
    [[nodiscard]] static bool GetNextLine(std::istream& stream, std::string& line,
                                          size_t& line_number);

    [[nodiscard]] static entities::ClubConfiguration ParseConfiguration(std::istream& stream,
                                                                        size_t& line_number);
    [[nodiscard]] static std::vector<entities::IncomingEvent> ParseEvents(std::istream& stream,
                                                                          size_t& line_number);

    [[nodiscard]] static entities::Time ParseTime(std::string_view time_str, size_t line_num,
                                                  const std::string& line);
    [[nodiscard]] static int ParseInteger(std::string_view int_str, size_t line_num,
                                          const std::string& line);

    static void CheckForTrailingData(std::istringstream& event_stream, size_t line_num,
                                     const std::string& line);
};
}  // namespace test_computer_club.cpp::utils
#endif  // FILE_PARSER_H_
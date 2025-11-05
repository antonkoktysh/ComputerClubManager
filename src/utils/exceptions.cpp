#include "utils/exceptions.h"

namespace computer_club::utils {

ComputerClubError::ComputerClubError(const std::string& message) : std::runtime_error(message) {
}

IOException::IOException(const std::string& message) : ComputerClubError(message) {
}

ParsingException::ParsingException(const std::string& message) : ComputerClubError(message) {
}

InvalidFormatException::InvalidFormatException(const std::string& message)
    : ParsingException(message) {
}

LogicException::LogicException(const std::string& message) : ComputerClubError(message) {
}

InvalidConfigurationException::InvalidConfigurationException(const std::string& message)
    : LogicException(message) {
}

InvalidEventException::InvalidEventException(const std::string& message) : LogicException(message) {
}

}  // namespace test_computer_club.cpp::utils
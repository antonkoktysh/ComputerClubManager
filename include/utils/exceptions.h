#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <stdexcept>
#include <string>

namespace computer_club::utils {

class ComputerClubError : public std::runtime_error {
public:
    explicit ComputerClubError(const std::string& message);
};

class IOException : public ComputerClubError {
public:
    explicit IOException(const std::string& message);
};
class ParsingException : public ComputerClubError {
public:
    explicit ParsingException(const std::string& message);
};

class InvalidFormatException : public ParsingException {
public:
    explicit InvalidFormatException(const std::string& message);
};

class LogicException : public ComputerClubError {
public:
    explicit LogicException(const std::string& message);
};

class InvalidConfigurationException : public LogicException {
public:
    explicit InvalidConfigurationException(const std::string& message);
};

class InvalidEventException : public LogicException {
public:
    explicit InvalidEventException(const std::string& message);
};

}  // namespace test_computer_club.cpp::utils
#endif  // EXCEPTIONS_H_
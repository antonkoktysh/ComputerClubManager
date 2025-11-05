#ifndef TABLE_H_
#define TABLE_H_

#include <string>
#include <string_view>
#include <optional>
#include <chrono>
#include "entities/time.h"

namespace computer_club::entities {

struct Session {
    std::string client_name;
    Time start_time;

    Session(std::string_view name, const Time& time) : client_name(name), start_time(time) {
    }
};

class Table {
public:
    explicit Table(int number) noexcept;

    [[nodiscard]] int GetNumber() const noexcept;
    [[nodiscard]] bool IsOccupied() const noexcept;

    [[nodiscard]] std::optional<std::string_view> GetClientName() const noexcept;
    [[nodiscard]] std::optional<Time> GetOccupiedSince() const noexcept;

    [[nodiscard]] std::chrono::minutes GetTotalMinutes() const noexcept;
    [[nodiscard]] int GetRevenue() const noexcept;

    void Occupy(std::string_view client_name, const Time& time);
    void Release(const Time& time, int hourly_rate) noexcept;

private:
    int number_;
    std::optional<Session> session_;
    std::chrono::minutes total_minutes_;
    int revenue_;
};

}  // namespace test_computer_club.cpp::entities
#endif  // TABLE_H_
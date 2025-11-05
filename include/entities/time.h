#ifndef TIME_H_
#define TIME_H_

#include <stdexcept>
#include <string>
#include <compare>
#include <chrono>
#include <format>

namespace computer_club::entities {

class Time {
public:
    static constexpr int kMinutesInDay = 24 * 60;

    Time() noexcept;

    Time(int hours, int minutes);

    [[nodiscard]] std::string ToString() const;

    [[nodiscard]] int Hours() const noexcept;
    [[nodiscard]] int Minutes() const noexcept;

    auto operator<=>(const Time& other) const noexcept = default;

    [[nodiscard]] std::chrono::minutes operator-(const Time& other) const noexcept;

    [[nodiscard]] Time operator+(std::chrono::minutes minutes) const noexcept;

    friend Time operator+(std::chrono::minutes minutes, const Time& time) noexcept;

    [[nodiscard]] Time operator-(std::chrono::minutes minutes) const noexcept;

    Time& operator+=(std::chrono::minutes minutes) noexcept;
    Time& operator-=(std::chrono::minutes minutes) noexcept;

    [[nodiscard]] std::chrono::minutes GetTimeSinceMidnight() const noexcept;

private:
    explicit Time(std::chrono::minutes time_since_midnight) noexcept;

    std::chrono::minutes time_since_midnight_;
};

struct WorkingTime {
    Time open;
    Time close;
    [[nodiscard]] bool IsOpenAt(const Time& time) const;
};

}  // namespace computer_club::entities
#endif  // TIME_H_
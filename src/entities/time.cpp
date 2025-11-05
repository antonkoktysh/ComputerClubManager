#include "entities/time.h"
#include <sstream>
#include <format>
#include <chrono>

namespace computer_club::entities {

Time::Time() noexcept : time_since_midnight_(0) {
}

Time::Time(int hours, int minutes) {
    time_since_midnight_ = std::chrono::hours(hours) + std::chrono::minutes(minutes);
}

Time::Time(std::chrono::minutes time_since_midnight) noexcept
    : time_since_midnight_(time_since_midnight) {
}

std::string Time::ToString() const {
    return std::format("{:02}:{:02}", Hours(), Minutes());
}

int Time::Hours() const noexcept {
    return std::chrono::duration_cast<std::chrono::hours>(time_since_midnight_).count();
}

int Time::Minutes() const noexcept {
    return (time_since_midnight_ % std::chrono::hours(1)).count();
}

std::chrono::minutes Time::GetTimeSinceMidnight() const noexcept {
    return time_since_midnight_;
}

std::chrono::minutes Time::operator-(const Time& other) const noexcept {
    return this->time_since_midnight_ - other.time_since_midnight_;
}

Time& Time::operator+=(std::chrono::minutes minutes) noexcept {
    auto total_minutes = (this->time_since_midnight_.count() + minutes.count()) % kMinutesInDay;
    if (total_minutes < 0) {
        total_minutes += kMinutesInDay;
    }
    time_since_midnight_ = std::chrono::minutes(total_minutes);
    return *this;
}

Time& Time::operator-=(std::chrono::minutes minutes) noexcept {
    *this += (-minutes);
    return *this;
}

Time Time::operator+(std::chrono::minutes minutes) const noexcept {
    Time result = *this;
    result += minutes;
    return result;
}

Time Time::operator-(std::chrono::minutes minutes) const noexcept {
    Time result = *this;
    result -= minutes;
    return result;
}

Time operator+(std::chrono::minutes minutes, const Time& time) noexcept {
    return time + minutes;
}

bool WorkingTime::IsOpenAt(const Time& time) const {
    return time >= open && time <= close;
}

}  // namespace computer_club::entities
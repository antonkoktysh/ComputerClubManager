#include "entities/table.h"
#include <cassert>
#include <chrono>

namespace computer_club::entities {

Table::Table(int number) noexcept
    : number_(number), session_(), total_minutes_{std::chrono::minutes::zero()}, revenue_(0) {
}

int Table::GetNumber() const noexcept {
    return number_;
}

bool Table::IsOccupied() const noexcept {
    return session_.has_value();
}

std::optional<std::string_view> Table::GetClientName() const noexcept {
    if (session_) {
        return session_->client_name;
    }
    return std::nullopt;
}

std::optional<Time> Table::GetOccupiedSince() const noexcept {
    if (session_) {
        return session_->start_time;
    }
    return std::nullopt;
}

std::chrono::minutes Table::GetTotalMinutes() const noexcept {
    return total_minutes_;
}

int Table::GetRevenue() const noexcept {
    return revenue_;
}

void Table::Occupy(std::string_view client_name, const Time& time) {
    assert(!IsOccupied() && "Table is already occupied");
    session_.emplace(client_name, time);
}

void Table::Release(const Time& time, int hourly_rate) noexcept {
    if (!session_) {
        return;
    }

    int usage_minutes_int = (time - session_->start_time).count();
    assert(usage_minutes_int >= 0 && "Release time cannot be before start time");

    if (usage_minutes_int < 0) {
        usage_minutes_int = 0;
    }
    const auto usage_minutes = std::chrono::minutes(usage_minutes_int);

    total_minutes_ += usage_minutes;
    const auto hours_used = std::chrono::ceil<std::chrono::hours>(usage_minutes);

    revenue_ += hours_used.count() * hourly_rate;

    session_.reset();
}

}  // namespace computer_club::entities
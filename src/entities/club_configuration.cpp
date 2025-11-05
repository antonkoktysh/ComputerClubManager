#include "entities/club_configuration.h"

namespace computer_club::entities {

ClubConfiguration::ClubConfiguration(int table_count, const Time& open_time, const Time& close_time,
                                     int hourly_rate)
    : table_count_(table_count), working_time_{open_time, close_time}, hourly_rate_(hourly_rate) {
}

int ClubConfiguration::GetTableCount() const noexcept {
    return table_count_;
}

Time ClubConfiguration::GetOpenTime() const noexcept {
    return working_time_.open;
}

Time ClubConfiguration::GetCloseTime() const noexcept {
    return working_time_.close;
}
WorkingTime ClubConfiguration::GetWorkingTime() const noexcept {
    return working_time_;
}
int ClubConfiguration::GetHourlyRate() const noexcept {
    return hourly_rate_;
}

bool ClubConfiguration::IsOpenAt(const Time& time) const noexcept {
    return working_time_.IsOpenAt(time);
}

}  // namespace test_computer_club.cpp::entities

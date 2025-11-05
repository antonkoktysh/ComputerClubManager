#ifndef CLUB_CONFIGURATION_H_
#define CLUB_CONFIGURATION_H_

#include "entities/time.h"

namespace computer_club::entities {

class ClubConfiguration {
public:
    ClubConfiguration(int table_count, const Time& open_time, const Time& close_time,
                      int hourly_rate);

    [[nodiscard]] int GetTableCount() const noexcept;
    [[nodiscard]] Time GetOpenTime() const noexcept;
    [[nodiscard]] Time GetCloseTime() const noexcept;
    [[nodiscard]] WorkingTime GetWorkingTime() const noexcept;
    [[nodiscard]] int GetHourlyRate() const noexcept;
    [[nodiscard]] bool IsOpenAt(const Time& time) const noexcept;

private:
    int table_count_;
    WorkingTime working_time_;
    int hourly_rate_;
};

}  // namespace test_computer_club.cpp::entities
#endif  // CLUB_CONFIGURATION_H_
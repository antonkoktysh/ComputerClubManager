#ifndef RESULT_PRINTER_H_
#define RESULT_PRINTER_H_

#include <ostream>
#include "core/computer_club_manager_impl.h"

namespace computer_club::utils {
struct EventPrintVisitor {
    std::ostream& out;

    std::string FormatTime(const entities::Time& time) const;

    void operator()(const entities::IncomingEvent& event) const;

    void operator()(const entities::OutgoingEvent& event) const;
};

void PrintResults(std::ostream& out, const core::ComputerClubManager& manager);
}  // namespace test_computer_club.cpp::utils
#endif  // RESULT_PRINTER_H_
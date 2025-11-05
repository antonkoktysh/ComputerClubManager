#include "utils/result_printer.h"
#include <format>
#include "utils/overloaded.h"
#include "entities/event.h"

namespace computer_club::utils {

namespace {
std::string_view ErrorToString(entities::ErrorType error_id) {
    using entities::ErrorType;
    switch (error_id) {
        case ErrorType::NotOpenYet:
            return "NotOpenYet";
        case ErrorType::YouShallNotPass:
            return "YouShallNotPass";
        case ErrorType::ClientUnknown:
            return "ClientUnknown";
        case ErrorType::PlaceIsBusy:
            return "PlaceIsBusy";
        case ErrorType::ICanWaitNoLonger:
            return "ICanWaitNoLonger";
    }
    return "UnknownError";
}
}  // namespace

std::string EventPrintVisitor::FormatTime(const entities::Time& time) const {
    return time.ToString();
}

void EventPrintVisitor::operator()(const entities::IncomingEvent& event) const {
    out << FormatTime(event.time) << " ";
    std::visit(utils::Overloaded{[&](const entities::EventInClientArrives& payload) {
                                     out << payload.kEventId << " " << payload.client;
                                 },
                                 [&](const entities::EventInClientSits& payload) {
                                     out << payload.kEventId << " " << payload.client << " "
                                         << payload.table_id;
                                 },
                                 [&](const entities::EventInClientWaits& payload) {
                                     out << payload.kEventId << " " << payload.client;
                                 },
                                 [&](const entities::EventInClientLeaves& payload) {
                                     out << payload.kEventId << " " << payload.client;
                                 }},
               event.payload);
}

void EventPrintVisitor::operator()(const entities::OutgoingEvent& event) const {
    out << FormatTime(event.time) << " ";
    std::visit(utils::Overloaded{[&](const entities::EventOutClientLeaves& payload) {
                                     out << payload.kEventId << " " << payload.client;
                                 },
                                 [&](const entities::EventOutClientSits& payload) {
                                     out << payload.kEventId << " " << payload.client << " "
                                         << payload.table_id;
                                 },
                                 [&](const entities::EventOutError& payload) {
                                     out << payload.kEventId << " "
                                         << ErrorToString(payload.error_id);
                                 }},
               event.payload);
}
void PrintResults(std::ostream& out, const core::ComputerClubManager& manager) {
    const auto& config = manager.GetConfiguration();
    const auto& events = manager.GetEvents();
    const auto& tables = manager.GetTables();

    out << std::format("{}\n", config.GetOpenTime().ToString());

    for (const auto& event : events) {
        std::visit(EventPrintVisitor{out}, event);
        out << "\n";
    }

    out << std::format("{}\n", config.GetCloseTime().ToString());
    for (const auto& table : tables) {
        out << std::format("{} {} {:%H:%M}\n", table->GetNumber(), table->GetRevenue(),
                           table->GetTotalMinutes());
    }
}
}  // namespace computer_club::utils

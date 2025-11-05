#include "services/event_repository.h"

namespace computer_club::services {

void EventRepository::AddEvent(const entities::IncomingEvent& event) {
    events_.push_back(event);
}

void EventRepository::AddEvent(entities::OutgoingEvent&& event) {
    events_.push_back(std::move(event));
}

const std::vector<entities::Event>& EventRepository::GetEvents() const noexcept {
    return events_;
}

}  // namespace test_computer_club.cpp::services

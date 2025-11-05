#ifndef EVENT_REPOSITORY_H_
#define EVENT_REPOSITORY_H_

#include <vector>
#include "entities/event.h"

namespace computer_club::services {

class EventRepository {
public:
    EventRepository() = default;

    void AddEvent(const entities::IncomingEvent& event);
    void AddEvent(entities::OutgoingEvent&& event);
    [[nodiscard]] const std::vector<entities::Event>& GetEvents() const noexcept;

private:
    std::vector<entities::Event> events_;
};

}  // namespace test_computer_club.cpp::services
#endif  // EVENT_REPOSITORY_H_
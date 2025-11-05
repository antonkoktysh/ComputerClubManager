#ifndef EVENT_H_
#define EVENT_H_

#include <string>
#include "entities/time.h"

namespace computer_club::entities {

enum class ErrorType { NotOpenYet, YouShallNotPass, ClientUnknown, PlaceIsBusy, ICanWaitNoLonger };

struct EventInClientArrives {
    static constexpr int kEventId = 1;
    std::string client;
};

struct EventInClientSits {
    static constexpr int kEventId = 2;
    std::string client;
    int table_id;
};

struct EventInClientWaits {
    static constexpr int kEventId = 3;
    std::string client;
};

struct EventInClientLeaves {
    static constexpr int kEventId = 4;
    std::string client;
};

struct EventOutClientLeaves {
    static constexpr int kEventId = 11;
    std::string client;
};

struct EventOutClientSits {
    static constexpr int kEventId = 12;
    std::string client;
    int table_id;
};

struct EventOutError {
    static constexpr int kEventId = 13;
    ErrorType error_id;
};

using IncomingPayload =
    std::variant<EventInClientArrives, EventInClientSits, EventInClientWaits, EventInClientLeaves>;

using OutgoingPayload = std::variant<EventOutClientLeaves, EventOutClientSits, EventOutError>;

struct IncomingEvent {
    Time time;
    IncomingPayload payload;
};

struct OutgoingEvent {
    Time time;
    OutgoingPayload payload;
};

using Event = std::variant<IncomingEvent, OutgoingEvent>;

}  // namespace test_computer_club.cpp::entities
#endif  // EVENT_H_
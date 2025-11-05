#ifndef COMPUTER_CLUB_MANAGER_H_
#define COMPUTER_CLUB_MANAGER_H_

#include <vector>
#include <memory>
#include "entities/event.h"
#include "entities/client.h"
#include "entities/club_configuration.h"
#include "entities/table.h"
namespace computer_club::core {

class ComputerClubManager {
public:
    virtual void ProcessEvents(const std::vector<entities::IncomingEvent>& incoming_events) = 0;
    virtual void ProcessEvent(const entities::IncomingEvent& incoming_event) = 0;
    virtual void Close() = 0;
    virtual ~ComputerClubManager() = default;
    [[nodiscard]] virtual const std::vector<entities::Event>& GetEvents() const noexcept = 0;
    [[nodiscard]] virtual const entities::ClubConfiguration& GetConfiguration() const noexcept = 0;
    [[nodiscard]] virtual const std::vector<std::shared_ptr<entities::Table>>& GetTables()
        const = 0;
};

}  // namespace test_computer_club.cpp::core

#endif  // COMPUTER_CLUB_MANAGER_H_
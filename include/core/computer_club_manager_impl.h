#ifndef COMPUTER_CLUB_MANAGER_IMPL_H_
#define COMPUTER_CLUB_MANAGER_IMPL_H_

#include <memory>
#include "core/computer_club_manager.h"
#include "entities/club_configuration.h"
#include "services/client_repository.h"
#include "services/event_repository.h"
#include "services/table_repository.h"

namespace computer_club::core {

class ComputerClubManagerImpl final : public ComputerClubManager {
public:
    explicit ComputerClubManagerImpl(const entities::ClubConfiguration& config);
    ~ComputerClubManagerImpl() override = default;

    ComputerClubManagerImpl(const ComputerClubManagerImpl&) = delete;
    ComputerClubManagerImpl& operator=(const ComputerClubManagerImpl&) = delete;
    ComputerClubManagerImpl(ComputerClubManagerImpl&&) noexcept = default;
    ComputerClubManagerImpl& operator=(ComputerClubManagerImpl&&) noexcept = default;

    void ProcessEvents(const std::vector<entities::IncomingEvent>& incoming_events) override;
    void ProcessEvent(const entities::IncomingEvent& incoming_event) override;
    void Close() override;

    [[nodiscard]] const entities::ClubConfiguration& GetConfiguration() const noexcept override;
    [[nodiscard]] const std::vector<entities::Event>& GetEvents() const noexcept override;
    [[nodiscard]] const std::vector<std::shared_ptr<entities::Table>>& GetTables() const override;

private:
    entities::ClubConfiguration config_;
    std::unique_ptr<services::EventRepository> event_repository_;
    std::unique_ptr<services::ClientRepository> client_repository_;
    std::unique_ptr<services::TableRepository> table_repository_;

    [[nodiscard]] bool IsOpenAt(const entities::Time& time) const noexcept;
    void OnClientArrival(const entities::Time& time, const entities::EventInClientArrives& payload);
    void OnClientSitting(const entities::Time& time, const entities::EventInClientSits& payload);
    void OnClientWaiting(const entities::Time& time, const entities::EventInClientWaits& payload);
    void OnClientLeaving(const entities::Time& time, const entities::EventInClientLeaves& payload);

    void AddErrorEvent(const entities::Time& time, entities::ErrorType error_id);
    void ProcessClientFromQueue(const entities::Time& time, int table_number);
};

}  // namespace computer_club::core

#endif  // COMPUTER_CLUB_CONTROLLER_IMPL_H_
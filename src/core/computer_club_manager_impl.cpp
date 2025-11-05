#include "core/computer_club_manager_impl.h"
#include "utils/overloaded.h"
namespace computer_club::core {

ComputerClubManagerImpl::ComputerClubManagerImpl(const entities::ClubConfiguration& config)
    : config_(config) {
    event_repository_ = std::make_unique<services::EventRepository>();
    client_repository_ = std::make_unique<services::ClientRepository>();
    table_repository_ = std::make_unique<services::TableRepository>(config.GetTableCount());
}

void ComputerClubManagerImpl::ProcessEvents(
    const std::vector<entities::IncomingEvent>& incoming_events) {
    for (auto&& event : incoming_events) {
        ProcessEvent(event);
    }
}

void ComputerClubManagerImpl::ProcessEvent(const entities::IncomingEvent& event) {
    event_repository_->AddEvent(event);
    std::visit(utils::Overloaded{[&](const entities::EventInClientArrives& payload) {
                                     OnClientArrival(event.time, payload);
                                 },
                                 [&](const entities::EventInClientSits& payload) {
                                     OnClientSitting(event.time, payload);
                                 },
                                 [&](const entities::EventInClientWaits& payload) {
                                     OnClientWaiting(event.time, payload);
                                 },
                                 [&](const entities::EventInClientLeaves& payload) {
                                     OnClientLeaving(event.time, payload);
                                 }},
               event.payload);
}

void ComputerClubManagerImpl::Close() {
    auto remaining_clients = client_repository_->GetClients();

    for (const auto& client : remaining_clients) {
        if (client && client->IsSeated()) {
            auto table_number = client->GetTableNumber();
            table_repository_->ReleaseTable(table_number.value(), config_.GetCloseTime(),
                                            config_.GetHourlyRate());
        }

        event_repository_->AddEvent(entities::OutgoingEvent{
            config_.GetCloseTime(), entities::EventOutClientLeaves{client->GetName()}});
    }
}

const std::vector<entities::Event>& ComputerClubManagerImpl::GetEvents() const noexcept {
    return event_repository_->GetEvents();
}

const entities::ClubConfiguration& ComputerClubManagerImpl::GetConfiguration() const noexcept {
    return config_;
}

const std::vector<std::shared_ptr<entities::Table>>& ComputerClubManagerImpl::GetTables() const {
    return table_repository_->GetAllTables();
}

bool ComputerClubManagerImpl::IsOpenAt(const entities::Time& time) const noexcept {
    return config_.IsOpenAt(time);
}

void ComputerClubManagerImpl::OnClientArrival(const entities::Time& time,
                                              const entities::EventInClientArrives& payload) {
    const auto& client_name = payload.client;
    if (!IsOpenAt(time)) {
        AddErrorEvent(time, entities::ErrorType::NotOpenYet);
        return;
    }

    if (client_repository_->HasClient(client_name)) {
        AddErrorEvent(time, entities::ErrorType::YouShallNotPass);
        return;
    }
    client_repository_->AddClient(client_name);
}

void ComputerClubManagerImpl::OnClientSitting(const entities::Time& time,
                                              const entities::EventInClientSits& payload) {
    auto& client_name = payload.client;
    auto table_number = payload.table_id;

    if (!client_repository_->HasClient(client_name)) {
        AddErrorEvent(time, entities::ErrorType::ClientUnknown);
        return;
    }

    auto client = client_repository_->GetClient(client_name);
    if (table_repository_->IsTableOccupied(table_number)) {
        AddErrorEvent(time, entities::ErrorType::PlaceIsBusy);
        return;
    }

    if (client->IsSeated()) {
        auto old_table = client->GetTableNumber();
        table_repository_->ReleaseTable(old_table.value(), time, config_.GetHourlyRate());
    }

    table_repository_->OccupyTable(table_number, client_name, time);
    client->SetSitting(table_number);
}

void ComputerClubManagerImpl::OnClientWaiting(const entities::Time& time,
                                              const entities::EventInClientWaits& payload) {
    const auto& client_name = payload.client;

    if (table_repository_->GetAvailableTableCount() > 0) {
        AddErrorEvent(time, entities::ErrorType::ICanWaitNoLonger);
        return;
    }
    if (!client_repository_->HasClient(client_name)) {
        client_repository_->AddClient(client_name);
    }

    auto client = client_repository_->GetClient(client_name);

    if (client->IsSeated()) {
        const auto old_table = client->GetTableNumber();
        table_repository_->ReleaseTable(old_table.value(), time, config_.GetHourlyRate());
        client->SetLeft();
    }

    if (client_repository_->GetWaitingListSize() > table_repository_->GetTableCount()) {
        client_repository_->RemoveClient(client_name);
        event_repository_->AddEvent(
            entities::OutgoingEvent{time, entities::EventOutClientLeaves{client_name}});
        return;
    }

    client_repository_->EnqueueWaiting(client_name);
    client->SetWaiting();
}

void ComputerClubManagerImpl::OnClientLeaving(const entities::Time& time,
                                              const entities::EventInClientLeaves& payload) {
    const auto& client_name = payload.client;

    if (!client_repository_->HasClient(client_name)) {
        AddErrorEvent(time, entities::ErrorType::ClientUnknown);
        return;
    }

    if (const auto client = client_repository_->GetClient(client_name); client->IsSeated()) {
        const auto table_number = client->GetTableNumber();
        table_repository_->ReleaseTable(table_number.value(), time, config_.GetHourlyRate());
        ProcessClientFromQueue(time, table_number.value());
    }
    client_repository_->RemoveClient(client_name);
}

void ComputerClubManagerImpl::AddErrorEvent(const entities::Time& time,
                                            entities::ErrorType error_id) {
    event_repository_->AddEvent(entities::OutgoingEvent{time, entities::EventOutError{error_id}});
}

void ComputerClubManagerImpl::ProcessClientFromQueue(const entities::Time& time, int table_number) {
    if (client_repository_->IsWaitingListEmpty()) {
        return;
    }

    const auto next_client_name = client_repository_->DequeueWaiting();
    if (next_client_name.empty()) {
        return;
    }

    const auto next_client = client_repository_->GetClient(next_client_name);

    table_repository_->OccupyTable(table_number, next_client_name, time);
    next_client->SetSitting(table_number);
    event_repository_->AddEvent(entities::OutgoingEvent{
        time, entities::EventOutClientSits{next_client_name, table_number}});
}

}  // namespace computer_club::core

#include "entities/client.h"
#include <utility>

namespace computer_club::entities {

Client::Client(std::string name) noexcept
    : name_(std::move(name)), state_(ClientState::kClientWaiting), table_number_(std::nullopt) {
}

const std::string& Client::GetName() const noexcept {
    return name_;
}

bool Client::IsSeated() const noexcept {
    return state_ == ClientState::kClientSitting;
}

bool Client::IsWaiting() const noexcept {
    return state_ == ClientState::kClientWaiting;
}

std::optional<int> Client::GetTableNumber() const noexcept {
    return table_number_;
}

void Client::SetSitting(int table_number) noexcept {
    state_ = ClientState::kClientSitting;
    table_number_ = table_number;
}
void Client::SetWaiting() noexcept {
    state_ = ClientState::kClientWaiting;
    table_number_ = std::nullopt;
}

void Client::SetLeft() noexcept {
    state_ = ClientState::kClientWaiting;
    table_number_ = std::nullopt;
}

}  // namespace computer_club::entities
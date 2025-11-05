#include "services/client_repository.h"
#include <algorithm>
#include <ranges>
#include <list>
#include <iterator>

namespace computer_club::services {

bool ClientRepository::HasClient(const std::string& client_name) const noexcept {
    return clients_.contains(client_name);
}

std::shared_ptr<entities::Client> ClientRepository::GetClient(const std::string& client_name) {
    if (const auto it = clients_.find(client_name); it != clients_.end()) {
        return it->second;
    }
    return nullptr;
}

void ClientRepository::AddClient(const std::string& client_name) {
    clients_.try_emplace(client_name, std::make_shared<entities::Client>(client_name));
}

void ClientRepository::RemoveClient(const std::string& client_name) noexcept {
    clients_.erase(client_name);
    waiting_list_.remove(client_name);
}

void ClientRepository::EnqueueWaiting(const std::string& client_name) {
    waiting_list_.push_back(client_name);
}

std::string ClientRepository::DequeueWaiting() {
    if (waiting_list_.empty()) {
        return "";
    }
    std::string client_name = waiting_list_.front();
    waiting_list_.pop_front();
    return client_name;
}

std::vector<std::string> ClientRepository::GetClientNames() const {
    std::vector<std::string> names;
    names.reserve(clients_.size());
    std::ranges::copy(clients_ | std::views::keys, std::back_inserter(names));
    return names;
}

bool ClientRepository::IsWaitingListEmpty() const noexcept {
    return waiting_list_.empty();
}

size_t ClientRepository::GetWaitingListSize() const noexcept {
    return waiting_list_.size();
}

std::vector<std::shared_ptr<entities::Client>> ClientRepository::GetClients() const {
    std::vector<std::shared_ptr<entities::Client>> all_clients;
    all_clients.reserve(clients_.size());
    std::ranges::copy(clients_ | std::views::values, std::back_inserter(all_clients));
    return all_clients;
}

}  // namespace computer_club::services

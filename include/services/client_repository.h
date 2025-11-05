#ifndef CLIENT_REPOSITORY_H_
#define CLIENT_REPOSITORY_H_
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "entities/client.h"
#include <list>

namespace computer_club::services {

class ClientRepository {
public:
    ClientRepository() = default;

    [[nodiscard]] bool HasClient(const std::string& client_name) const noexcept;
    [[nodiscard]] std::shared_ptr<entities::Client> GetClient(const std::string& client_name);
    void AddClient(const std::string& client_name);
    void RemoveClient(const std::string& client_name) noexcept;

    [[nodiscard]] std::vector<std::string> GetClientNames() const;

    void EnqueueWaiting(const std::string& client_name);
    std::string DequeueWaiting();
    [[nodiscard]] bool IsWaitingListEmpty() const noexcept;
    [[nodiscard]] size_t GetWaitingListSize() const noexcept;
    [[nodiscard]] std::vector<std::shared_ptr<entities::Client>> GetClients() const;

private:
    std::map<std::string, std::shared_ptr<entities::Client>> clients_;
    std::list<std::string> waiting_list_;
};

}  // namespace computer_club::services
#endif  // CLIENT_REPOSITORY_H_
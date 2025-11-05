#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <optional>

namespace computer_club::entities {

enum class ClientState { kClientWaiting, kClientSitting };

class Client {
public:
    explicit Client(std::string name) noexcept;

    [[nodiscard]] const std::string& GetName() const noexcept;
    [[nodiscard]] bool IsSeated() const noexcept;
    [[nodiscard]] bool IsWaiting() const noexcept;

    std::optional<int> GetTableNumber() const noexcept;

    void SetSitting(int table_number) noexcept;

    void SetWaiting() noexcept;

    void SetLeft() noexcept;

private:
    std::string name_;
    ClientState state_;
    std::optional<int> table_number_;
};

}  // namespace test_computer_club.cpp::entities
#endif  // CLIENT_H_
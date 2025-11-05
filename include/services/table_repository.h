#ifndef TABLE_REPOSITORY_H_
#define TABLE_REPOSITORY_H_

#include <memory>
#include <vector>
#include "entities/table.h"

namespace computer_club::services {

class TableRepository {
public:
    explicit TableRepository(int table_count);

    [[nodiscard]] int GetTableCount() const noexcept;
    [[nodiscard]] bool IsTableOccupied(int table_number) const;
    std::shared_ptr<entities::Table> GetTable(int table_number);

    void OccupyTable(int table_number, const std::string& client_name, const entities::Time& time);
    void ReleaseTable(int table_number, const entities::Time& time, int hourly_rate);

    [[nodiscard]] int GetAvailableTableCount() const noexcept;
    [[nodiscard]] const std::vector<std::shared_ptr<entities::Table>>& GetAllTables() const;

private:
    int table_count_;
    std::vector<std::shared_ptr<entities::Table>> tables_;
};

}  // namespace test_computer_club.cpp::services
#endif  // TABLE_REPOSITORY_H_
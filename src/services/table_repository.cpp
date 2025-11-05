#include "services/table_repository.h"
#include "utils/exceptions.h"
#include <ranges>
#include <cstdint>
#include <algorithm>
#include <iterator>

namespace computer_club::services {

TableRepository::TableRepository(int table_count) : table_count_(table_count) {
    if (table_count <= 0) {
        throw std::invalid_argument("Table count must be positive");
    }
    tables_.reserve(table_count_);
    for (int i = 1; i <= table_count; ++i) {
        tables_.push_back(std::make_shared<entities::Table>(i));
    }
}

int TableRepository::GetTableCount() const noexcept {
    return table_count_;
}

bool TableRepository::IsTableOccupied(int table_number) const {
    return tables_.at(table_number - 1)->IsOccupied();
}

std::shared_ptr<entities::Table> TableRepository::GetTable(int table_number) {
    return tables_.at(table_number - 1);
}

void TableRepository::OccupyTable(int table_number, const std::string& client_name,
                                  const entities::Time& time) {
    tables_.at(table_number - 1)->Occupy(client_name, time);
}

void TableRepository::ReleaseTable(int table_number, const entities::Time& time, int hourly_rate) {
    tables_.at(table_number - 1)->Release(time, hourly_rate);
}

int TableRepository::GetAvailableTableCount() const noexcept {
    return std::ranges::count_if(tables_,
                                 [](const auto& table_ptr) { return !table_ptr->IsOccupied(); });
}

const std::vector<std::shared_ptr<entities::Table>>& TableRepository::GetAllTables() const {
    return tables_;
}

}  // namespace test_computer_club.cpp::services

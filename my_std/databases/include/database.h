//
// Created by lhy on 24-9-24.
//

#ifndef DATABASE_H
#define DATABASE_H

#include <mysqlx/xdevapi.h>

namespace lhy {
class DataBase {
 public:
  class Condition {
   public:
    Condition& Equal(const std::string& column, const std::string& value) {
      conditions_.push_back(column + " = '" + value + "'");
      return *this;
    }

    Condition& GreaterThan(const std::string& column, const std::string& value) {
      conditions_.push_back(column + " > '" + value + "'");
      return *this;
    }

    Condition& LessThan(const std::string& column, const std::string& value) {
      conditions_.push_back(column + " < '" + value + "'");
      return *this;
    }

    Condition& And() {
      conditions_.emplace_back(" AND ");
      return *this;
    }

    Condition& Or() {
      conditions_.emplace_back(" OR ");
      return *this;
    }

    [[nodiscard]] std::string ToString() const {
      std::string result;
      for (const auto& condition : conditions_) {
        result += condition;
      }
      return result;
    }

   private:
    std::vector<std::string> conditions_;
  };
  class MysqlRow {
   public:
    MysqlRow(mysqlx::Row row, const std::vector<std::string>& columns) {
      for (size_t i = 0; i < row.colCount(); i++) {
        row_[columns[i]] = row[i];
      }
    }
    mysqlx::Value operator[](const std::string& column) const { return row_.at(column); }

   private:
    std::map<std::string, mysqlx::Value> row_;
  };
  static std::string str(auto value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
  }
  DataBase();
  void Open(const std::string& schema, const std::string& table);
  DataBase& operator=(DataBase&& other) = delete;
  ~DataBase() = default;
  template <typename... Args>
  std::vector<MysqlRow> SelectColumns(const std::tuple<Args...>& columns, const Condition& condition = Condition());
  template <typename... ArgsValues>
  bool InsertRow(const std::vector<std::string>& columns, ArgsValues&&... values);

 private:
  inline static mysqlx::Client client_{"root:111111@localhost:33060", mysqlx::ClientOption::POOL_MAX_SIZE, 7};
  mysqlx::Session session_;
  std::optional<mysqlx::Schema> schema_;
  std::optional<mysqlx::Table> table_;
  // inline static std::vector<std::wstring> ORDERSTRING = {L"Hero ASC", L"Mana ASC", L"ID ASC"};
};
template <typename... ArgsValues>
bool DataBase::InsertRow(const std::vector<std::string>& columns, ArgsValues&&... values) {
  // Ensure that the number of columns matches the number of values at compile-time
  assert(sizeof...(values) == columns.size());
  // Create an insert query
  auto insert_query = table_->insert(columns);

  // Insert the values
  insert_query.values(std::forward<std::string>(str(values))...).execute();

  return true;
}
template <typename... Args>
std::vector<DataBase::MysqlRow> DataBase::SelectColumns(const std::tuple<Args...>& columns,
                                                        const Condition& condition) {
  mysqlx::RowResult result;

  auto columns_vector = std::apply(
      [&](auto... args) -> std::vector<std::string> {
        if (!condition.ToString().empty()) {
          result = table_->select(args...).where(condition.ToString()).execute();
        } else {
          result = table_->select(args...).execute();
        }
        return {args...};
      },
      columns);

  std::vector<MysqlRow> rows;
  for (auto row : result) {
    rows.emplace_back(row, columns_vector);
  }
  return rows;
}
}  // namespace lhy

#endif  // DATABASE_H

#pragma once

#include <sqlite3.h>

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class Database {
public:
    using Row = std::unordered_map<std::string, std::string>;
    using Parameter = std::optional<std::string>;

    explicit Database(const std::string& dbPath);
    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    void open();
    void createTables();
    void execute(const std::string& sql);
    std::vector<Row> query(const std::string& sql);
    void executePrepared(const std::string& sql, const std::vector<Parameter>& parameters);
    std::vector<Row> queryPrepared(const std::string& sql, const std::vector<Parameter>& parameters);
    int lastInsertId() const;
    bool isEmpty(const std::string& tableName);

private:
    void bindParameters(sqlite3_stmt* statement, const std::vector<Parameter>& parameters);

    std::string dbPath_;
    sqlite3* db_;
};

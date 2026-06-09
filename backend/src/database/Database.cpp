#include "database/Database.h"

#include "utils/AppError.h"

#include <utility>

Database::Database(const std::string& dbPath)
    : dbPath_(dbPath), db_(nullptr) {}

Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
    }
}

void Database::open() {
    if (sqlite3_open(dbPath_.c_str(), &db_) != SQLITE_OK) {
        throw AppError(500, db_ ? sqlite3_errmsg(db_) : "Unable to open database.");
    }

    execute("PRAGMA foreign_keys = ON;");
}

void Database::createTables() {
    execute(R"SQL(
        CREATE TABLE IF NOT EXISTS engineers (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            full_name TEXT NOT NULL,
            email TEXT NOT NULL UNIQUE,
            role TEXT NOT NULL,
            department TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );

        CREATE TABLE IF NOT EXISTS tickets (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            description TEXT NOT NULL,
            requester_name TEXT NOT NULL,
            requester_email TEXT NOT NULL,
            category TEXT NOT NULL,
            priority TEXT NOT NULL,
            status TEXT NOT NULL DEFAULT 'Open',
            assigned_engineer_id INTEGER NULL REFERENCES engineers(id) ON DELETE SET NULL,
            resolution_notes TEXT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );

        CREATE TABLE IF NOT EXISTS activity_logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            ticket_id INTEGER NOT NULL REFERENCES tickets(id) ON DELETE CASCADE,
            action TEXT NOT NULL,
            description TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )SQL");
}

void Database::execute(const std::string& sql) {
    char* errorMessage = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::string error = errorMessage ? errorMessage : "Database command failed.";
        sqlite3_free(errorMessage);
        throw AppError(500, error);
    }
}

std::vector<Database::Row> Database::query(const std::string& sql) {
    std::vector<Row> rows;
    auto callback = [](void* data, int columnCount, char** values, char** columnNames) -> int {
        auto* resultRows = static_cast<std::vector<Row>*>(data);
        Row row;

        for (int i = 0; i < columnCount; ++i) {
            row[columnNames[i]] = values[i] ? values[i] : "";
        }

        resultRows->push_back(row);
        return 0;
    };

    char* errorMessage = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), callback, &rows, &errorMessage) != SQLITE_OK) {
        std::string error = errorMessage ? errorMessage : "Database query failed.";
        sqlite3_free(errorMessage);
        throw AppError(500, error);
    }

    return rows;
}

void Database::executePrepared(const std::string& sql, const std::vector<Parameter>& parameters) {
    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
        throw AppError(500, sqlite3_errmsg(db_));
    }

    bindParameters(statement, parameters);
    int result = sqlite3_step(statement);
    sqlite3_finalize(statement);

    if (result != SQLITE_DONE) {
        throw AppError(500, sqlite3_errmsg(db_));
    }
}

std::vector<Database::Row> Database::queryPrepared(const std::string& sql, const std::vector<Parameter>& parameters) {
    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
        throw AppError(500, sqlite3_errmsg(db_));
    }

    bindParameters(statement, parameters);

    std::vector<Row> rows;
    int result = SQLITE_ROW;
    while ((result = sqlite3_step(statement)) == SQLITE_ROW) {
        Row row;
        int columnCount = sqlite3_column_count(statement);

        for (int i = 0; i < columnCount; ++i) {
            const char* columnName = sqlite3_column_name(statement, i);
            const unsigned char* text = sqlite3_column_text(statement, i);
            row[columnName] = text ? reinterpret_cast<const char*>(text) : "";
        }

        rows.push_back(row);
    }

    sqlite3_finalize(statement);

    if (result != SQLITE_DONE) {
        throw AppError(500, sqlite3_errmsg(db_));
    }

    return rows;
}

int Database::lastInsertId() const {
    return static_cast<int>(sqlite3_last_insert_rowid(db_));
}

bool Database::isEmpty(const std::string& tableName) {
    auto rows = query("SELECT COUNT(*) AS count FROM " + tableName + ";");
    return rows.empty() || rows.front().at("count") == "0";
}

void Database::bindParameters(sqlite3_stmt* statement, const std::vector<Parameter>& parameters) {
    for (std::size_t i = 0; i < parameters.size(); ++i) {
        int index = static_cast<int>(i + 1);

        if (!parameters[i].has_value()) {
            sqlite3_bind_null(statement, index);
            continue;
        }

        sqlite3_bind_text(statement, index, parameters[i]->c_str(), -1, SQLITE_TRANSIENT);
    }
}

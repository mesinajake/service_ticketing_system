#include "database/Database.h"
#include "services/EngineerService.h"
#include "utils/AppError.h"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>

namespace {

struct EngineerFixture {
    std::string dbPath = "engineer_service_test.db";
    Database database{dbPath};
    EngineerService engineerService;

    EngineerFixture()
        : engineerService(database) {
        std::filesystem::remove(dbPath);
        database.open();
        database.createTables();
    }

    ~EngineerFixture() {
        std::filesystem::remove(dbPath);
    }
};

}

TEST_CASE("createEngineer with valid data returns an engineer") {
    EngineerFixture fixture;
    Engineer engineer = fixture.engineerService.createEngineer(
        {"Ava Santos", "ava@example.com", "Junior Engineer", "IT Support"}
    );

    REQUIRE(engineer.id == 1);
    REQUIRE(engineer.fullName == "Ava Santos");
}

TEST_CASE("createEngineer with duplicate email throws an error") {
    EngineerFixture fixture;
    fixture.engineerService.createEngineer({"Ava Santos", "ava@example.com", "Junior Engineer", "IT Support"});

    REQUIRE_THROWS_AS(
        fixture.engineerService.createEngineer({"Ava Clone", "ava@example.com", "Senior Engineer", "IT Support"}),
        AppError
    );
}

TEST_CASE("getAllEngineers returns the correct ticket count per engineer") {
    EngineerFixture fixture;
    fixture.engineerService.createEngineer({"Ava Santos", "ava@example.com", "Junior Engineer", "IT Support"});

    auto engineers = fixture.engineerService.getAllEngineers();
    REQUIRE(engineers.size() == 1);
    REQUIRE(engineers.front().activeTicketCount.has_value());
    REQUIRE(*engineers.front().activeTicketCount == 0);
}

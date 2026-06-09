#include "database/Database.h"
#include "services/EngineerService.h"
#include "services/LogService.h"
#include "services/TicketService.h"
#include "utils/AppError.h"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>

namespace {

struct TicketFixture {
    std::string dbPath = "ticket_service_test.db";
    Database database{dbPath};
    LogService logService;
    EngineerService engineerService;
    TicketService ticketService;

    TicketFixture()
        : logService(database), engineerService(database), ticketService(database, engineerService, logService) {
        std::filesystem::remove(dbPath);
        database.open();
        database.createTables();
        engineerService.createEngineer({"Test Engineer", "engineer@example.com", "Junior Engineer", "IT Support"});
    }

    ~TicketFixture() {
        std::filesystem::remove(dbPath);
    }
};

TicketInput validTicket() {
    return {
        "Cannot log in",
        "Requester cannot log in to the portal.",
        "Test Requester",
        "requester@example.com",
        "Account Access",
        "High",
        "Open",
        1,
        std::nullopt
    };
}

}

TEST_CASE("createTicket with valid data returns a ticket with correct fields") {
    TicketFixture fixture;
    Ticket ticket = fixture.ticketService.createTicket(validTicket());

    REQUIRE(ticket.id == 1);
    REQUIRE(ticket.title == "Cannot log in");
    REQUIRE(ticket.priority == "High");
}

TEST_CASE("createTicket with missing title throws a validation error") {
    TicketFixture fixture;
    TicketInput input = validTicket();
    input.title = "";

    REQUIRE_THROWS_AS(fixture.ticketService.createTicket(input), AppError);
}

TEST_CASE("getTicketById with a non-existent ID throws a not-found error") {
    TicketFixture fixture;
    REQUIRE_THROWS_AS(fixture.ticketService.getTicketById(99), AppError);
}

TEST_CASE("updateStatus with an invalid status value throws a validation error") {
    TicketFixture fixture;
    Ticket ticket = fixture.ticketService.createTicket(validTicket());

    REQUIRE_THROWS_AS(fixture.ticketService.updateStatus(ticket.id, "Waiting"), AppError);
}

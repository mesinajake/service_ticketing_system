#include "config/Config.h"
#include "controllers/EngineerController.h"
#include "controllers/LogController.h"
#include "controllers/ReportController.h"
#include "controllers/TicketController.h"
#include "database/Database.h"
#include "services/EngineerService.h"
#include "services/LogService.h"
#include "services/ReportService.h"
#include "services/TicketService.h"
#include "utils/JsonHelper.h"

#include <crow.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {

void seedDatabaseIfEmpty(Database& database) {
    if (!database.isEmpty("engineers")) {
        return;
    }

    std::ifstream seedFile("data/seed.sql");
    if (!seedFile) {
        std::cerr << "Seed file not found. Starting with an empty database.\n";
        return;
    }

    std::stringstream buffer;
    buffer << seedFile.rdbuf();
    database.execute(buffer.str());
}

}

int main() {
    try {
        std::filesystem::create_directories("data");

        Database database(Config::DB_PATH);
        database.open();
        database.createTables();
        seedDatabaseIfEmpty(database);

        LogService logService(database);
        EngineerService engineerService(database);
        TicketService ticketService(database, engineerService, logService);
        ReportService reportService(database);

        crow::SimpleApp app;

        CROW_ROUTE(app, "/api/health").methods(crow::HTTPMethod::GET)([]() {
            return JsonHelper::jsonResponse(200, {{"status", "ok"}});
        });

        TicketController::registerRoutes(app, ticketService);
        EngineerController::registerRoutes(app, engineerService);
        LogController::registerRoutes(app, logService);
        ReportController::registerRoutes(app, reportService);

        const char* portEnv = std::getenv("APP_PORT");
        int port = portEnv ? std::atoi(portEnv) : Config::PORT;

        std::cout << Config::APP_NAME << " running on port " << port << '\n';
        app.port(port).multithreaded().run();
    } catch (const std::exception& error) {
        std::cerr << "Server failed to start: " << error.what() << '\n';
        return 1;
    }

    return 0;
}

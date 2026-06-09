#include "controllers/LogController.h"

#include "controllers/ControllerUtils.h"
#include "services/LogService.h"

namespace {

nlohmann::json logArray(const std::vector<ActivityLog>& logs) {
    nlohmann::json data = nlohmann::json::array();
    for (const auto& log : logs) {
        data.push_back(log.toJson());
    }
    return data;
}

}

void LogController::registerRoutes(crow::SimpleApp& app, LogService& logService) {
    CROW_ROUTE(app, "/api/logs").methods(crow::HTTPMethod::GET)(
        [&logService]() {
            try {
                return JsonHelper::jsonResponse(200, JsonHelper::successResponse(logArray(logService.getAllLogs())));
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/tickets/<int>/logs").methods(crow::HTTPMethod::GET)(
        [&logService](int ticketId) {
            try {
                return JsonHelper::jsonResponse(
                    200,
                    JsonHelper::successResponse(logArray(logService.getLogsByTicketId(ticketId)))
                );
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );
}

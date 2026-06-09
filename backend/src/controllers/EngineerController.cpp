#include "controllers/EngineerController.h"

#include "controllers/ControllerUtils.h"
#include "services/EngineerService.h"

namespace {

EngineerInput parseEngineerInput(const crow::request& request) {
    auto body = ControllerUtils::parseJsonBody(request);
    return {
        body.value("full_name", ""),
        body.value("email", ""),
        body.value("role", ""),
        body.value("department", "")
    };
}

nlohmann::json engineerArray(const std::vector<Engineer>& engineers) {
    nlohmann::json data = nlohmann::json::array();
    for (const auto& engineer : engineers) {
        data.push_back(engineer.toJson());
    }
    return data;
}

}

void EngineerController::registerRoutes(crow::SimpleApp& app, EngineerService& engineerService) {
    CROW_ROUTE(app, "/api/engineers").methods(crow::HTTPMethod::GET)(
        [&engineerService]() {
            try {
                return JsonHelper::jsonResponse(
                    200,
                    JsonHelper::successResponse(engineerArray(engineerService.getAllEngineers()))
                );
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/engineers/<int>").methods(crow::HTTPMethod::GET)(
        [&engineerService](int id) {
            try {
                return JsonHelper::jsonResponse(
                    200,
                    JsonHelper::successResponse(engineerService.getEngineerById(id).toJson())
                );
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/engineers").methods(crow::HTTPMethod::POST)(
        [&engineerService](const crow::request& request) {
            try {
                return JsonHelper::jsonResponse(
                    201,
                    JsonHelper::successResponse(engineerService.createEngineer(parseEngineerInput(request)).toJson())
                );
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );
}

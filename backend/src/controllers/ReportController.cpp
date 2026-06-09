#include "controllers/ReportController.h"

#include "controllers/ControllerUtils.h"
#include "services/ReportService.h"

void ReportController::registerRoutes(crow::SimpleApp& app, ReportService& reportService) {
    CROW_ROUTE(app, "/api/reports/summary").methods(crow::HTTPMethod::GET)(
        [&reportService]() {
            try {
                return JsonHelper::jsonResponse(200, JsonHelper::successResponse(reportService.getSummary().toJson()));
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/reports/status").methods(crow::HTTPMethod::GET)(
        [&reportService]() {
            try {
                return JsonHelper::jsonResponse(200, JsonHelper::successResponse(reportService.getByStatus()));
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/reports/priority").methods(crow::HTTPMethod::GET)(
        [&reportService]() {
            try {
                return JsonHelper::jsonResponse(200, JsonHelper::successResponse(reportService.getByPriority()));
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/reports/category").methods(crow::HTTPMethod::GET)(
        [&reportService]() {
            try {
                return JsonHelper::jsonResponse(200, JsonHelper::successResponse(reportService.getByCategory()));
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/reports/engineers").methods(crow::HTTPMethod::GET)(
        [&reportService]() {
            try {
                return JsonHelper::jsonResponse(200, JsonHelper::successResponse(reportService.getByEngineer()));
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );
}

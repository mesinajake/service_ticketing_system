#include "controllers/TicketController.h"

#include "controllers/ControllerUtils.h"
#include "services/TicketService.h"

namespace {

std::optional<int> optionalEngineerId(const nlohmann::json& body) {
    if (!body.contains("assigned_engineer_id") || body["assigned_engineer_id"].is_null()) {
        return std::nullopt;
    }
    return body["assigned_engineer_id"].get<int>();
}

std::optional<std::string> optionalResolutionNotes(const nlohmann::json& body) {
    if (!body.contains("resolution_notes") || body["resolution_notes"].is_null()) {
        return std::nullopt;
    }
    return body["resolution_notes"].get<std::string>();
}

TicketInput parseTicketInput(const crow::request& request) {
    auto body = ControllerUtils::parseJsonBody(request);

    return {
        body.value("title", ""),
        body.value("description", ""),
        body.value("requester_name", ""),
        body.value("requester_email", ""),
        body.value("category", ""),
        body.value("priority", ""),
        body.value("status", "Open"),
        optionalEngineerId(body),
        optionalResolutionNotes(body)
    };
}

nlohmann::json ticketArray(const std::vector<Ticket>& tickets) {
    nlohmann::json data = nlohmann::json::array();
    for (const auto& ticket : tickets) {
        data.push_back(ticket.toJson());
    }
    return data;
}

}

void TicketController::registerRoutes(crow::SimpleApp& app, TicketService& ticketService) {
    CROW_ROUTE(app, "/api/tickets").methods(crow::HTTPMethod::GET)(
        [&ticketService](const crow::request& request) {
            try {
                TicketFilters filters;
                filters.search = ControllerUtils::queryParam(request, "search");
                filters.status = ControllerUtils::queryParam(request, "status");
                filters.priority = ControllerUtils::queryParam(request, "priority");
                filters.category = ControllerUtils::queryParam(request, "category");
                filters.dateFrom = ControllerUtils::queryParam(request, "date_from");
                filters.dateTo = ControllerUtils::queryParam(request, "date_to");

                std::string limit = ControllerUtils::queryParam(request, "limit");
                if (!limit.empty()) {
                    filters.limit = std::stoi(limit);
                }

                return JsonHelper::jsonResponse(
                    200,
                    JsonHelper::successResponse(ticketArray(ticketService.getAllTickets(filters)))
                );
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/tickets/<int>").methods(crow::HTTPMethod::GET)(
        [&ticketService](int id) {
            try {
                return JsonHelper::jsonResponse(
                    200,
                    JsonHelper::successResponse(ticketService.getTicketById(id).toJson())
                );
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/tickets").methods(crow::HTTPMethod::POST)(
        [&ticketService](const crow::request& request) {
            try {
                return JsonHelper::jsonResponse(
                    201,
                    JsonHelper::successResponse(ticketService.createTicket(parseTicketInput(request)).toJson())
                );
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/tickets/<int>").methods(crow::HTTPMethod::PUT)(
        [&ticketService](const crow::request& request, int id) {
            try {
                return JsonHelper::jsonResponse(
                    200,
                    JsonHelper::successResponse(ticketService.updateTicket(id, parseTicketInput(request)).toJson())
                );
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/tickets/<int>").methods(crow::HTTPMethod::DELETE)(
        [&ticketService](int id) {
            try {
                ticketService.deleteTicket(id);
                return JsonHelper::noContent();
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/tickets/<int>/status").methods(crow::HTTPMethod::PATCH)(
        [&ticketService](const crow::request& request, int id) {
            try {
                auto body = ControllerUtils::parseJsonBody(request);
                return JsonHelper::jsonResponse(
                    200,
                    JsonHelper::successResponse(ticketService.updateStatus(id, body.value("status", "")).toJson())
                );
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );

    CROW_ROUTE(app, "/api/tickets/<int>/assign").methods(crow::HTTPMethod::PATCH)(
        [&ticketService](const crow::request& request, int id) {
            try {
                auto body = ControllerUtils::parseJsonBody(request);
                return JsonHelper::jsonResponse(
                    200,
                    JsonHelper::successResponse(ticketService.assignEngineer(id, body.value("engineer_id", 0)).toJson())
                );
            } catch (const std::exception& error) {
                return ControllerUtils::handleError(error);
            }
        }
    );
}

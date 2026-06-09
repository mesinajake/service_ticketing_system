#pragma once

#include "utils/AppError.h"
#include "utils/JsonHelper.h"

#include <crow.h>
#include <nlohmann/json.hpp>

#include <exception>

namespace ControllerUtils {

inline crow::response handleError(const std::exception& error) {
    const auto* appError = dynamic_cast<const AppError*>(&error);
    if (appError) {
        return JsonHelper::jsonResponse(appError->statusCode(), JsonHelper::errorResponse(appError->what()));
    }

    return JsonHelper::jsonResponse(500, JsonHelper::errorResponse("Internal server error."));
}

inline nlohmann::json parseJsonBody(const crow::request& request) {
    try {
        return nlohmann::json::parse(request.body);
    } catch (...) {
        throw AppError(400, "Invalid JSON body.");
    }
}

inline std::string queryParam(const crow::request& request, const char* key) {
    const char* value = request.url_params.get(key);
    return value ? std::string(value) : "";
}

}

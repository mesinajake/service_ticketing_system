#pragma once

#include <crow.h>
#include <nlohmann/json.hpp>

#include <string>

namespace JsonHelper {
    nlohmann::json successResponse(const nlohmann::json& data);
    nlohmann::json errorResponse(const std::string& message);
    crow::response jsonResponse(int statusCode, const nlohmann::json& body);
    crow::response noContent();
}

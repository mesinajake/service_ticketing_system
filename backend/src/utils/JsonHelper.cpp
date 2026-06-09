#include "utils/JsonHelper.h"

namespace JsonHelper {

nlohmann::json successResponse(const nlohmann::json& data) {
    return {
        {"success", true},
        {"data", data}
    };
}

nlohmann::json errorResponse(const std::string& message) {
    return {
        {"success", false},
        {"error", message}
    };
}

crow::response jsonResponse(int statusCode, const nlohmann::json& body) {
    crow::response response(statusCode, body.dump());
    response.set_header("Content-Type", "application/json");
    response.set_header("Access-Control-Allow-Origin", "http://localhost:5173");
    response.set_header("Access-Control-Allow-Headers", "Content-Type");
    response.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, PATCH, DELETE, OPTIONS");
    return response;
}

crow::response noContent() {
    crow::response response(204);
    response.set_header("Access-Control-Allow-Origin", "http://localhost:5173");
    response.set_header("Access-Control-Allow-Headers", "Content-Type");
    response.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, PATCH, DELETE, OPTIONS");
    return response;
}

}

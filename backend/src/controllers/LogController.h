#pragma once

#include <crow.h>

class LogService;

class LogController {
public:
    static void registerRoutes(crow::SimpleApp& app, LogService& logService);
};

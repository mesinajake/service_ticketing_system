#pragma once

#include <crow.h>

class EngineerService;

class EngineerController {
public:
    static void registerRoutes(crow::SimpleApp& app, EngineerService& engineerService);
};

#pragma once

#include <crow.h>

class ReportService;

class ReportController {
public:
    static void registerRoutes(crow::SimpleApp& app, ReportService& reportService);
};

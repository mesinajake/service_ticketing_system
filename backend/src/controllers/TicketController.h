#pragma once

#include <crow.h>

class TicketService;

class TicketController {
public:
    static void registerRoutes(crow::SimpleApp& app, TicketService& ticketService);
};

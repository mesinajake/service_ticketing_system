#pragma once

#include <nlohmann/json.hpp>

struct ReportSummary {
    int total = 0;
    int open = 0;
    int inProgress = 0;
    int resolved = 0;
    int closed = 0;
    int critical = 0;

    nlohmann::json toJson() const {
        return {
            {"total", total},
            {"open", open},
            {"in_progress", inProgress},
            {"resolved", resolved},
            {"closed", closed},
            {"critical", critical}
        };
    }
};

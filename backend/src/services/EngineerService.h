#pragma once

#include "database/Database.h"
#include "models/Engineer.h"

#include <string>
#include <vector>

struct EngineerInput {
    std::string fullName;
    std::string email;
    std::string role;
    std::string department;
};

class EngineerService {
public:
    explicit EngineerService(Database& database);

    std::vector<Engineer> getAllEngineers();
    Engineer getEngineerById(int id);
    Engineer createEngineer(const EngineerInput& input);
    bool engineerExists(int id);

private:
    void validateEngineer(const EngineerInput& input);

    Database& database_;
};

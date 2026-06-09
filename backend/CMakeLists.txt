cmake_minimum_required(VERSION 3.15)

project(ServiceTicketSystemBackend LANGUAGES CXX)

option(BUILD_TESTS "Build service layer tests" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.2.1.2
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(Crow nlohmann_json)

find_package(SQLite3 REQUIRED)

set(BACKEND_SOURCES
    src/controllers/EngineerController.cpp
    src/controllers/LogController.cpp
    src/controllers/ReportController.cpp
    src/controllers/TicketController.cpp
    src/database/Database.cpp
    src/models/ActivityLog.cpp
    src/models/Engineer.cpp
    src/models/Ticket.cpp
    src/services/EngineerService.cpp
    src/services/LogService.cpp
    src/services/ReportService.cpp
    src/services/TicketService.cpp
    src/utils/JsonHelper.cpp
    src/utils/Validator.cpp
)

add_library(service_ticket_core ${BACKEND_SOURCES})
target_include_directories(service_ticket_core PUBLIC src)
target_link_libraries(service_ticket_core PUBLIC Crow::Crow nlohmann_json::nlohmann_json SQLite::SQLite3)

add_executable(service_ticket_backend src/main.cpp)
target_link_libraries(service_ticket_backend PRIVATE service_ticket_core)

if(BUILD_TESTS)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.7.1
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(run_tests
        tests/test_engineer_service.cpp
        tests/test_ticket_service.cpp
        tests/test_validator.cpp
    )
    target_link_libraries(run_tests PRIVATE service_ticket_core Catch2::Catch2WithMain)
endif()

#include "utils/Validator.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("isValidEmail returns false for invalid formats") {
    REQUIRE_FALSE(Validator::isValidEmail("not-an-email"));
    REQUIRE_FALSE(Validator::isValidEmail("missing-domain@"));
    REQUIRE(Validator::isValidEmail("user@example.com"));
}

TEST_CASE("isValidPriority returns false for undefined priority values") {
    REQUIRE_FALSE(Validator::isValidPriority("Urgent"));
    REQUIRE(Validator::isValidPriority("Critical"));
}

TEST_CASE("isValidStatus returns false for undefined status values") {
    REQUIRE_FALSE(Validator::isValidStatus("Waiting"));
    REQUIRE(Validator::isValidStatus("In Progress"));
}

#include "ContestsController.h"

crow::response ContestsController::getContests(const crow::request& req) {
    return crow::response(200, "[]"); // TODO: Implement contests repository
}

crow::response ContestsController::createContest(const crow::request& req) {
    return crow::response(501, JsonSerializer::errorResponse("Contests not implemented yet"));
}

#include "CodeforcesAccountsController.h"

crow::response CodeforcesAccountsController::getAccounts(const crow::request& req) {
    return crow::response(200, "[]"); // TODO: Implement codeforces accounts repository
}

crow::response CodeforcesAccountsController::createAccount(const crow::request& req) {
    return crow::response(501, JsonSerializer::errorResponse("Codeforces accounts not implemented yet"));
}

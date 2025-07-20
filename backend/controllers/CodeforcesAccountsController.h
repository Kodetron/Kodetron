#ifndef CODEFORCESACCOUNTSCONTROLLER_H
#define CODEFORCESACCOUNTSCONTROLLER_H

#include <crow.h>
#include "utils/JsonSerializer.h"

class CodeforcesAccountsController {
public:
    crow::response getAccounts(const crow::request& req);
    crow::response createAccount(const crow::request& req);
};

#endif // CODEFORCESACCOUNTSCONTROLLER_H

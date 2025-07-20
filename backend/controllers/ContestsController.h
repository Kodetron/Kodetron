#ifndef CONTESTSCONTROLLER_H
#define CONTESTSCONTROLLER_H

#include <crow.h>
#include "utils/JsonSerializer.h"

class ContestsController {
public:
    crow::response getContests(const crow::request& req);
    crow::response createContest(const crow::request& req);
};

#endif // CONTESTSCONTROLLER_H

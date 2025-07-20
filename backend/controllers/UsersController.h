#ifndef USERSCONTROLLER_H
#define USERSCONTROLLER_H

#include <crow.h>
#include "DataBase/UserRepository.h"
#include "utils/JsonSerializer.h"

class UsersController {
private:
    UserRepository userRepository;
    
public:
    UsersController() = default;
    
    crow::response getAllUsers(const crow::request& req);
    crow::response createUser(const crow::request& req);
    crow::response getUserById(const crow::request& req, int userId);
    crow::response updateUser(const crow::request& req, int userId);
    crow::response deleteUser(const crow::request& req, int userId);
};

#endif // USERSCONTROLLER_H

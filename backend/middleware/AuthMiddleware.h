#ifndef AUTHMIDDLEWARE_H
#define AUTHMIDDLEWARE_H

#include <crow.h>
#include <string>

struct AuthMiddleware {
    struct context {
        int userId = -1;
        bool authenticated = false;
        std::string username;
    };
    
    void before_handle(crow::request& req, crow::response& res, context& ctx);
    void after_handle(crow::request& req, crow::response& res, context& ctx);
    
private:
    bool validateToken(const std::string& token, context& ctx);
    bool isPublicEndpoint(const std::string& url);
};

#endif // AUTHMIDDLEWARE_H

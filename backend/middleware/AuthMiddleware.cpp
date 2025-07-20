#include "AuthMiddleware.h"
#include <iostream>

void AuthMiddleware::before_handle(crow::request& req, crow::response& res, context& ctx) {
    // Add CORS headers
    res.add_header("Access-Control-Allow-Origin", "*");
    res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    
    // Skip authentication for public endpoints
    if (isPublicEndpoint(req.url)) {
        ctx.authenticated = true;
        ctx.userId = 1; // Default user for demo purposes
        return;
    }
    
    // Handle OPTIONS requests (CORS preflight)
    if (req.method == crow::HTTPMethod::Options) {
        ctx.authenticated = true;
        return;
    }
    
    // Get Authorization header
    std::string authHeader = req.get_header_value("Authorization");
    
    if (authHeader.empty()) {
        // For demo purposes, allow unauthenticated access with default user
        ctx.authenticated = true;
        ctx.userId = 1;
        ctx.username = "default_user";
        return;
    }
    
    // Validate token
    if (validateToken(authHeader, ctx)) {
        ctx.authenticated = true;
    } else {
        res.code = 401;
        res.write("Unauthorized");
        res.end();
        return;
    }
}

void AuthMiddleware::after_handle(crow::request& req, crow::response& res, context& ctx) {
    // Add common response headers
    if (res.headers.find("Content-Type") == res.headers.end()) {
        res.add_header("Content-Type", "application/json");
    }
}

bool AuthMiddleware::validateToken(const std::string& token, context& ctx) {
    // Simple token validation for demo
    // In production, you would validate JWT tokens here
    
    if (token.find("Bearer ") == 0) {
        std::string actualToken = token.substr(7);
        
        // Demo: accept "demo_token" as valid
        if (actualToken == "demo_token") {
            ctx.userId = 1;
            ctx.username = "demo_user";
            return true;
        }
    }
    
    return false;
}

bool AuthMiddleware::isPublicEndpoint(const std::string& url) {
    // List of public endpoints that don't require authentication
    std::vector<std::string> publicEndpoints = {
        "/health",
        "/api",
        "/api/users" // For demo, allow user creation without auth
    };
    
    for (const auto& endpoint : publicEndpoints) {
        if (url.find(endpoint) == 0) {
            return true;
        }
    }
    
    return false;
}

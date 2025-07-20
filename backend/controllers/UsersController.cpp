#include "UsersController.h"
#include <vector>

crow::response UsersController::getAllUsers(const crow::request& req) {
    try {
        auto usersList = userRepository.findAll();
        std::vector<User> users(usersList.begin(), usersList.end());
        
        std::string json = JsonSerializer::usersToJson(users);
        return crow::response(200, json);
    } catch (const std::exception& e) {
        return crow::response(500, JsonSerializer::errorResponse("Failed to get users: " + std::string(e.what())));
    }
}

crow::response UsersController::createUser(const crow::request& req) {
    try {
        User user = JsonSerializer::jsonToUser(req.body);
        
        if (user.username.isEmpty()) {
            return crow::response(400, JsonSerializer::errorResponse("Username is required"));
        }
        
        if (userRepository.existsByUsername(user.username)) {
            return crow::response(409, JsonSerializer::errorResponse("Username already exists"));
        }
        
        if (userRepository.create(user)) {
            return crow::response(201, JsonSerializer::userToJson(user));
        } else {
            return crow::response(500, JsonSerializer::errorResponse("Failed to create user"));
        }
    } catch (const std::exception& e) {
        return crow::response(400, JsonSerializer::errorResponse("Invalid JSON: " + std::string(e.what())));
    }
}

crow::response UsersController::getUserById(const crow::request& req, int userId) {
    try {
        User user = userRepository.findById(userId);
        
        if (!user.isValid()) {
            return crow::response(404, JsonSerializer::errorResponse("User not found"));
        }
        
        return crow::response(200, JsonSerializer::userToJson(user));
    } catch (const std::exception& e) {
        return crow::response(500, JsonSerializer::errorResponse("Failed to get user: " + std::string(e.what())));
    }
}

crow::response UsersController::updateUser(const crow::request& req, int userId) {
    try {
        User existingUser = userRepository.findById(userId);
        
        if (!existingUser.isValid()) {
            return crow::response(404, JsonSerializer::errorResponse("User not found"));
        }
        
        User updatedUser = JsonSerializer::jsonToUser(req.body);
        updatedUser.id = userId;
        updatedUser.createdAt = existingUser.createdAt; // Preserve original creation time
        
        if (userRepository.update(updatedUser)) {
            return crow::response(200, JsonSerializer::userToJson(updatedUser));
        } else {
            return crow::response(500, JsonSerializer::errorResponse("Failed to update user"));
        }
    } catch (const std::exception& e) {
        return crow::response(400, JsonSerializer::errorResponse("Invalid JSON: " + std::string(e.what())));
    }
}

crow::response UsersController::deleteUser(const crow::request& req, int userId) {
    try {
        if (!userRepository.exists(userId)) {
            return crow::response(404, JsonSerializer::errorResponse("User not found"));
        }
        
        if (userRepository.deleteById(userId)) {
            return crow::response(200, JsonSerializer::successResponse("User deleted successfully"));
        } else {
            return crow::response(500, JsonSerializer::errorResponse("Failed to delete user"));
        }
    } catch (const std::exception& e) {
        return crow::response(500, JsonSerializer::errorResponse("Failed to delete user: " + std::string(e.what())));
    }
}

#include "ThemesController.h"
#include <vector>

crow::response ThemesController::getThemes(const crow::request& req) {
    try {
        // Get user_id from query parameters
        char* userIdParam = req.url_params.get("user_id");
        if (userIdParam) {
            int userId = std::stoi(userIdParam);
            auto themesList = themeRepository.findByUserId(userId);
            std::vector<Theme> themes(themesList.begin(), themesList.end());
            return crow::response(200, JsonSerializer::themesToJson(themes));
        } else {
            // Return all themes if no user_id specified
            auto themesList = themeRepository.findAll();
            std::vector<Theme> themes(themesList.begin(), themesList.end());
            return crow::response(200, JsonSerializer::themesToJson(themes));
        }
    } catch (const std::exception& e) {
        return crow::response(500, JsonSerializer::errorResponse("Failed to get themes: " + std::string(e.what())));
    }
}

crow::response ThemesController::createTheme(const crow::request& req) {
    try {
        Theme theme = JsonSerializer::jsonToTheme(req.body);
        
        if (theme.name.isEmpty()) {
            return crow::response(400, JsonSerializer::errorResponse("Theme name is required"));
        }
        
        if (theme.userId <= 0) {
            return crow::response(400, JsonSerializer::errorResponse("Valid user_id is required"));
        }
        
        // Check if theme with same name exists for this user
        Theme existingTheme = themeRepository.findByUserIdAndName(theme.userId, theme.name);
        if (existingTheme.isValid()) {
            return crow::response(409, JsonSerializer::errorResponse("Theme with this name already exists for this user"));
        }
        
        if (themeRepository.create(theme)) {
            return crow::response(201, JsonSerializer::themeToJson(theme));
        } else {
            return crow::response(500, JsonSerializer::errorResponse("Failed to create theme"));
        }
    } catch (const std::exception& e) {
        return crow::response(400, JsonSerializer::errorResponse("Invalid JSON: " + std::string(e.what())));
    }
}

crow::response ThemesController::getThemeById(const crow::request& req, int themeId) {
    try {
        Theme theme = themeRepository.findById(themeId);
        
        if (!theme.isValid()) {
            return crow::response(404, JsonSerializer::errorResponse("Theme not found"));
        }
        
        return crow::response(200, JsonSerializer::themeToJson(theme));
    } catch (const std::exception& e) {
        return crow::response(500, JsonSerializer::errorResponse("Failed to get theme: " + std::string(e.what())));
    }
}

crow::response ThemesController::updateTheme(const crow::request& req, int themeId) {
    try {
        Theme existingTheme = themeRepository.findById(themeId);
        
        if (!existingTheme.isValid()) {
            return crow::response(404, JsonSerializer::errorResponse("Theme not found"));
        }
        
        Theme updatedTheme = JsonSerializer::jsonToTheme(req.body);
        updatedTheme.id = themeId;
        updatedTheme.userId = existingTheme.userId; // Preserve user ID
        updatedTheme.createdAt = existingTheme.createdAt; // Preserve creation time
        
        if (themeRepository.update(updatedTheme)) {
            return crow::response(200, JsonSerializer::themeToJson(updatedTheme));
        } else {
            return crow::response(500, JsonSerializer::errorResponse("Failed to update theme"));
        }
    } catch (const std::exception& e) {
        return crow::response(400, JsonSerializer::errorResponse("Invalid JSON: " + std::string(e.what())));
    }
}

crow::response ThemesController::deleteTheme(const crow::request& req, int themeId) {
    try {
        if (!themeRepository.exists(themeId)) {
            return crow::response(404, JsonSerializer::errorResponse("Theme not found"));
        }
        
        if (themeRepository.deleteById(themeId)) {
            return crow::response(200, JsonSerializer::successResponse("Theme deleted successfully"));
        } else {
            return crow::response(500, JsonSerializer::errorResponse("Failed to delete theme"));
        }
    } catch (const std::exception& e) {
        return crow::response(500, JsonSerializer::errorResponse("Failed to delete theme: " + std::string(e.what())));
    }
}

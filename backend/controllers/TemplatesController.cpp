#include "TemplatesController.h"

crow::response TemplatesController::getTemplates(const crow::request& req) {
    return crow::response(200, "[]"); // TODO: Implement templates repository
}

crow::response TemplatesController::createTemplate(const crow::request& req) {
    return crow::response(501, JsonSerializer::errorResponse("Templates not implemented yet"));
}

crow::response TemplatesController::getTemplateById(const crow::request& req, int templateId) {
    return crow::response(501, JsonSerializer::errorResponse("Templates not implemented yet"));
}

crow::response TemplatesController::updateTemplate(const crow::request& req, int templateId) {
    return crow::response(501, JsonSerializer::errorResponse("Templates not implemented yet"));
}

crow::response TemplatesController::deleteTemplate(const crow::request& req, int templateId) {
    return crow::response(501, JsonSerializer::errorResponse("Templates not implemented yet"));
}

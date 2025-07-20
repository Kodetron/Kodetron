#include "SnippetsController.h"

crow::response SnippetsController::getSnippets(const crow::request& req) {
    return crow::response(200, "[]"); // TODO: Implement snippets repository
}

crow::response SnippetsController::createSnippet(const crow::request& req) {
    return crow::response(501, JsonSerializer::errorResponse("Snippets not implemented yet"));
}

crow::response SnippetsController::getSnippetById(const crow::request& req, int snippetId) {
    return crow::response(501, JsonSerializer::errorResponse("Snippets not implemented yet"));
}

crow::response SnippetsController::updateSnippet(const crow::request& req, int snippetId) {
    return crow::response(501, JsonSerializer::errorResponse("Snippets not implemented yet"));
}

crow::response SnippetsController::deleteSnippet(const crow::request& req, int snippetId) {
    return crow::response(501, JsonSerializer::errorResponse("Snippets not implemented yet"));
}

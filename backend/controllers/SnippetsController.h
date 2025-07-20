#ifndef SNIPPETSCONTROLLER_H
#define SNIPPETSCONTROLLER_H

#include <crow.h>
#include "utils/JsonSerializer.h"

class SnippetsController {
public:
    crow::response getSnippets(const crow::request& req);
    crow::response createSnippet(const crow::request& req);
    crow::response getSnippetById(const crow::request& req, int snippetId);
    crow::response updateSnippet(const crow::request& req, int snippetId);
    crow::response deleteSnippet(const crow::request& req, int snippetId);
};

#endif // SNIPPETSCONTROLLER_H

#ifndef TEMPLATESCONTROLLER_H
#define TEMPLATESCONTROLLER_H

#include <crow.h>
#include "utils/JsonSerializer.h"

class TemplatesController {
public:
    crow::response getTemplates(const crow::request& req);
    crow::response createTemplate(const crow::request& req);
    crow::response getTemplateById(const crow::request& req, int templateId);
    crow::response updateTemplate(const crow::request& req, int templateId);
    crow::response deleteTemplate(const crow::request& req, int templateId);
};

#endif // TEMPLATESCONTROLLER_H

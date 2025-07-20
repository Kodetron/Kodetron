#ifndef THEMESCONTROLLER_H
#define THEMESCONTROLLER_H

#include <crow.h>
#include "DataBase/ThemeRepository.h"
#include "utils/JsonSerializer.h"

class ThemesController {
private:
    ThemeRepository themeRepository;
    
public:
    ThemesController() = default;
    
    crow::response getThemes(const crow::request& req);
    crow::response createTheme(const crow::request& req);
    crow::response getThemeById(const crow::request& req, int themeId);
    crow::response updateTheme(const crow::request& req, int themeId);
    crow::response deleteTheme(const crow::request& req, int themeId);
};

#endif // THEMESCONTROLLER_H

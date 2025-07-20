#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <string>
#include <vector>
#include "DataBase/Models.h"

class JsonSerializer {
public:
    // User serialization
    static std::string userToJson(const User& user);
    static std::string usersToJson(const std::vector<User>& users);
    static User jsonToUser(const std::string& json);
    
    // Theme serialization
    static std::string themeToJson(const Theme& theme);
    static std::string themesToJson(const std::vector<Theme>& themes);
    static Theme jsonToTheme(const std::string& json);
    
    // Snippet serialization
    static std::string snippetToJson(const Snippet& snippet);
    static std::string snippetsToJson(const std::vector<Snippet>& snippets);
    static Snippet jsonToSnippet(const std::string& json);
    
    // Template serialization
    static std::string templateToJson(const Template& tmpl);
    static std::string templatesToJson(const std::vector<Template>& templates);
    static Template jsonToTemplate(const std::string& json);
    
    // Contest serialization
    static std::string contestToJson(const Contest& contest);
    static std::string contestsToJson(const std::vector<Contest>& contests);
    static Contest jsonToContest(const std::string& json);
    
    // CodeforcesAccount serialization
    static std::string codeforcesAccountToJson(const CodeforcesAccount& account);
    static std::string codeforcesAccountsToJson(const std::vector<CodeforcesAccount>& accounts);
    static CodeforcesAccount jsonToCodeforcesAccount(const std::string& json);
    
    // Error responses
    static std::string errorResponse(const std::string& message, int code = 400);
    static std::string successResponse(const std::string& message);
    
private:
    static std::string escapeJson(const std::string& str);
    static std::string unescapeJson(const std::string& str);
    static std::string extractValue(const std::string& json, const std::string& key);
    static int extractIntValue(const std::string& json, const std::string& key);
};

#endif // JSONSERIALIZER_H

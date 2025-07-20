#include "JsonSerializer.h"
#include <sstream>
#include <algorithm>
#include <QDateTime>

// User serialization
std::string JsonSerializer::userToJson(const User& user) {
    std::ostringstream json;
    json << "{"
         << "\"id\":" << user.id << ","
         << "\"username\":\"" << escapeJson(user.username.toStdString()) << "\","
         << "\"email\":\"" << escapeJson(user.email.toStdString()) << "\","
         << "\"created_at\":\"" << escapeJson(user.createdAt.toString(Qt::ISODate).toStdString()) << "\""
         << "}";
    return json.str();
}

std::string JsonSerializer::usersToJson(const std::vector<User>& users) {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < users.size(); ++i) {
        json << userToJson(users[i]);
        if (i < users.size() - 1) json << ",";
    }
    json << "]";
    return json.str();
}

User JsonSerializer::jsonToUser(const std::string& json) {
    User user;
    user.username = QString::fromStdString(extractValue(json, "username"));
    user.email = QString::fromStdString(extractValue(json, "email"));
    user.createdAt = QDateTime::currentDateTime();
    return user;
}

// Theme serialization
std::string JsonSerializer::themeToJson(const Theme& theme) {
    std::ostringstream json;
    json << "{"
         << "\"id\":" << theme.id << ","
         << "\"user_id\":" << theme.userId << ","
         << "\"name\":\"" << escapeJson(theme.name.toStdString()) << "\","
         << "\"qss_content\":\"" << escapeJson(theme.qssContent.toStdString()) << "\","
         << "\"created_at\":\"" << escapeJson(theme.createdAt.toString(Qt::ISODate).toStdString()) << "\","
         << "\"updated_at\":\"" << escapeJson(theme.updatedAt.toString(Qt::ISODate).toStdString()) << "\""
         << "}";
    return json.str();
}

std::string JsonSerializer::themesToJson(const std::vector<Theme>& themes) {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < themes.size(); ++i) {
        json << themeToJson(themes[i]);
        if (i < themes.size() - 1) json << ",";
    }
    json << "]";
    return json.str();
}

Theme JsonSerializer::jsonToTheme(const std::string& json) {
    Theme theme;
    theme.userId = extractIntValue(json, "user_id");
    theme.name = QString::fromStdString(extractValue(json, "name"));
    theme.qssContent = QString::fromStdString(extractValue(json, "qss_content"));
    theme.createdAt = QDateTime::currentDateTime();
    theme.updatedAt = QDateTime::currentDateTime();
    return theme;
}

// Snippet serialization
std::string JsonSerializer::snippetToJson(const Snippet& snippet) {
    std::ostringstream json;
    json << "{"
         << "\"id\":" << snippet.id << ","
         << "\"user_id\":" << snippet.userId << ","
         << "\"name\":\"" << escapeJson(snippet.name.toStdString()) << "\","
         << "\"prefix\":\"" << escapeJson(snippet.prefix.toStdString()) << "\","
         << "\"body\":\"" << escapeJson(snippet.body.toStdString()) << "\","
         << "\"description\":\"" << escapeJson(snippet.description.toStdString()) << "\","
         << "\"created_at\":\"" << escapeJson(snippet.createdAt.toString(Qt::ISODate).toStdString()) << "\","
         << "\"updated_at\":\"" << escapeJson(snippet.updatedAt.toString(Qt::ISODate).toStdString()) << "\""
         << "}";
    return json.str();
}

std::string JsonSerializer::snippetsToJson(const std::vector<Snippet>& snippets) {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < snippets.size(); ++i) {
        json << snippetToJson(snippets[i]);
        if (i < snippets.size() - 1) json << ",";
    }
    json << "]";
    return json.str();
}

Snippet JsonSerializer::jsonToSnippet(const std::string& json) {
    Snippet snippet;
    snippet.userId = extractIntValue(json, "user_id");
    snippet.name = QString::fromStdString(extractValue(json, "name"));
    snippet.prefix = QString::fromStdString(extractValue(json, "prefix"));
    snippet.body = QString::fromStdString(extractValue(json, "body"));
    snippet.description = QString::fromStdString(extractValue(json, "description"));
    snippet.createdAt = QDateTime::currentDateTime();
    snippet.updatedAt = QDateTime::currentDateTime();
    return snippet;
}

// Template serialization
std::string JsonSerializer::templateToJson(const Template& tmpl) {
    std::ostringstream json;
    json << "{"
         << "\"id\":" << tmpl.id << ","
         << "\"user_id\":" << tmpl.userId << ","
         << "\"name\":\"" << escapeJson(tmpl.name.toStdString()) << "\","
         << "\"content\":\"" << escapeJson(tmpl.content.toStdString()) << "\","
         << "\"created_at\":\"" << escapeJson(tmpl.createdAt.toString(Qt::ISODate).toStdString()) << "\","
         << "\"updated_at\":\"" << escapeJson(tmpl.updatedAt.toString(Qt::ISODate).toStdString()) << "\""
         << "}";
    return json.str();
}

std::string JsonSerializer::templatesToJson(const std::vector<Template>& templates) {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < templates.size(); ++i) {
        json << templateToJson(templates[i]);
        if (i < templates.size() - 1) json << ",";
    }
    json << "]";
    return json.str();
}

Template JsonSerializer::jsonToTemplate(const std::string& json) {
    Template tmpl;
    tmpl.userId = extractIntValue(json, "user_id");
    tmpl.name = QString::fromStdString(extractValue(json, "name"));
    tmpl.content = QString::fromStdString(extractValue(json, "content"));
    tmpl.createdAt = QDateTime::currentDateTime();
    tmpl.updatedAt = QDateTime::currentDateTime();
    return tmpl;
}

// Contest serialization
std::string JsonSerializer::contestToJson(const Contest& contest) {
    std::ostringstream json;
    json << "{"
         << "\"id\":" << contest.id << ","
         << "\"user_id\":" << contest.userId << ","
         << "\"codeforces_account_id\":" << contest.codeforcesAccountId << ","
         << "\"name\":\"" << escapeJson(contest.name.toStdString()) << "\","
         << "\"start_time\":\"" << escapeJson(contest.startTime.toString(Qt::ISODate).toStdString()) << "\","
         << "\"end_time\":\"" << escapeJson(contest.endTime.toString(Qt::ISODate).toStdString()) << "\","
         << "\"created_at\":\"" << escapeJson(contest.createdAt.toString(Qt::ISODate).toStdString()) << "\""
         << "}";
    return json.str();
}

std::string JsonSerializer::contestsToJson(const std::vector<Contest>& contests) {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < contests.size(); ++i) {
        json << contestToJson(contests[i]);
        if (i < contests.size() - 1) json << ",";
    }
    json << "]";
    return json.str();
}

Contest JsonSerializer::jsonToContest(const std::string& json) {
    Contest contest;
    contest.userId = extractIntValue(json, "user_id");
    contest.codeforcesAccountId = extractIntValue(json, "codeforces_account_id");
    contest.name = QString::fromStdString(extractValue(json, "name"));
    
    std::string startTimeStr = extractValue(json, "start_time");
    contest.startTime = QDateTime::fromString(QString::fromStdString(startTimeStr), Qt::ISODate);
    
    std::string endTimeStr = extractValue(json, "end_time");
    if (!endTimeStr.empty()) {
        contest.endTime = QDateTime::fromString(QString::fromStdString(endTimeStr), Qt::ISODate);
    }
    
    contest.createdAt = QDateTime::currentDateTime();
    return contest;
}

// CodeforcesAccount serialization
std::string JsonSerializer::codeforcesAccountToJson(const CodeforcesAccount& account) {
    std::ostringstream json;
    json << "{"
         << "\"id\":" << account.id << ","
         << "\"user_id\":" << account.userId << ","
         << "\"handle\":\"" << escapeJson(account.handle.toStdString()) << "\","
         << "\"api_key\":\"" << escapeJson(account.apiKey.toStdString()) << "\","
         << "\"api_secret\":\"" << escapeJson(account.apiSecret.toStdString()) << "\","
         << "\"created_at\":\"" << escapeJson(account.createdAt.toString(Qt::ISODate).toStdString()) << "\","
         << "\"updated_at\":\"" << escapeJson(account.updatedAt.toString(Qt::ISODate).toStdString()) << "\""
         << "}";
    return json.str();
}

std::string JsonSerializer::codeforcesAccountsToJson(const std::vector<CodeforcesAccount>& accounts) {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < accounts.size(); ++i) {
        json << codeforcesAccountToJson(accounts[i]);
        if (i < accounts.size() - 1) json << ",";
    }
    json << "]";
    return json.str();
}

CodeforcesAccount JsonSerializer::jsonToCodeforcesAccount(const std::string& json) {
    CodeforcesAccount account;
    account.userId = extractIntValue(json, "user_id");
    account.handle = QString::fromStdString(extractValue(json, "handle"));
    account.apiKey = QString::fromStdString(extractValue(json, "api_key"));
    account.apiSecret = QString::fromStdString(extractValue(json, "api_secret"));
    account.createdAt = QDateTime::currentDateTime();
    account.updatedAt = QDateTime::currentDateTime();
    return account;
}

// Utility methods
std::string JsonSerializer::errorResponse(const std::string& message, int code) {
    std::ostringstream json;
    json << "{"
         << "\"error\":true,"
         << "\"code\":" << code << ","
         << "\"message\":\"" << escapeJson(message) << "\""
         << "}";
    return json.str();
}

std::string JsonSerializer::successResponse(const std::string& message) {
    std::ostringstream json;
    json << "{"
         << "\"success\":true,"
         << "\"message\":\"" << escapeJson(message) << "\""
         << "}";
    return json.str();
}

// Private helper methods
std::string JsonSerializer::escapeJson(const std::string& str) {
    std::string escaped = str;
    std::replace(escaped.begin(), escaped.end(), '\"', '\'');
    // Simple escape - in production use proper JSON escaping
    return escaped;
}

std::string JsonSerializer::unescapeJson(const std::string& str) {
    // Simple unescape - implement proper JSON unescaping for production
    return str;
}

std::string JsonSerializer::extractValue(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\":\"";
    size_t startPos = json.find(searchKey);
    if (startPos == std::string::npos) return "";
    
    startPos += searchKey.length();
    size_t endPos = json.find("\"", startPos);
    if (endPos == std::string::npos) return "";
    
    return json.substr(startPos, endPos - startPos);
}

int JsonSerializer::extractIntValue(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\":";
    size_t startPos = json.find(searchKey);
    if (startPos == std::string::npos) return 0;
    
    startPos += searchKey.length();
    size_t endPos = json.find_first_of(",}", startPos);
    if (endPos == std::string::npos) return 0;
    
    std::string valueStr = json.substr(startPos, endPos - startPos);
    return std::stoi(valueStr);
}

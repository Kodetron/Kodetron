#include <crow.h>
#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <string>

#include "DataBase/DatabaseManager.h"
#include "controllers/UsersController.h"
#include "controllers/ThemesController.h"
#include "controllers/SnippetsController.h"
#include "controllers/TemplatesController.h"
#include "controllers/ContestsController.h"
#include "controllers/CodeforcesAccountsController.h"
#include "middleware/AuthMiddleware.h"

class KodetronServer {
private:
    crow::App<AuthMiddleware> app;
    
    // Controllers
    std::unique_ptr<UsersController> usersController;
    std::unique_ptr<ThemesController> themesController;
    std::unique_ptr<SnippetsController> snippetsController;
    std::unique_ptr<TemplatesController> templatesController;
    std::unique_ptr<ContestsController> contestsController;
    std::unique_ptr<CodeforcesAccountsController> codeforcesController;
    
public:
    KodetronServer() {
        setupControllers();
        setupRoutes();
        setupCORS();
    }
    
    void setupControllers() {
        usersController = std::make_unique<UsersController>();
        themesController = std::make_unique<ThemesController>();
        snippetsController = std::make_unique<SnippetsController>();
        templatesController = std::make_unique<TemplatesController>();
        contestsController = std::make_unique<ContestsController>();
        codeforcesController = std::make_unique<CodeforcesAccountsController>();
    }
    
    void setupCORS() {
        // Enable CORS for all routes
        CROW_ROUTE(app, "/").methods("OPTIONS"_method)
        ([](const crow::request& req) {
            crow::response res(200);
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            return res;
        });
    }
    
    void setupRoutes() {
        std::cout << "Setting up API routes..." << std::endl;
        
        // Health check
        CROW_ROUTE(app, "/health")
        ([](const crow::request& req) {
            return crow::response(200, "Kodetron API Server is running");
        });
        
        // API Info
        CROW_ROUTE(app, "/api")
        ([](const crow::request& req) {
            return crow::response(200, R"({
                "name": "Kodetron API",
                "version": "1.0.0",
                "endpoints": {
                    "users": "/api/users",
                    "themes": "/api/themes",
                    "snippets": "/api/snippets",
                    "templates": "/api/templates",
                    "contests": "/api/contests",
                    "codeforces": "/api/codeforces"
                }
            })");
        });
        
        // ===== USERS ROUTES =====
        CROW_ROUTE(app, "/api/users").methods("GET"_method)
        ([this](const crow::request& req) {
            return usersController->getAllUsers(req);
        });
        
        CROW_ROUTE(app, "/api/users").methods("POST"_method)
        ([this](const crow::request& req) {
            return usersController->createUser(req);
        });
        
        CROW_ROUTE(app, "/api/users/<int>").methods("GET"_method)
        ([this](const crow::request& req, int userId) {
            return usersController->getUserById(req, userId);
        });
        
        CROW_ROUTE(app, "/api/users/<int>").methods("PUT"_method)
        ([this](const crow::request& req, int userId) {
            return usersController->updateUser(req, userId);
        });
        
        CROW_ROUTE(app, "/api/users/<int>").methods("DELETE"_method)
        ([this](const crow::request& req, int userId) {
            return usersController->deleteUser(req, userId);
        });
        
        // ===== THEMES ROUTES =====
        CROW_ROUTE(app, "/api/themes").methods("GET"_method)
        ([this](const crow::request& req) {
            return themesController->getThemes(req);
        });
        
        CROW_ROUTE(app, "/api/themes").methods("POST"_method)
        ([this](const crow::request& req) {
            return themesController->createTheme(req);
        });
        
        CROW_ROUTE(app, "/api/themes/<int>").methods("GET"_method)
        ([this](const crow::request& req, int themeId) {
            return themesController->getThemeById(req, themeId);
        });
        
        CROW_ROUTE(app, "/api/themes/<int>").methods("PUT"_method)
        ([this](const crow::request& req, int themeId) {
            return themesController->updateTheme(req, themeId);
        });
        
        CROW_ROUTE(app, "/api/themes/<int>").methods("DELETE"_method)
        ([this](const crow::request& req, int themeId) {
            return themesController->deleteTheme(req, themeId);
        });
        
        // ===== SNIPPETS ROUTES =====
        CROW_ROUTE(app, "/api/snippets").methods("GET"_method)
        ([this](const crow::request& req) {
            return snippetsController->getSnippets(req);
        });
        
        CROW_ROUTE(app, "/api/snippets").methods("POST"_method)
        ([this](const crow::request& req) {
            return snippetsController->createSnippet(req);
        });
        
        CROW_ROUTE(app, "/api/snippets/<int>").methods("GET"_method)
        ([this](const crow::request& req, int snippetId) {
            return snippetsController->getSnippetById(req, snippetId);
        });
        
        CROW_ROUTE(app, "/api/snippets/<int>").methods("PUT"_method)
        ([this](const crow::request& req, int snippetId) {
            return snippetsController->updateSnippet(req, snippetId);
        });
        
        CROW_ROUTE(app, "/api/snippets/<int>").methods("DELETE"_method)
        ([this](const crow::request& req, int snippetId) {
            return snippetsController->deleteSnippet(req, snippetId);
        });
        
        // ===== TEMPLATES ROUTES =====
        CROW_ROUTE(app, "/api/templates").methods("GET"_method)
        ([this](const crow::request& req) {
            return templatesController->getTemplates(req);
        });
        
        CROW_ROUTE(app, "/api/templates").methods("POST"_method)
        ([this](const crow::request& req) {
            return templatesController->createTemplate(req);
        });
        
        CROW_ROUTE(app, "/api/templates/<int>").methods("GET"_method)
        ([this](const crow::request& req, int templateId) {
            return templatesController->getTemplateById(req, templateId);
        });
        
        CROW_ROUTE(app, "/api/templates/<int>").methods("PUT"_method)
        ([this](const crow::request& req, int templateId) {
            return templatesController->updateTemplate(req, templateId);
        });
        
        CROW_ROUTE(app, "/api/templates/<int>").methods("DELETE"_method)
        ([this](const crow::request& req, int templateId) {
            return templatesController->deleteTemplate(req, templateId);
        });
        
        // ===== CONTESTS ROUTES =====
        CROW_ROUTE(app, "/api/contests").methods("GET"_method)
        ([this](const crow::request& req) {
            return contestsController->getContests(req);
        });
        
        CROW_ROUTE(app, "/api/contests").methods("POST"_method)
        ([this](const crow::request& req) {
            return contestsController->createContest(req);
        });
        
        // ===== CODEFORCES ACCOUNTS ROUTES =====
        CROW_ROUTE(app, "/api/codeforces").methods("GET"_method)
        ([this](const crow::request& req) {
            return codeforcesController->getAccounts(req);
        });
        
        CROW_ROUTE(app, "/api/codeforces").methods("POST"_method)
        ([this](const crow::request& req) {
            return codeforcesController->createAccount(req);
        });
        
        std::cout << "All routes configured successfully!" << std::endl;
    }
    
    void run(int port = 8080) {
        std::cout << "Starting Kodetron API Server on port " << port << std::endl;
        std::cout << "API documentation available at: http://localhost:" << port << "/api" << std::endl;
        
        app.port(port)
           .multithreaded()
           .run();
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    // Set application properties
    QCoreApplication::setApplicationName("Kodetron-Server");
    QCoreApplication::setOrganizationName("Kodetron");
    QCoreApplication::setApplicationVersion("1.0.0");
    
    std::cout << "=== Kodetron API Server ===" << std::endl;
    
    // Initialize database
    std::cout << "Initializing database..." << std::endl;
    if (!DatabaseManager::instance()->initialize("kodetron_server.db")) {
        std::cerr << "Failed to initialize database: " 
                  << DatabaseManager::instance()->lastError().toStdString() << std::endl;
        return -1;
    }
    
    std::cout << "Database initialized successfully!" << std::endl;
    
    // Get port from command line or use default
    int port = 8080;
    if (argc > 1) {
        port = std::atoi(argv[1]);
    }
    
    try {
        KodetronServer server;
        server.run(port);
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}

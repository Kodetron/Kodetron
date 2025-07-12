# Kodetron Database Implementation

This directory contains the SQLite database implementation for Kodetron IDE.

## Files

- `schema.sql` - SQLite database schema with all tables and indexes
- `../src/database/DatabaseManager.h` - C++ header for database operations
- `../src/database/DatabaseManager.cpp` - C++ implementation of database operations
- `../src/database/DatabaseExample.cpp` - Example usage of the database
- `../src/database/DatabaseTest.cpp` - Test program to verify database functionality

## Database Schema

The database includes the following tables:

### Users
- `id` - Primary key
- `username` - Unique username
- `email` - User email (optional)
- `created_at` - Timestamp when user was created

### Settings
- `id` - Primary key
- `user_id` - Foreign key to users table
- `key` - Setting name
- `value` - Setting value
- `updated_at` - Timestamp when setting was last updated

### Templates
- `id` - Primary key
- `user_id` - Foreign key to users table
- `name` - Template name
- `content` - Template code content
- `created_at` - Timestamp when template was created
- `updated_at` - Timestamp when template was last updated

### Codeforces Accounts
- `id` - Primary key
- `user_id` - Foreign key to users table
- `handle` - Codeforces handle
- `api_key` - API key (optional)
- `api_secret` - API secret (optional)
- `created_at` - Timestamp when account was created
- `updated_at` - Timestamp when account was last updated

### Contests
- `id` - Primary key
- `user_id` - Foreign key to users table
- `codeforces_account_id` - Foreign key to codeforces_accounts table
- `name` - Contest name
- `start_time` - Contest start time
- `end_time` - Contest end time (optional)
- `created_at` - Timestamp when contest was created

### Snippets
- `id` - Primary key
- `user_id` - Foreign key to users table
- `name` - Snippet name
- `prefix` - Snippet trigger prefix
- `body` - Snippet content
- `description` - Snippet description (optional)
- `created_at` - Timestamp when snippet was created
- `updated_at` - Timestamp when snippet was last updated

### Themes
- `id` - Primary key
- `user_id` - Foreign key to users table
- `name` - Theme name
- `qss_content` - QSS (Qt Style Sheets) content
- `created_at` - Timestamp when theme was created
- `updated_at` - Timestamp when theme was last updated

## Usage

### Basic Usage

```cpp
#include "database/DatabaseManager.h"

// Create database manager
DatabaseManager *dbManager = new DatabaseManager();

// Initialize database (creates kodetron.db file)
if (!dbManager->initialize()) {
    qDebug() << "Database initialization failed:" << dbManager->getLastError();
    return;
}

// Create a user
int userId = dbManager->createUser("username", "email@example.com");

// Set user settings
dbManager->setSetting(userId, "theme", "dark");
dbManager->setSetting(userId, "font_size", "12");

// Get user settings
QString theme = dbManager->getSetting(userId, "theme", "light");

// Create a template
int templateId = dbManager->createTemplate(userId, "Basic C++", "#include <iostream>\n\nint main() {\n    return 0;\n}");

// Get all templates for user
QList<QVariantMap> templates = dbManager->getTemplates(userId);
```

### Integration with Main Application

In your main.cpp:

```cpp
#include "database/DatabaseManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Initialize database
    DatabaseManager *dbManager = new DatabaseManager(&app);
    if (!dbManager->initialize()) {
        qDebug() << "Database initialization failed:" << dbManager->getLastError();
        return -1;
    }
    
    // Pass database manager to main window or other components
    MainWindow mainWindow;
    mainWindow.setDatabaseManager(dbManager);
    
    mainWindow.show();
    return app.exec();
}
```

## Building and Testing

### Build the main application with database support:

```bash
mkdir build && cd build
cmake ..
make
```

### Run the database test:

```bash
cd database_test
mkdir build && cd build
cmake ..
make
./DatabaseTest
```

## Features

- **Automatic table creation** - Tables are created automatically when the database is initialized
- **Foreign key constraints** - Proper relationships between tables with cascade deletes
- **Indexes** - Performance indexes on frequently queried columns
- **Error handling** - Comprehensive error reporting
- **Prepared statements** - SQL injection protection
- **Transaction support** - Can be extended for complex operations

## Error Handling

All database operations return appropriate success/failure indicators:

- `int` return values: -1 indicates failure, positive values indicate success (usually the ID of created record)
- `bool` return values: `true` for success, `false` for failure
- Use `getLastError()` to get detailed error messages

## Security Notes

- API keys and secrets are stored in plain text - consider encryption for production use
- User input should be validated before database operations
- The database file should be protected with appropriate file permissions

## Future Enhancements

- Add encryption for sensitive data
- Implement database migrations for schema updates
- Add connection pooling for better performance
- Implement database backup and restore functionality
- Add data validation and sanitization 
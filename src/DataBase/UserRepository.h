#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "Models.h"
#include <QList>

class UserRepository
{
public:
    UserRepository() = default;
    
    // CRUD operations
    bool create(User& user);
    User findById(int id);  // Returns invalid user if not found
    User findByUsername(const QString& username);  // Returns invalid user if not found
    QList<User> findAll();
    bool update(const User& user);
    bool deleteById(int id);
    
    // Utility methods
    bool exists(int id);
    bool existsByUsername(const QString& username);
    int count();
    
private:
    User mapFromQuery(const class QSqlQuery& query);
};

#endif // USERREPOSITORY_H

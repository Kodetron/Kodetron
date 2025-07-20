#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include "DataBase/DatabaseManager.h"
#include "DataBase/UserRepository.h"
#include "DataBase/ThemeRepository.h"

class DatabaseTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Use a temporary database for testing
        QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        testDbPath = tempDir + "/test_kodetron.db";
        
        // Remove if exists
        if (QFile::exists(testDbPath)) {
            QFile::remove(testDbPath);
        }
        
        // Initialize database
        ASSERT_TRUE(DatabaseManager::instance()->initialize(testDbPath));
    }
    
    void TearDown() override
    {
        DatabaseManager::instance()->close();
        
        // Clean up test database
        if (QFile::exists(testDbPath)) {
            QFile::remove(testDbPath);
        }
    }
    
    QString testDbPath;
};

TEST_F(DatabaseTest, DatabaseInitialization)
{
    EXPECT_TRUE(DatabaseManager::instance()->isConnected());
}

TEST_F(DatabaseTest, UserCRUD)
{
    UserRepository userRepo;
    
    // Test create
    User user("testuser", "test@example.com");
    EXPECT_TRUE(userRepo.create(user));
    EXPECT_GT(user.id, 0);
    
    // Test find by ID
    User foundUser = userRepo.findById(user.id);
    ASSERT_TRUE(foundUser.isValid());
    EXPECT_EQ(foundUser.username, "testuser");
    EXPECT_EQ(foundUser.email, "test@example.com");
    
    // Test find by username
    User foundByUsername = userRepo.findByUsername("testuser");
    ASSERT_TRUE(foundByUsername.isValid());
    EXPECT_EQ(foundByUsername.id, user.id);
    
    // Test update
    foundUser.email = "newemail@example.com";
    EXPECT_TRUE(userRepo.update(foundUser));
    
    User updatedUser = userRepo.findById(user.id);
    ASSERT_TRUE(updatedUser.isValid());
    EXPECT_EQ(updatedUser.email, "newemail@example.com");
    
    // Test exists
    EXPECT_TRUE(userRepo.exists(user.id));
    EXPECT_TRUE(userRepo.existsByUsername("testuser"));
    
    // Test count
    EXPECT_EQ(userRepo.count(), 1);
    
    // Test delete
    EXPECT_TRUE(userRepo.deleteById(user.id));
    EXPECT_FALSE(userRepo.exists(user.id));
    EXPECT_EQ(userRepo.count(), 0);
}

TEST_F(DatabaseTest, ThemeCRUD)
{
    // First create a user
    UserRepository userRepo;
    User user("themeuser", "theme@example.com");
    ASSERT_TRUE(userRepo.create(user));
    
    ThemeRepository themeRepo;
    
    // Test create
    Theme theme(user.id, "Dark Theme", "QWidget { background-color: #2b2b2b; }");
    EXPECT_TRUE(themeRepo.create(theme));
    EXPECT_GT(theme.id, 0);
    
    // Test find by ID
    Theme foundTheme = themeRepo.findById(theme.id);
    ASSERT_TRUE(foundTheme.isValid());
    EXPECT_EQ(foundTheme.name, "Dark Theme");
    EXPECT_EQ(foundTheme.userId, user.id);
    
    // Test find by user ID
    auto userThemes = themeRepo.findByUserId(user.id);
    EXPECT_EQ(userThemes.size(), 1);
    EXPECT_EQ(userThemes[0].name, "Dark Theme");
    
    // Test find by user ID and name
    Theme specificTheme = themeRepo.findByUserIdAndName(user.id, "Dark Theme");
    ASSERT_TRUE(specificTheme.isValid());
    EXPECT_EQ(specificTheme.id, theme.id);
    
    // Test update
    foundTheme.qssContent = "QWidget { background-color: #1e1e1e; }";
    EXPECT_TRUE(themeRepo.update(foundTheme));
    
    Theme updatedTheme = themeRepo.findById(theme.id);
    ASSERT_TRUE(updatedTheme.isValid());
    EXPECT_EQ(updatedTheme.qssContent, "QWidget { background-color: #1e1e1e; }");
    
    // Test count by user ID
    EXPECT_EQ(themeRepo.countByUserId(user.id), 1);
    
    // Test delete
    EXPECT_TRUE(themeRepo.deleteById(theme.id));
    EXPECT_FALSE(themeRepo.exists(theme.id));
    EXPECT_EQ(themeRepo.countByUserId(user.id), 0);
}

TEST_F(DatabaseTest, Transactions)
{
    DatabaseManager* db = DatabaseManager::instance();
    UserRepository userRepo;
    
    EXPECT_TRUE(db->beginTransaction());
    
    User user1("user1", "user1@example.com");
    User user2("user2", "user2@example.com");
    
    EXPECT_TRUE(userRepo.create(user1));
    EXPECT_TRUE(userRepo.create(user2));
    
    EXPECT_TRUE(db->commitTransaction());
    
    // Verify both users were created
    EXPECT_EQ(userRepo.count(), 2);
    
    // Test rollback
    EXPECT_TRUE(db->beginTransaction());
    
    User user3("user3", "user3@example.com");
    EXPECT_TRUE(userRepo.create(user3));
    
    EXPECT_TRUE(db->rollbackTransaction());
    
    // Verify user3 was not persisted
    EXPECT_EQ(userRepo.count(), 2);
    EXPECT_FALSE(userRepo.existsByUsername("user3"));
}

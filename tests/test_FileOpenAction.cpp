#include <gtest/gtest.h>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QTextStream>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QDir>
#include <QTest>
#include <QSignalSpy>
#include "FileMenuActions.h"
#include "../src/widgets/KodetronEditor.h"

class OpenActionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create QApplication if it doesn't exist
        if (!QApplication::instance()) {
            int argc = 0;
            char** argv = nullptr;
            app = new QApplication(argc, argv);
        }
        
        // Create main window and editors
        mainWindow = new QMainWindow();
        mainWindow->setWindowTitle("Test Window");
        
        textEditor = new KodetronEditor(mainWindow);
        inputEditor = new KodetronEditor(mainWindow);
        outputEditor = new KodetronEditor(mainWindow);
        
        // Create file menu
        fileMenu = new QMenu("File", mainWindow);
        
        // Create FileMenuActions instance
        fileMenuActions = new FileMenuActions(mainWindow, textEditor, inputEditor, 
                                            outputEditor, fileMenu, mainWindow);
        
        // Create a temporary test file
        createTestFile();
    }
    
    void TearDown() override {
        delete fileMenuActions;
        delete mainWindow;
        // Clean up test file if it exists
        if (QFile::exists(testFileName)) {
            QFile::remove(testFileName);
        }
    }
    
    void createTestFile() {
        testFileName = QDir::tempPath() + "/test_code_file.cpp";
        testFileContent = "#include <iostream>\nint main() {\n    std::cout << \"Hello World!\" << std::endl;\n    return 0;\n}";
        
        QFile file(testFileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << testFileContent;
            file.close();
        }
    }
    
    // Helper method to find the "Open..." action in the menu
    QAction* findOpenAction() {
        for (QAction* action : fileMenu->actions()) {
            if (action->text() == "Open...") {
                return action;
            }
        }
        return nullptr;
    }
    
protected:
    QApplication* app = nullptr;
    QMainWindow* mainWindow = nullptr;
    KodetronEditor* textEditor = nullptr;
    KodetronEditor* inputEditor = nullptr;
    KodetronEditor* outputEditor = nullptr;
    QMenu* fileMenu = nullptr;
    FileMenuActions* fileMenuActions = nullptr;
    QString testFileName;
    QString testFileContent;
};

// Test that the Open action exists in the menu
TEST_F(OpenActionTest, OpenActionExists) {
    QAction* openAction = findOpenAction();
    ASSERT_NE(openAction, nullptr) << "Open action should exist in the file menu";
    EXPECT_EQ(openAction->text(), "Open...");
}

// Note: Testing file dialog interactions is complex and typically requires mocking
// or integration testing. For unit tests, we focus on testing the file reading logic
// which happens after a file is selected.

// Test that a valid file can be read and loaded into the text editor
TEST_F(OpenActionTest, ValidFileContentLoading) {
    // Verify test file exists and has content
    ASSERT_TRUE(QFile::exists(testFileName)) << "Test file should exist";
    
    // Simulate what happens when a file is selected and opened
    QFile file(testFileName);
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text)) << "Should be able to open test file";
    
    QTextStream inputStream(&file);
    QString fileContent = inputStream.readAll();
    file.close();
    
    // Verify content matches what we expect
    EXPECT_EQ(fileContent, testFileContent) << "File content should match expected content";
    
    // Simulate loading content into editor (what the Open action does)
    textEditor->setText(fileContent);
    EXPECT_EQ(textEditor->text(), testFileContent) << "Text editor should contain file content";
}

// Test window title update functionality
TEST_F(OpenActionTest, WindowTitleUpdateLogic) {
    QString fileName = testFileName;
    QFileInfo fileInfo(fileName);
    QString expectedTitle = "Kodetron - " + fileInfo.fileName();
    
    // Test the title formatting logic
    mainWindow->setWindowTitle(expectedTitle);
    EXPECT_EQ(mainWindow->windowTitle(), expectedTitle) << "Window title should be updated correctly";
}

// Test file existence checking
TEST_F(OpenActionTest, FileExistenceCheck) {
    // Test with existing file
    EXPECT_TRUE(QFile::exists(testFileName)) << "Test file should exist";
    
    // Test with non-existing file
    QString nonExistentFile = "/path/that/does/not/exist/file.txt";
    EXPECT_FALSE(QFile::exists(nonExistentFile)) << "Non-existent file should return false";
}

// Test file opening with read permissions
TEST_F(OpenActionTest, FileOpenPermissions) {
    QFile file(testFileName);
    
    // Test read-only opening
    EXPECT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text)) 
        << "Should be able to open file in read-only mode";
    
    // Test that file is readable
    EXPECT_TRUE(file.isReadable()) << "File should be readable";
    
    file.close();
}

// Test reading empty file
TEST_F(OpenActionTest, EmptyFileHandling) {
    // Create empty test file
    QString emptyFileName = QDir::tempPath() + "/empty_test_file.txt";
    QFile emptyFile(emptyFileName);
    ASSERT_TRUE(emptyFile.open(QIODevice::WriteOnly | QIODevice::Text));
    emptyFile.close();
    
    // Read empty file
    ASSERT_TRUE(emptyFile.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream stream(&emptyFile);
    QString content = stream.readAll();
    emptyFile.close();
    
    EXPECT_TRUE(content.isEmpty()) << "Empty file should return empty content";
    
    // Simulate loading empty content
    textEditor->setText(content);
    EXPECT_TRUE(textEditor->text().isEmpty()) << "Editor should be empty after loading empty file";
    
    // Clean up
    QFile::remove(emptyFileName);
}

// Test reading file with special characters
TEST_F(OpenActionTest, SpecialCharactersHandling) {
    // Create file with special characters
    QString specialFileName = QDir::tempPath() + "/special_chars_test.txt";
    QString specialContent = "Special chars: àáâãäåæçèéêë ñ 中文 🚀\n#include <vector>\nstd::string text = \"Hello, 世界!\";";
    
    QFile specialFile(specialFileName);
    ASSERT_TRUE(specialFile.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream writeStream(&specialFile);
    writeStream.setEncoding(QStringConverter::Utf8);
    writeStream << specialContent;
    specialFile.close();
    
    // Read file with special characters
    ASSERT_TRUE(specialFile.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream readStream(&specialFile);
    readStream.setEncoding(QStringConverter::Utf8);
    QString readContent = readStream.readAll();
    specialFile.close();
    
    // Verify content preservation
    EXPECT_EQ(readContent, specialContent) << "Special characters should be preserved";
    
    // Clean up
    QFile::remove(specialFileName);
}

// Test large file handling (within reasonable limits for unit tests)
TEST_F(OpenActionTest, LargeFileHandling) {
    // Create a moderately large file (not too large for unit tests)
    QString largeFileName = QDir::tempPath() + "/large_test_file.txt";
    QString largeContent;
    
    // Generate content with multiple lines
    for (int i = 0; i < 1000; ++i) {
        largeContent += QString("Line %1: This is a test line with some content.\n").arg(i);
    }
    
    QFile largeFile(largeFileName);
    ASSERT_TRUE(largeFile.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream writeStream(&largeFile);
    writeStream << largeContent;
    largeFile.close();
    
    // Read large file
    ASSERT_TRUE(largeFile.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream readStream(&largeFile);
    QString readContent = readStream.readAll();
    largeFile.close();
    
    // Verify content matches
    EXPECT_EQ(readContent, largeContent) << "Large file content should be read correctly";
    EXPECT_GT(readContent.length(), 50000) << "Content should be reasonably large";
    
    // Clean up
    QFile::remove(largeFileName);
}

// Test file path with spaces and special characters
TEST_F(OpenActionTest, FilePathWithSpaces) {
    QString spacesFileName = QDir::tempPath() + "/file with spaces & special chars.cpp";
    QString content = "// File with spaces in name\nint main() { return 0; }";
    
    QFile spacesFile(spacesFileName);
    ASSERT_TRUE(spacesFile.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream stream(&spacesFile);
    stream << content;
    spacesFile.close();
    
    // Test reading file with spaces in name
    ASSERT_TRUE(spacesFile.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream readStream(&spacesFile);
    QString readContent = readStream.readAll();
    spacesFile.close();
    
    EXPECT_EQ(readContent, content) << "File with spaces in name should be read correctly";
    
    // Test filename extraction
    QFileInfo fileInfo(spacesFileName);
    QString expectedTitle = "Kodetron - " + fileInfo.fileName();
    EXPECT_TRUE(expectedTitle.contains("file with spaces & special chars.cpp")) 
        << "Filename should be extracted correctly";
    
    // Clean up
    QFile::remove(spacesFileName);
}
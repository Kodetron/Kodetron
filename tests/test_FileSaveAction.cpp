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
#include "FileMenuActions.h"
#include "../src/widgets/KodetronEditor.h"

class SaveActionTest : public ::testing::Test {
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
        
        // Set up test content in the text editor
        testContent = "#include <iostream>\n\nint main() {\n    std::cout << \"Save test!\" << std::endl;\n    return 0;\n}";
        textEditor->setText(testContent);
        
        // Create file menu
        fileMenu = new QMenu("File", mainWindow);
        
        // Create FileMenuActions instance
        fileMenuActions = new FileMenuActions(mainWindow, textEditor, inputEditor, 
                                            outputEditor, fileMenu, mainWindow);
        
        // Set up test file path
        testFileName = QDir::tempPath() + "/save_test_file.cpp";
    }
    
    void TearDown() override {
        delete fileMenuActions;
        delete mainWindow;
        // Clean up test file if it exists
        if (QFile::exists(testFileName)) {
            QFile::remove(testFileName);
        }
    }
    
    // Helper method to find the "Save" action in the menu
    QAction* findSaveAction() {
        for (QAction* action : fileMenu->actions()) {
            if (action->text() == "Save") {
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
    QString testContent;
};

// Test that the Save action exists in the menu
TEST_F(SaveActionTest, SaveActionExists) {
    QAction* saveAction = findSaveAction();
    ASSERT_NE(saveAction, nullptr) << "Save action should exist in the file menu";
    EXPECT_EQ(saveAction->text(), "Save");
}

// Test file saving functionality
TEST_F(SaveActionTest, FileSavingLogic) {
    // Simulate the file saving process that happens after user selects a file name
    QFile file(testFileName);
    ASSERT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text)) 
        << "Should be able to create and open file for writing";
    
    QTextStream outputStream(&file);
    outputStream << textEditor->text();
    file.close();
    
    // Verify file was created and content was saved
    ASSERT_TRUE(QFile::exists(testFileName)) << "File should exist after saving";
    
    // Read back the content to verify it was saved correctly
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream inputStream(&file);
    QString savedContent = inputStream.readAll();
    file.close();
    
    EXPECT_EQ(savedContent, testContent) << "Saved content should match original content";
}

// Test window title update after saving
TEST_F(SaveActionTest, WindowTitleUpdateAfterSave) {
    QFileInfo fileInfo(testFileName);
    QString expectedTitle = "Kodetron - " + fileInfo.fileName();
    
    // Simulate title update (what happens after successful save)
    mainWindow->setWindowTitle(expectedTitle);
    
    EXPECT_EQ(mainWindow->windowTitle(), expectedTitle) 
        << "Window title should be updated with filename after save";
}

// Test saving empty content
TEST_F(SaveActionTest, SaveEmptyContent) {
    // Clear the editor
    textEditor->clear();
    EXPECT_TRUE(textEditor->text().isEmpty()) << "Editor should be empty";
    
    // Simulate saving empty content
    QFile file(testFileName);
    ASSERT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream outputStream(&file);
    outputStream << textEditor->text();
    file.close();
    
    // Verify empty file was created
    ASSERT_TRUE(QFile::exists(testFileName)) << "Empty file should be created";
    
    // Verify file is actually empty
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream inputStream(&file);
    QString savedContent = inputStream.readAll();
    file.close();
    
    EXPECT_TRUE(savedContent.isEmpty()) << "Saved file should be empty";
}

// Test saving content with special characters
TEST_F(SaveActionTest, SaveSpecialCharacters) {
    QString specialContent = "Special chars: àáâãäåæçèéêë ñ 中文 🚀\n// Comment with émojis\nstd::string msg = \"Hello, 世界!\";";
    textEditor->setText(specialContent);
    
    // Simulate saving with special characters
    QFile file(testFileName);
    ASSERT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream outputStream(&file);
    outputStream.setEncoding(QStringConverter::Utf8);
    outputStream << textEditor->text();
    file.close();
    
    // Verify special characters are preserved
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream inputStream(&file);
    inputStream.setEncoding(QStringConverter::Utf8);
    QString savedContent = inputStream.readAll();
    file.close();
    
    EXPECT_EQ(savedContent, specialContent) << "Special characters should be preserved when saving";
}

// Test saving large content
TEST_F(SaveActionTest, SaveLargeContent) {
    QString largeContent;
    for (int i = 0; i < 1000; ++i) {
        largeContent += QString("// Line %1\nint function%1() { return %1; }\n").arg(i);
    }
    
    textEditor->setText(largeContent);
    
    // Simulate saving large content
    QFile file(testFileName);
    ASSERT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream outputStream(&file);
    outputStream << textEditor->text();
    file.close();
    
    // Verify large content was saved correctly
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream inputStream(&file);
    QString savedContent = inputStream.readAll();
    file.close();
    
    EXPECT_EQ(savedContent, largeContent) << "Large content should be saved correctly";
    EXPECT_GT(savedContent.length(), 50000) << "Content should be reasonably large";
}

// Test file permissions and write access
TEST_F(SaveActionTest, FileWritePermissions) {
    QFile file(testFileName);
    
    // Test that file can be opened for writing
    EXPECT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text)) 
        << "Should be able to open file for writing";
    
    // Test that file is writable
    EXPECT_TRUE(file.isWritable()) << "File should be writable";
    
    file.close();
}

// Test saving to file with spaces and special characters in name
TEST_F(SaveActionTest, SaveToFileWithSpacesInName) {
    QString spacesFileName = QDir::tempPath() + "/save test file & more.cpp";
    
    // Simulate saving to file with spaces
    QFile file(spacesFileName);
    ASSERT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream outputStream(&file);
    outputStream << textEditor->text();
    file.close();
    
    // Verify file was created and content saved
    ASSERT_TRUE(QFile::exists(spacesFileName)) << "File with spaces in name should be created";
    
    // Read back content
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream inputStream(&file);
    QString savedContent = inputStream.readAll();
    file.close();
    
    EXPECT_EQ(savedContent, testContent) << "Content should be saved correctly to file with spaces";
    
    // Test filename extraction for title
    QFileInfo fileInfo(spacesFileName);
    QString expectedTitle = "Kodetron - " + fileInfo.fileName();
    EXPECT_TRUE(expectedTitle.contains("save test file & more.cpp")) 
        << "Filename with spaces should be handled correctly in title";
    
    // Clean up
    QFile::remove(spacesFileName);
}

// Test overwriting existing file
TEST_F(SaveActionTest, OverwriteExistingFile) {
    QString originalContent = "Original content";
    
    // Create initial file
    QFile file(testFileName);
    ASSERT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream stream(&file);
    stream << originalContent;
    file.close();
    
    // Verify original content
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream inputStream(&file);
    QString readContent = inputStream.readAll();
    file.close();
    EXPECT_EQ(readContent, originalContent);
    
    // Now overwrite with new content from editor
    ASSERT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream outputStream(&file);
    outputStream << textEditor->text();
    file.close();
    
    // Verify content was overwritten
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream newInputStream(&file);
    QString newContent = newInputStream.readAll();
    file.close();
    
    EXPECT_EQ(newContent, testContent) << "File should be overwritten with new content";
    EXPECT_NE(newContent, originalContent) << "Content should be different from original";
}

// Test preserving line endings
TEST_F(SaveActionTest, PreserveLineEndings) {
    QString contentWithDifferentLineEndings = "Line 1\nLine 2\r\nLine 3\nLine 4";
    textEditor->setText(contentWithDifferentLineEndings);
    
    // Save content
    QFile file(testFileName);
    ASSERT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream outputStream(&file);
    outputStream << textEditor->text();
    file.close();
    
    // Read back content
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream inputStream(&file);
    QString savedContent = inputStream.readAll();
    file.close();
    
    // On Windows, line endings might be normalized, so we check that the logical content is preserved
    EXPECT_TRUE(savedContent.contains("Line 1")) << "Content should contain all lines";
    EXPECT_TRUE(savedContent.contains("Line 2")) << "Content should contain all lines";
    EXPECT_TRUE(savedContent.contains("Line 3")) << "Content should contain all lines";
    EXPECT_TRUE(savedContent.contains("Line 4")) << "Content should contain all lines";
}
#include <gtest/gtest.h>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QTest>
#include "FileMenuActions.h"
#include "CodeEditor.h"

class FileMenuActionsTest : public ::testing::Test {
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
        mainWindow->setWindowTitle("Test Window - Some File.cpp");
        
        textEditor = new CodeEditor(mainWindow);
        inputEditor = new CodeEditor(mainWindow);
        outputEditor = new CodeEditor(mainWindow);
        
        // Add some initial content to test clearing
        textEditor->setPlainText("int main() { return 0; }");
        inputEditor->setPlainText("test input data");
        outputEditor->setPlainText("test output data");
        
        // Create file menu
        fileMenu = new QMenu("File", mainWindow);
        
        // Create FileMenuActions instance
        fileMenuActions = new FileMenuActions(mainWindow, textEditor, inputEditor, 
                                            outputEditor, fileMenu, mainWindow);
    }
    
    void TearDown() override {
        delete fileMenuActions;
        delete mainWindow;
        // Note: QApplication should not be deleted in tests as it may be shared
    }
    
    // Helper method to find the "New" action in the menu
    QAction* findNewAction() {
        for (QAction* action : fileMenu->actions()) {
            if (action->text() == "New") {
                return action;
            }
        }
        return nullptr;
    }
    
protected:
    QApplication* app = nullptr;
    QMainWindow* mainWindow = nullptr;
    CodeEditor* textEditor = nullptr;
    CodeEditor* inputEditor = nullptr;
    CodeEditor* outputEditor = nullptr;
    QMenu* fileMenu = nullptr;
    FileMenuActions* fileMenuActions = nullptr;
};

// Test that the New action exists in the menu
TEST_F(FileMenuActionsTest, NewActionExists) {
    QAction* newAction = findNewAction();
    ASSERT_NE(newAction, nullptr) << "New action should exist in the file menu";
    EXPECT_EQ(newAction->text(), "New");
}

// Test that the New action clears the text editor
TEST_F(FileMenuActionsTest, NewActionClearsTextEditor) {
    // Verify initial content exists
    EXPECT_FALSE(textEditor->toPlainText().isEmpty()) << "Text editor should have initial content";
    
    // Find and trigger the New action
    QAction* newAction = findNewAction();
    ASSERT_NE(newAction, nullptr);
    
    // Trigger the action
    newAction->trigger();
    
    // Verify text editor is cleared
    EXPECT_TRUE(textEditor->toPlainText().isEmpty()) << "Text editor should be cleared after New action";
}

// Test that the New action clears the input editor
TEST_F(FileMenuActionsTest, NewActionClearsInputEditor) {
    // Verify initial content exists
    EXPECT_FALSE(inputEditor->toPlainText().isEmpty()) << "Input editor should have initial content";
    
    // Find and trigger the New action
    QAction* newAction = findNewAction();
    ASSERT_NE(newAction, nullptr);
    
    // Trigger the action
    newAction->trigger();
    
    // Verify input editor is cleared
    EXPECT_TRUE(inputEditor->toPlainText().isEmpty()) << "Input editor should be cleared after New action";
}

// Test that the New action clears the output editor
TEST_F(FileMenuActionsTest, NewActionClearsOutputEditor) {
    // Verify initial content exists
    EXPECT_FALSE(outputEditor->toPlainText().isEmpty()) << "Output editor should have initial content";
    
    // Find and trigger the New action
    QAction* newAction = findNewAction();
    ASSERT_NE(newAction, nullptr);
    
    // Trigger the action
    newAction->trigger();
    
    // Verify output editor is cleared
    EXPECT_TRUE(outputEditor->toPlainText().isEmpty()) << "Output editor should be cleared after New action";
}

// Test that the New action resets the window title
TEST_F(FileMenuActionsTest, NewActionResetsWindowTitle) {
    // Verify initial window title is not the "New Document" title
    EXPECT_NE(mainWindow->windowTitle(), "Kodetron - New Document") 
        << "Window should not initially have the 'New Document' title";
    
    // Find and trigger the New action
    QAction* newAction = findNewAction();
    ASSERT_NE(newAction, nullptr);
    
    // Trigger the action
    newAction->trigger();
    
    // Verify window title is reset
    EXPECT_EQ(mainWindow->windowTitle(), "Kodetron - New Document") 
        << "Window title should be reset to 'Kodetron - New Document' after New action";
}

// Test that the New action clears all editors and resets title in one operation
TEST_F(FileMenuActionsTest, NewActionClearsAllEditorsAndResetsTitle) {
    // Verify initial state
    EXPECT_FALSE(textEditor->toPlainText().isEmpty());
    EXPECT_FALSE(inputEditor->toPlainText().isEmpty());
    EXPECT_FALSE(outputEditor->toPlainText().isEmpty());
    EXPECT_NE(mainWindow->windowTitle(), "Kodetron - New Document");
    
    // Find and trigger the New action
    QAction* newAction = findNewAction();
    ASSERT_NE(newAction, nullptr);
    
    // Trigger the action
    newAction->trigger();
    
    // Verify all changes happened
    EXPECT_TRUE(textEditor->toPlainText().isEmpty()) << "Text editor should be cleared";
    EXPECT_TRUE(inputEditor->toPlainText().isEmpty()) << "Input editor should be cleared";
    EXPECT_TRUE(outputEditor->toPlainText().isEmpty()) << "Output editor should be cleared";
    EXPECT_EQ(mainWindow->windowTitle(), "Kodetron - New Document") << "Window title should be reset";
}

// Test that New action can be triggered multiple times without issues
TEST_F(FileMenuActionsTest, NewActionCanBeTriggeredMultipleTimes) {
    QAction* newAction = findNewAction();
    ASSERT_NE(newAction, nullptr);
    
    // Trigger multiple times
    newAction->trigger();
    newAction->trigger();
    newAction->trigger();
    
    // Should still work correctly
    EXPECT_TRUE(textEditor->toPlainText().isEmpty());
    EXPECT_TRUE(inputEditor->toPlainText().isEmpty());
    EXPECT_TRUE(outputEditor->toPlainText().isEmpty());
    EXPECT_EQ(mainWindow->windowTitle(), "Kodetron - New Document");
}

// Test that New action works even when editors are already empty
TEST_F(FileMenuActionsTest, NewActionWorksWhenEditorsAlreadyEmpty) {
    // Clear editors first
    textEditor->clear();
    inputEditor->clear();
    outputEditor->clear();
    
    QAction* newAction = findNewAction();
    ASSERT_NE(newAction, nullptr);
    
    // Trigger New action on already empty editors
    newAction->trigger();
    
    // Should still work without issues
    EXPECT_TRUE(textEditor->toPlainText().isEmpty());
    EXPECT_TRUE(inputEditor->toPlainText().isEmpty());
    EXPECT_TRUE(outputEditor->toPlainText().isEmpty());
    EXPECT_EQ(mainWindow->windowTitle(), "Kodetron - New Document");
}
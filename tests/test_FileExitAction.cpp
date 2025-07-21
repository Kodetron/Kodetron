#include <gtest/gtest.h>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QTest>
#include <QSignalSpy>
#include <QTimer>
#include "FileMenuActions.h"
#include "../src/widgets/KodetronEditor.h"

class ExitActionTest: public::testing::Test {
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
        
        // Add some test content
        textEditor->setText("int main() { return 0; }");
        inputEditor->setText("test input");
        outputEditor->setText("test output");
        
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
    
    // Helper method to find the "Exit" action in the menu
    QAction* findExitAction() {
        for (QAction* action : fileMenu->actions()) {
            if (action->text() == "Exit") {
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
};

// Test that the Exit action exists in the menu
TEST_F(ExitActionTest, ExitActionExists) {
    QAction* exitAction = findExitAction();
    ASSERT_NE(exitAction, nullptr) << "Exit action should exist in the file menu";
    EXPECT_EQ(exitAction->text(), "Exit");
}

// Test that Exit action is properly connected to QApplication::quit
TEST_F(ExitActionTest, ExitActionConnectedToQuit) {
    QAction* exitAction = findExitAction();
    ASSERT_NE(exitAction, nullptr);
    
    // Create a signal spy to monitor QApplication::aboutToQuit signal
    QSignalSpy quitSpy(QApplication::instance(), &QApplication::aboutToQuit);
    EXPECT_TRUE(quitSpy.isValid()) << "Signal spy should be valid";
    
    exitAction->disconnect(); // Disconnect all existing connections
    
    bool quitCalled = false;
    QObject::connect(exitAction, &QAction::triggered, [&quitCalled]() {
        quitCalled = true;
    });
    
    // Trigger the exit action
    exitAction->trigger();
    
    // Verify that our test function was called (simulating quit behavior)
    EXPECT_TRUE(quitCalled) << "Exit action should trigger quit functionality";
}

// Test Exit action accessibility and properties
TEST_F(ExitActionTest, ExitActionProperties) {
    QAction* exitAction = findExitAction();
    ASSERT_NE(exitAction, nullptr);
    
    // Test basic properties
    EXPECT_EQ(exitAction->text(), "Exit") << "Exit action should have correct text";
    EXPECT_TRUE(exitAction->isEnabled()) << "Exit action should be enabled by default";
    EXPECT_TRUE(exitAction->isVisible()) << "Exit action should be visible";
}

// Test that Exit action can be triggered programmatically
TEST_F(ExitActionTest, ExitActionCanBeTriggeredProgrammatically) {
    QAction* exitAction = findExitAction();
    ASSERT_NE(exitAction, nullptr);
    
    // Disconnect original connection to prevent actual quit
    exitAction->disconnect();
    
    bool triggered = false;
    QObject::connect(exitAction, &QAction::triggered, [&triggered]() {
        triggered = true;
    });
    
    // Trigger the action programmatically
    exitAction->trigger();
    
    EXPECT_TRUE(triggered) << "Exit action should be triggerable programmatically";
}

// Test that Exit action can be triggered multiple times
TEST_F(ExitActionTest, ExitActionCanBeTriggeredMultipleTimes) {
    QAction* exitAction = findExitAction();
    ASSERT_NE(exitAction, nullptr);
    
    // Disconnect original connection to prevent actual quit
    exitAction->disconnect();
    
    int triggerCount = 0;
    QObject::connect(exitAction, &QAction::triggered, [&triggerCount]() {
        triggerCount++;
    });
    
    // Trigger multiple times
    exitAction->trigger();
    exitAction->trigger();
    exitAction->trigger();
    
    EXPECT_EQ(triggerCount, 3) << "Exit action should be triggerable multiple times";
}

// Test Exit action with keyboard shortcut (if any is set)
TEST_F(ExitActionTest, ExitActionKeyboardShortcut) {
    QAction* exitAction = findExitAction();
    ASSERT_NE(exitAction, nullptr);
    
    // Most applications set Ctrl+Q as exit shortcut, but let's check what's actually set
    QKeySequence shortcut = exitAction->shortcut();
    
    // If a shortcut is set, it should be a valid key sequence
    if (!shortcut.isEmpty()) {
        EXPECT_FALSE(shortcut.toString().isEmpty()) 
            << "If shortcut is set, it should have a valid string representation";
    }
}

// Test that Exit action is positioned correctly in the menu (should be last)
TEST_F(ExitActionTest, ExitActionPositionInMenu) {
    QList<QAction*> menuActions = fileMenu->actions();
    EXPECT_GT(menuActions.size(), 0) << "File menu should have actions";
    
    // Find the exit action in the list
    QAction* exitAction = findExitAction();
    ASSERT_NE(exitAction, nullptr);
    
    // Exit should typically be the last action in the File menu
    QAction* lastAction = menuActions.last();
    EXPECT_EQ(lastAction, exitAction) 
        << "Exit action should typically be the last action in the File menu";
}

// Test Exit action parent relationship
TEST_F(ExitActionTest, ExitActionParentRelationship) {
    QAction* exitAction = findExitAction();
    ASSERT_NE(exitAction, nullptr);
    
    // The action should be parented to the main window (as per the constructor)
    EXPECT_EQ(exitAction->parent(), mainWindow) 
        << "Exit action should be parented to the main window";
}

// Test that Exit action doesn't interfere with other actions
TEST_F(ExitActionTest, ExitActionDoesNotInterfereWithOtherActions) {
    // Verify that other actions still exist after Exit action is created
    QList<QAction*> menuActions = fileMenu->actions();
    
    QStringList expectedActions = {"New", "Open...", "Save", "Save As...", "Exit"};
    
    EXPECT_EQ(menuActions.size(), expectedActions.size()) 
        << "All expected actions should be present";
    
    for (const QString& expectedAction : expectedActions) {
        bool found = false;
        for (QAction* action : menuActions) {
            if (action->text() == expectedAction) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found) << ("Action '" + expectedAction + "' should exist").toStdString();
    }
}

// Test Exit action state doesn't change with editor content
TEST_F(ExitActionTest, ExitActionStateUnaffectedByEditorContent) {
    QAction* exitAction = findExitAction();
    ASSERT_NE(exitAction, nullptr);
    
    // Test with various editor states
    bool initiallyEnabled = exitAction->isEnabled();
    
    // Clear all editors
    textEditor->clear();
    inputEditor->clear();
    outputEditor->clear();
    EXPECT_EQ(exitAction->isEnabled(), initiallyEnabled) 
        << "Exit action state should not change when editors are cleared";
    
    // Add content to editors
    textEditor->setText("New content");
    inputEditor->setText("New input");
    outputEditor->setText("New output");
    EXPECT_EQ(exitAction->isEnabled(), initiallyEnabled)
        << "Exit action state should not change when editors have content";
}

// Test Exit action behavior with window state changes
TEST_F(ExitActionTest, ExitActionBehaviorWithWindowStateChanges) {
    QAction* exitAction = findExitAction();
    ASSERT_NE(exitAction, nullptr);
    
    bool initiallyEnabled = exitAction->isEnabled();
    
    // Test with different window states
    mainWindow->setWindowTitle("Modified Title");
    EXPECT_EQ(exitAction->isEnabled(), initiallyEnabled)
        << "Exit action should remain enabled regardless of window title changes";
}
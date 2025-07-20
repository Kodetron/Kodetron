/*
 * Example integration of DatabaseService in MainWindow
 * Add this code to your MainWindow.h and MainWindow.cpp files
 */

// In MainWindow.h:
/*
#include "DataBase/DatabaseService.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onThemeChanged(const QString& themeName);
    void onDatabaseError(const QString& error);
    void saveCurrentTheme();
    void loadTheme();

private:
    void setupDatabaseConnections();
    void populateThemeMenu();
    
    DatabaseService* m_databaseService;
    QMenu* m_themeMenu;
    QAction* m_saveThemeAction;
};
*/

// In MainWindow.cpp:
/*
#include "MainWindow.h"
#include <QMenuBar>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_databaseService(new DatabaseService(this))
{
    setupDatabaseConnections();
    
    // Create theme menu
    m_themeMenu = menuBar()->addMenu("Themes");
    
    m_saveThemeAction = m_themeMenu->addAction("Save Current Theme...");
    connect(m_saveThemeAction, &QAction::triggered, this, &MainWindow::saveCurrentTheme);
    
    m_themeMenu->addSeparator();
    populateThemeMenu();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupDatabaseConnections()
{
    connect(m_databaseService, &DatabaseService::themeChanged,
            this, &MainWindow::onThemeChanged);
    
    connect(m_databaseService, &DatabaseService::databaseError,
            this, &MainWindow::onDatabaseError);
}

void MainWindow::onThemeChanged(const QString& themeName)
{
    qDebug() << "Theme changed to:" << themeName;
    populateThemeMenu(); // Refresh menu
}

void MainWindow::onDatabaseError(const QString& error)
{
    QMessageBox::warning(this, "Database Error", error);
}

void MainWindow::saveCurrentTheme()
{
    bool ok;
    QString themeName = QInputDialog::getText(this, "Save Theme",
                                            "Theme name:", QLineEdit::Normal,
                                            "", &ok);
    
    if (ok && !themeName.isEmpty()) {
        QString currentStyleSheet = qApp->styleSheet();
        
        if (m_databaseService->saveTheme(themeName, currentStyleSheet)) {
            QMessageBox::information(this, "Success", "Theme saved successfully!");
        } else {
            QMessageBox::warning(this, "Error", 
                               "Failed to save theme: " + m_databaseService->getLastError());
        }
    }
}

void MainWindow::loadTheme()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        QString themeName = action->text();
        
        if (m_databaseService->applyTheme(themeName)) {
            qDebug() << "Applied theme:" << themeName;
        } else {
            QMessageBox::warning(this, "Error", 
                               "Failed to load theme: " + m_databaseService->getLastError());
        }
    }
}

void MainWindow::populateThemeMenu()
{
    // Remove old theme actions (keep save action and separator)
    QList<QAction*> actions = m_themeMenu->actions();
    for (int i = actions.size() - 1; i >= 2; --i) {
        m_themeMenu->removeAction(actions[i]);
        delete actions[i];
    }
    
    // Add user themes
    QList<Theme> themes = m_databaseService->getUserThemes();
    for (const Theme& theme : themes) {
        QAction* themeAction = m_themeMenu->addAction(theme.name);
        connect(themeAction, &QAction::triggered, this, &MainWindow::loadTheme);
    }
}
*/

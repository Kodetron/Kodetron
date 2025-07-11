#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

#include <QObject>
#include <QMainWindow>
#include <QDockWidget>
#include <QToolButton>
#include <QSplitter>
#include <QWidget>
#include <QVBoxLayout>
#include <QIcon>
#include <QSize>
#include <QStyle>

class ToolbarManager : public QObject
{
    Q_OBJECT

public:
    explicit ToolbarManager(QMainWindow *parent,
                           QWidget *outputContainer,
                           QWidget *examplesContainer,
                           QSplitter *inputOutputSplitter,
                           QSplitter *outputHorizontalSplitter,
                           QSplitter *editorIoSplitter);

    ~ToolbarManager();

private slots:
    void toggleOutputExamples();

private:
    void setupToolbar();
    void setupToggleButton();

    // UI Components
    QMainWindow *m_mainWindow;
    QDockWidget *m_sidebarDockWidget;
    QWidget *m_sidebarContentWidget;
    QVBoxLayout *m_sidebarLayout;
    QToolButton *m_toggleOutputExamplesButton;

    // Editor containers and splitters
    QWidget *m_outputContainer;
    QWidget *m_examplesContainer;
    QSplitter *m_inputOutputSplitter;
    QSplitter *m_outputHorizontalSplitter;
    QSplitter *m_editorIoSplitter;

    // State tracking
    bool m_examplesVisible;
    
    // Icon path - you can modify this
    static const QString TOGGLE_ICON_PATH;
};

#endif
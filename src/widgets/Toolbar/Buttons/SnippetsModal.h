#ifndef SNIPPETSMODAL_H
#define SNIPPETSMODAL_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QSplitter>
#include <vector>
#include "..\Database\DataBaseManager.h"

class SnippetsModal : public QDialog {
    Q_OBJECT

public:
    explicit SnippetsModal(DatabaseManager* dbManager, int userId, QWidget* parent = nullptr);
    ~SnippetsModal();

private slots:
    void onSnippetSelectionChanged();
    void onCreateSnippet();
    void onUpdateSnippet();
    void onDeleteSnippet();
    void onSnippetNameChanged();
    void onSnippetContentChanged();

private:
    void setupUI();
    void loadSnippets();
    void clearEditor();
    void updateButtonStates();
    void saveCurrentSnippet();
    bool validateSnippetData();

    // UI Components
    QVBoxLayout* mainLayout;
    QHBoxLayout* buttonLayout;
    QSplitter* splitter;
    
    QListWidget* snippetsList;
    QVBoxLayout* editorLayout;
    QLineEdit* snippetNameEdit;
    QTextEdit* snippetContentEdit;
    
    QPushButton* createButton;
    QPushButton* updateButton;
    QPushButton* deleteButton;
    QPushButton* closeButton;
    
    // Data
    DatabaseManager* dbManager;
    int currentUserId;
    std::vector<Snippet> snippets;
    int currentSnippetIndex;
    bool hasUnsavedChanges;
    
    // Labels
    QLabel* nameLabel;
    QLabel* contentLabel;
};

#endif
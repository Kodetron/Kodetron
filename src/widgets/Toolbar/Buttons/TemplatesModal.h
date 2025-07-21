#ifndef TEMPLATESMODAL_H
#define TEMPLATESMODAL_H

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

class TemplatesModal : public QDialog {
    Q_OBJECT

public:
    explicit TemplatesModal(DatabaseManager* dbManager, int userId, QWidget* parent = nullptr);
    ~TemplatesModal();

private slots:
    void onTemplateSelectionChanged();
    void onCreateTemplate();
    void onUpdateTemplate();
    void onDeleteTemplate();
    void onTemplateNameChanged();
    void onTemplateContentChanged();

private:
    void setupUI();
    void loadTemplates();
    void clearEditor();
    void updateButtonStates();
    void saveCurrentTemplate();
    bool validateTemplateData();

    // UI Components
    QVBoxLayout* mainLayout;
    QHBoxLayout* buttonLayout;
    QSplitter* splitter;
    
    QListWidget* templatesList;
    QVBoxLayout* editorLayout;
    QLineEdit* templateNameEdit;
    QTextEdit* templateContentEdit;
    
    QPushButton* createButton;
    QPushButton* updateButton;
    QPushButton* deleteButton;
    QPushButton* closeButton;
    
    // Data
    DatabaseManager* dbManager;
    int currentUserId;
    std::vector<Template> templates;
    int currentTemplateIndex;
    bool hasUnsavedChanges;
    
    // Labels
    QLabel* nameLabel;
    QLabel* contentLabel;
};

#endif // TEMPLATESMODAL_H
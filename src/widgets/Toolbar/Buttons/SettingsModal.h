#ifndef SETTINGSMODAL_H
#define SETTINGSMODAL_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QGroupBox>
#include "..\Database\DataBaseManager.h"

class SettingsModal : public QDialog {
    Q_OBJECT

public:
    explicit SettingsModal(DatabaseManager* dbManager, int userId, QWidget* parent = nullptr);
    ~SettingsModal();

private slots:
    void onSaveSettings();
    void onResetSettings();
    void onFieldChanged();

private:
    void setupUI();
    void loadUserData();
    void updateButtonStates();
    bool validateUserData();
    void applyChanges();

    // UI Components
    QVBoxLayout* mainLayout;
    QFormLayout* formLayout;
    QHBoxLayout* buttonLayout;
    QGroupBox* userInfoGroup;
    
    QLineEdit* nameEdit;
    QLineEdit* emailEdit;
    QLineEdit* codeforcesHandleEdit;
    
    QPushButton* saveButton;
    QPushButton* resetButton;
    QPushButton* closeButton;
    
    // Data
    DatabaseManager* dbManager;
    int currentUserId;
    User originalUser;
    bool hasUnsavedChanges;
    
    // Labels
    QLabel* infoLabel;
};

#endif
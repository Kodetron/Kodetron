#include "SettingsModal.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

SettingsModal::SettingsModal(DatabaseManager* dbManager, int userId, QWidget* parent)
    : QDialog(parent), dbManager(dbManager), currentUserId(userId), hasUnsavedChanges(false) {
    setWindowTitle("User Settings");
    setModal(true);
    setFixedSize(400, 300);
    
    setupUI();
    loadUserData();
    updateButtonStates();
}

SettingsModal::~SettingsModal() = default;

void SettingsModal::setupUI() {
    mainLayout = new QVBoxLayout(this);
    
    // Info label
    infoLabel = new QLabel("Update your profile information:");
    infoLabel->setStyleSheet("font-weight: bold; margin-bottom: 10px;");
    mainLayout->addWidget(infoLabel);
    
    // User information group
    userInfoGroup = new QGroupBox("User Information");
    formLayout = new QFormLayout(userInfoGroup);
    
    // Name field
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Enter your name...");
    connect(nameEdit, &QLineEdit::textChanged, this, &SettingsModal::onFieldChanged);
    formLayout->addRow("Name:", nameEdit);
    
    // Email field
    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("Enter your email...");
    
    // Email validation
    QRegularExpression emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    QRegularExpressionValidator* emailValidator = new QRegularExpressionValidator(emailRegex, this);
    emailEdit->setValidator(emailValidator);
    
    connect(emailEdit, &QLineEdit::textChanged, this, &SettingsModal::onFieldChanged);
    formLayout->addRow("Email:", emailEdit);
    
    // Codeforces handle field
    codeforcesHandleEdit = new QLineEdit();
    codeforcesHandleEdit->setPlaceholderText("Enter your Codeforces handle...");
    
    // Codeforces handle validation (alphanumeric and underscore only)
    QRegularExpression handleRegex(R"([a-zA-Z0-9_]{1,24})");
    QRegularExpressionValidator* handleValidator = new QRegularExpressionValidator(handleRegex, this);
    codeforcesHandleEdit->setValidator(handleValidator);
    
    connect(codeforcesHandleEdit, &QLineEdit::textChanged, this, &SettingsModal::onFieldChanged);
    formLayout->addRow("Codeforces Handle:", codeforcesHandleEdit);
    
    mainLayout->addWidget(userInfoGroup);
    
    // Buttons
    buttonLayout = new QHBoxLayout();
    
    saveButton = new QPushButton("Save Changes");
    resetButton = new QPushButton("Reset");
    closeButton = new QPushButton("Close");
    
    saveButton->setDefault(true);
    saveButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    
    connect(saveButton, &QPushButton::clicked, this, &SettingsModal::onSaveSettings);
    connect(resetButton, &QPushButton::clicked, this, &SettingsModal::onResetSettings);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::close);
    
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addLayout(buttonLayout);
}

void SettingsModal::loadUserData() {
    if (dbManager->getUserById(currentUserId, originalUser)) {
        nameEdit->setText(QString::fromStdString(originalUser.email)); // Assuming name is stored in email field for now
        emailEdit->setText(QString::fromStdString(originalUser.email));
        codeforcesHandleEdit->setText(QString::fromStdString(originalUser.codeforces_handle));
        
        hasUnsavedChanges = false;
    } else {
        QMessageBox::warning(this, "Error", "Failed to load user data.");
        close();
    }
}

void SettingsModal::onSaveSettings() {
    if (!validateUserData()) {
        return;
    }
    
    // Show confirmation dialog
    int reply = QMessageBox::question(this, "Confirm Changes", 
        "Are you sure you want to save these changes?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        applyChanges();
    }
}

void SettingsModal::onResetSettings() {
    if (hasUnsavedChanges) {
        int reply = QMessageBox::question(this, "Reset Changes", 
            "Are you sure you want to reset all changes? Unsaved changes will be lost.",
            QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::No) {
            return;
        }
    }
    
    loadUserData();
    updateButtonStates();
}

void SettingsModal::onFieldChanged() {
    hasUnsavedChanges = true;
    updateButtonStates();
}

void SettingsModal::updateButtonStates() {
    bool hasValidData = validateUserData();
    
    saveButton->setEnabled(hasUnsavedChanges && hasValidData);
    resetButton->setEnabled(hasUnsavedChanges);
}

bool SettingsModal::validateUserData() {
    // Check if email is valid (if not empty)
    if (!emailEdit->text().isEmpty() && !emailEdit->hasAcceptableInput()) {
        return false;
    }
    
    // Check if codeforces handle is valid (if not empty)
    if (!codeforcesHandleEdit->text().isEmpty() && !codeforcesHandleEdit->hasAcceptableInput()) {
        return false;
    }
    
    // At least one field should be filled
    return !nameEdit->text().trimmed().isEmpty() || 
           !emailEdit->text().trimmed().isEmpty() || 
           !codeforcesHandleEdit->text().trimmed().isEmpty();
}

void SettingsModal::applyChanges() {
    User updatedUser = originalUser;
    
    // Update fields (assuming name is stored in the email field for now - you might want to add a name field to the database)
    updatedUser.email = emailEdit->text().toStdString();
    updatedUser.codeforces_handle = codeforcesHandleEdit->text().toStdString();
    
    if (dbManager->updateUser(updatedUser)) {
        QMessageBox::information(this, "Success", "Settings saved successfully!");
        originalUser = updatedUser;
        hasUnsavedChanges = false;
        updateButtonStates();
    } else {
        QMessageBox::warning(this, "Error", "Failed to save settings. Please try again.");
    }
}
#include "TemplatesModal.h"

TemplatesModal::TemplatesModal(DatabaseManager* dbManager, int userId, QWidget* parent)
    : QDialog(parent), dbManager(dbManager), currentUserId(userId), 
      currentTemplateIndex(-1), hasUnsavedChanges(false) {
    setWindowTitle("Templates Manager");
    setModal(true);
    resize(800, 600);
    
    setupUI();
    loadTemplates();
    updateButtonStates();
}

TemplatesModal::~TemplatesModal() = default;

void TemplatesModal::setupUI() {
    mainLayout = new QVBoxLayout(this);
    
    // Create splitter for list and editor
    splitter = new QSplitter(Qt::Horizontal, this);
    
    // Left side - Templates list
    templatesList = new QListWidget();
    templatesList->setMinimumWidth(250);
    templatesList->setMaximumWidth(350);
    connect(templatesList, &QListWidget::currentRowChanged, this, &TemplatesModal::onTemplateSelectionChanged);
    splitter->addWidget(templatesList);
    
    // Right side - Editor
    QWidget* editorWidget = new QWidget();
    editorLayout = new QVBoxLayout(editorWidget);
    
    // Name editor
    nameLabel = new QLabel("Template Name:");
    templateNameEdit = new QLineEdit();
    templateNameEdit->setPlaceholderText("Enter template name...");
    connect(templateNameEdit, &QLineEdit::textChanged, this, &TemplatesModal::onTemplateNameChanged);
    
    // Content editor
    contentLabel = new QLabel("Template Content:");
    templateContentEdit = new QTextEdit();
    templateContentEdit->setPlaceholderText("Enter template content...");
    connect(templateContentEdit, &QTextEdit::textChanged, this, &TemplatesModal::onTemplateContentChanged);
    
    editorLayout->addWidget(nameLabel);
    editorLayout->addWidget(templateNameEdit);
    editorLayout->addWidget(contentLabel);
    editorLayout->addWidget(templateContentEdit);
    
    splitter->addWidget(editorWidget);
    splitter->setSizes({300, 500});
    
    mainLayout->addWidget(splitter);
    
    // Buttons
    buttonLayout = new QHBoxLayout();
    
    createButton = new QPushButton("Create New");
    updateButton = new QPushButton("Update");
    deleteButton = new QPushButton("Delete");
    closeButton = new QPushButton("Close");
    
    connect(createButton, &QPushButton::clicked, this, &TemplatesModal::onCreateTemplate);
    connect(updateButton, &QPushButton::clicked, this, &TemplatesModal::onUpdateTemplate);
    connect(deleteButton, &QPushButton::clicked, this, &TemplatesModal::onDeleteTemplate);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::close);
    
    buttonLayout->addWidget(createButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addLayout(buttonLayout);
}

void TemplatesModal::loadTemplates() {
    templatesList->clear();
    templates = dbManager->getTemplatesByUserId(currentUserId);
    
    for (const auto& template_obj : templates) {
        templatesList->addItem(QString::fromStdString(template_obj.name));
    }
    
    clearEditor();
}

void TemplatesModal::onTemplateSelectionChanged() {
    int currentRow = templatesList->currentRow();
    
    if (currentRow >= 0 && currentRow < static_cast<int>(templates.size())) {
        currentTemplateIndex = currentRow;
        const Template& template_obj = templates[currentTemplateIndex];
        
        templateNameEdit->setText(QString::fromStdString(template_obj.name));
        templateContentEdit->setPlainText(QString::fromStdString(template_obj.content));
        
        hasUnsavedChanges = false;
    } else {
        currentTemplateIndex = -1;
        clearEditor();
    }
    
    updateButtonStates();
}

void TemplatesModal::onCreateTemplate() {
    if (!validateTemplateData()) {
        return;
    }
    
    std::string name = templateNameEdit->text().toStdString();
    std::string content = templateContentEdit->toPlainText().toStdString();
    
    if (dbManager->createTemplate(name, content, currentUserId)) {
        QMessageBox::information(this, "Success", "Template created successfully!");
        loadTemplates();
        clearEditor();
    } else {
        QMessageBox::warning(this, "Error", "Failed to create template.");
    }
}

void TemplatesModal::onUpdateTemplate() {
    if (currentTemplateIndex < 0 || !validateTemplateData()) {
        return;
    }
    
    Template updatedTemplate = templates[currentTemplateIndex];
    updatedTemplate.name = templateNameEdit->text().toStdString();
    updatedTemplate.content = templateContentEdit->toPlainText().toStdString();
    
    if (dbManager->updateTemplate(updatedTemplate)) {
        QMessageBox::information(this, "Success", "Template updated successfully!");
        loadTemplates();
        hasUnsavedChanges = false;
    } else {
        QMessageBox::warning(this, "Error", "Failed to update template.");
    }
}

void TemplatesModal::onDeleteTemplate() {
    if (currentTemplateIndex < 0) {
        return;
    }
    
    const Template& template_obj = templates[currentTemplateIndex];
    
    int reply = QMessageBox::question(this, "Confirm Delete", 
        QString("Are you sure you want to delete the template '%1'?")
        .arg(QString::fromStdString(template_obj.name)),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (dbManager->deleteTemplate(template_obj.id)) {
            QMessageBox::information(this, "Success", "Template deleted successfully!");
            loadTemplates();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete template.");
        }
    }
}

void TemplatesModal::onTemplateNameChanged() {
    hasUnsavedChanges = true;
    updateButtonStates();
}

void TemplatesModal::onTemplateContentChanged() {
    hasUnsavedChanges = true;
    updateButtonStates();
}

void TemplatesModal::clearEditor() {
    templateNameEdit->clear();
    templateContentEdit->clear();
    hasUnsavedChanges = false;
}

void TemplatesModal::updateButtonStates() {
    bool hasSelection = currentTemplateIndex >= 0;
    bool hasContent = !templateNameEdit->text().isEmpty() && !templateContentEdit->toPlainText().isEmpty();
    
    createButton->setEnabled(hasContent);
    updateButton->setEnabled(hasSelection && hasContent && hasUnsavedChanges);
    deleteButton->setEnabled(hasSelection);
}

bool TemplatesModal::validateTemplateData() {
    if (templateNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a template name.");
        templateNameEdit->setFocus();
        return false;
    }
    
    if (templateContentEdit->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter template content.");
        templateContentEdit->setFocus();
        return false;
    }
    
    return true;
}
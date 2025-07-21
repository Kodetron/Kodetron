#include "SnippetsModal.h"

SnippetsModal::SnippetsModal(DatabaseManager* dbManager, int userId, QWidget* parent)
    : QDialog(parent), dbManager(dbManager), currentUserId(userId), 
      currentSnippetIndex(-1), hasUnsavedChanges(false) {
    setWindowTitle("Snippets Manager");
    setModal(true);
    resize(800, 600);
    
    setupUI();
    loadSnippets();
    updateButtonStates();
}

SnippetsModal::~SnippetsModal() = default;

void SnippetsModal::setupUI() {
    mainLayout = new QVBoxLayout(this);
    
    // Create splitter for list and editor
    splitter = new QSplitter(Qt::Horizontal, this);
    
    // Left side - Snippets list
    snippetsList = new QListWidget();
    snippetsList->setMinimumWidth(250);
    snippetsList->setMaximumWidth(350);
    connect(snippetsList, &QListWidget::currentRowChanged, this, &SnippetsModal::onSnippetSelectionChanged);
    splitter->addWidget(snippetsList);
    
    // Right side - Editor
    QWidget* editorWidget = new QWidget();
    editorLayout = new QVBoxLayout(editorWidget);
    
    // Name editor
    nameLabel = new QLabel("Snippet Name:");
    snippetNameEdit = new QLineEdit();
    snippetNameEdit->setPlaceholderText("Enter snippet name...");
    connect(snippetNameEdit, &QLineEdit::textChanged, this, &SnippetsModal::onSnippetNameChanged);
    
    // Content editor
    contentLabel = new QLabel("Snippet Content:");
    snippetContentEdit = new QTextEdit();
    snippetContentEdit->setPlaceholderText("Enter snippet content...");
    connect(snippetContentEdit, &QTextEdit::textChanged, this, &SnippetsModal::onSnippetContentChanged);
    
    editorLayout->addWidget(nameLabel);
    editorLayout->addWidget(snippetNameEdit);
    editorLayout->addWidget(contentLabel);
    editorLayout->addWidget(snippetContentEdit);
    
    splitter->addWidget(editorWidget);
    splitter->setSizes({300, 500});
    
    mainLayout->addWidget(splitter);
    
    // Buttons
    buttonLayout = new QHBoxLayout();
    
    createButton = new QPushButton("Create New");
    updateButton = new QPushButton("Update");
    deleteButton = new QPushButton("Delete");
    closeButton = new QPushButton("Close");
    
    connect(createButton, &QPushButton::clicked, this, &SnippetsModal::onCreateSnippet);
    connect(updateButton, &QPushButton::clicked, this, &SnippetsModal::onUpdateSnippet);
    connect(deleteButton, &QPushButton::clicked, this, &SnippetsModal::onDeleteSnippet);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::close);
    
    buttonLayout->addWidget(createButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addLayout(buttonLayout);
}

void SnippetsModal::loadSnippets() {
    snippetsList->clear();
    snippets = dbManager->getSnippetsByUserId(currentUserId);
    
    for (const auto& snippet : snippets) {
        snippetsList->addItem(QString::fromStdString(snippet.name));
    }
    
    clearEditor();
}

void SnippetsModal::onSnippetSelectionChanged() {
    int currentRow = snippetsList->currentRow();
    
    if (currentRow >= 0 && currentRow < static_cast<int>(snippets.size())) {
        currentSnippetIndex = currentRow;
        const Snippet& snippet = snippets[currentSnippetIndex];
        
        snippetNameEdit->setText(QString::fromStdString(snippet.name));
        snippetContentEdit->setPlainText(QString::fromStdString(snippet.content));
        
        hasUnsavedChanges = false;
    } else {
        currentSnippetIndex = -1;
        clearEditor();
    }
    
    updateButtonStates();
}

void SnippetsModal::onCreateSnippet() {
    if (!validateSnippetData()) {
        return;
    }
    
    std::string name = snippetNameEdit->text().toStdString();
    std::string content = snippetContentEdit->toPlainText().toStdString();
    
    if (dbManager->createSnippet(name, content, currentUserId)) {
        QMessageBox::information(this, "Success", "Snippet created successfully!");
        loadSnippets();
        clearEditor();
    } else {
        QMessageBox::warning(this, "Error", "Failed to create snippet. Name might already exist.");
    }
}

void SnippetsModal::onUpdateSnippet() {
    if (currentSnippetIndex < 0 || !validateSnippetData()) {
        return;
    }
    
    Snippet updatedSnippet = snippets[currentSnippetIndex];
    updatedSnippet.name = snippetNameEdit->text().toStdString();
    updatedSnippet.content = snippetContentEdit->toPlainText().toStdString();
    
    if (dbManager->updateSnippet(updatedSnippet)) {
        QMessageBox::information(this, "Success", "Snippet updated successfully!");
        loadSnippets();
        hasUnsavedChanges = false;
    } else {
        QMessageBox::warning(this, "Error", "Failed to update snippet.");
    }
}

void SnippetsModal::onDeleteSnippet() {
    if (currentSnippetIndex < 0) {
        return;
    }
    
    const Snippet& snippet = snippets[currentSnippetIndex];
    
    int reply = QMessageBox::question(this, "Confirm Delete", 
        QString("Are you sure you want to delete the snippet '%1'?")
        .arg(QString::fromStdString(snippet.name)),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (dbManager->deleteSnippet(snippet.id)) {
            QMessageBox::information(this, "Success", "Snippet deleted successfully!");
            loadSnippets();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete snippet.");
        }
    }
}

void SnippetsModal::onSnippetNameChanged() {
    hasUnsavedChanges = true;
    updateButtonStates();
}

void SnippetsModal::onSnippetContentChanged() {
    hasUnsavedChanges = true;
    updateButtonStates();
}

void SnippetsModal::clearEditor() {
    snippetNameEdit->clear();
    snippetContentEdit->clear();
    hasUnsavedChanges = false;
}

void SnippetsModal::updateButtonStates() {
    bool hasSelection = currentSnippetIndex >= 0;
    bool hasContent = !snippetNameEdit->text().isEmpty() && !snippetContentEdit->toPlainText().isEmpty();
    
    createButton->setEnabled(hasContent);
    updateButton->setEnabled(hasSelection && hasContent && hasUnsavedChanges);
    deleteButton->setEnabled(hasSelection);
}

bool SnippetsModal::validateSnippetData() {
    if (snippetNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a snippet name.");
        snippetNameEdit->setFocus();
        return false;
    }
    
    if (snippetContentEdit->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter snippet content.");
        snippetContentEdit->setFocus();
        return false;
    }
    
    return true;
}
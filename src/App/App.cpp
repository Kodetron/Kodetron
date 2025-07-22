#include "App.h"
#include "../utils/StyleLoader/StyleReader.h"

#include <iostream>

App::App(QWidget *parent) : QWidget(parent) {
    // Database initialization
    db_manager = new DatabaseManager();
    int user_id = 1;
    db_manager->initializeDatabase();
    User user;
    if (!db_manager->getUserById(user_id, user)) {
        db_manager->createUser("default_handle", "default@email.com");
    }

    // Childs initialization
    menu_section = new MenuSection(this);
    toolbar_section = new ToolbarSection(db_manager, user_id, this);
    explorer_section = new ExplorerSection(this);
    editor_section = new EditorSection(this);
    standardio_section = new StandardIOSection(editor_section->getCodeEditor(), this);
    content_wrapper = new QWidget(this); // content = all - menu_section

    // Splitter
    file_editor_standardio_splitter = new QSplitter(Qt::Horizontal, this);
    file_editor_standardio_splitter->addWidget(explorer_section);
    file_editor_standardio_splitter->addWidget(editor_section);
    file_editor_standardio_splitter->addWidget(standardio_section);

    // Layouts
    horizontal_layout = new QHBoxLayout(content_wrapper);
    horizontal_layout->addWidget(toolbar_section);
    horizontal_layout->addWidget(file_editor_standardio_splitter);
    content_wrapper->setLayout(horizontal_layout);
    
    vertical_layout = new QVBoxLayout(this);
    vertical_layout->addWidget(menu_section);
    vertical_layout->addWidget(content_wrapper);
    setLayout(vertical_layout);

    // Style sheet
    setAttribute(Qt::WA_StyledBackground, true);
    assignObjectNames();
    applyQtStyles();
    loadStyleSheet();
}
void App::assignObjectNames() {
    file_editor_standardio_splitter->setObjectName("file_editor_standardio_splitter");
}
void App::applyQtStyles() {
    file_editor_standardio_splitter->setSizes({300, 1010, 400});
    file_editor_standardio_splitter->setCollapsible(0, false);
    file_editor_standardio_splitter->setCollapsible(1, false);
    horizontal_layout->setContentsMargins(0, 0, 0, 0);
    vertical_layout->setContentsMargins(0, 0, 0, 0);
    vertical_layout->setSpacing(0);
    horizontal_layout->setSpacing(0);
}
void App::loadStyleSheet() {
    QString styleSheet = StyleLoader::read("../src/App/App.qss");
    if (!styleSheet.isEmpty()) {
        setStyleSheet(styleSheet);
    }
}

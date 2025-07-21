#include "App.h"
#include "../utils/StyleLoader/StyleReader.h"

#include <iostream>

App::App(QWidget *parent) : QWidget(parent) {
    // Childs initialization
    menu_section = new MenuSection(this);
    toolbar_section = new ToolbarSection(this);
    explorer_section = new QWidget(this);
    editor_section = new QWidget(this);
    standardio_section = new QWidget(this);
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
    applyQtStyles();
    loadStyleSheet();
}
void App::applyQtStyles() {
    file_editor_standardio_splitter->setSizes({25, 45, 20});
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

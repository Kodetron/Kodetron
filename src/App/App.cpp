#include "App.h"
#include "../utils/StyleLoader/StyleReader.h"

#include <iostream>

App::App(QWidget *parent) : QWidget(parent) {
    // Childs initialization
    menu_section = new QWidget(this);
    toolbar_section = new QWidget(this);
    explorer_section = new QWidget(this);
    editor_section = new QWidget(this);
    standardio_section = new QWidget(this);

    // Splitter
    file_editor_standardio_splitter = new QSplitter(Qt::Horizontal, this);
    file_editor_standardio_splitter->addWidget(toolbar_section);
    file_editor_standardio_splitter->addWidget(explorer_section);
    file_editor_standardio_splitter->addWidget(editor_section);
    file_editor_standardio_splitter->addWidget(standardio_section);

    // Layout
    layout = new QVBoxLayout(this);
    layout->addWidget(file_editor_standardio_splitter);
    setLayout(layout);

    // Style sheet
    loadStyleSheet();
}

void App::loadStyleSheet() {
    QString styleSheet = StyleLoader::read("../src/App/App.qss");
    if (!styleSheet.isEmpty()) {
        setStyleSheet(styleSheet);
    }
}

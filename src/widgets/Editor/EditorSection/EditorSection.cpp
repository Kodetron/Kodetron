#include "EditorSection.h"
#include "../utils/StyleLoader/StyleReader.h"

EditorSection::EditorSection(QWidget *parent) : QWidget(parent) {
    // Childs initialization
    files_container = new QWidget(this);
    code_editor = new QWidget(this);

    // Layout
    layout = new QVBoxLayout(this);
    layout->addWidget(files_container);
    layout->addWidget(code_editor);
    setLayout(layout);

    // Styles
    setAttribute(Qt::WA_StyledBackground, true);
    assignObjectNames();
    applyQtStyles();
    loadStyleSheet();
}
void EditorSection::assignObjectNames() {
    setObjectName("editor_section");
}
void EditorSection::applyQtStyles() {
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(20);
    files_container->setStyleSheet("background-color: #050505; border: none; border-radius: 4px; max-height: 40px;");
    code_editor->setStyleSheet("background-color: #050505; border: none; border-radius: 4px;");
}
void EditorSection::loadStyleSheet() {
    QString styleSheet = StyleLoader::read("../src/widgets/Editor/EditorSection/EditorSection.qss");
    if (!styleSheet.isEmpty()) {
        setStyleSheet(styleSheet);
    }
}

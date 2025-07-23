#include "EditorSection.h"
#include "../utils/StyleLoader/StyleReader.h"

EditorSection::EditorSection(QWidget *parent) : QWidget(parent) {
    // Childs initialization
    code_editor = new KodetronEditor(this);

    // Layout
    layout = new QVBoxLayout(this);
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
    code_editor->setStyleSheet("border: none;");
}
void EditorSection::loadStyleSheet() {
    QString styleSheet = StyleLoader::read("../src/widgets/Editor/EditorSection/EditorSection.qss");
    if (!styleSheet.isEmpty()) {
        setStyleSheet(styleSheet);
    }
}

#include "StandardIOSection.h"
#include "../utils/StyleLoader/StyleReader.h"

StandardIOSection::StandardIOSection(QWidget *parent) : QWidget(parent) {
    // Childs initialization
    execution_options_container = new ExecutionOptionsContainer(this);
    input_text_box = new QTextEdit(this);
    output_text_box = new QTextEdit(this); 
    output_text_box->setReadOnly(true);

    // Layout
    layout = new QVBoxLayout(this);
    layout->addWidget(execution_options_container);
    layout->addWidget(input_text_box);
    layout->addWidget(output_text_box);
    setLayout(layout);

    // Styles
    setAttribute(Qt::WA_StyledBackground, true);
    assignObjectNames();
    applyQtStyles();
    loadStyleSheet();
}
void StandardIOSection::assignObjectNames() {
    setObjectName("standard_io_section");
    input_text_box->setObjectName("input_text_box");
    output_text_box->setObjectName("output_text_box");
}
void StandardIOSection::applyQtStyles() {
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(20);
}
void StandardIOSection::loadStyleSheet() {
    QString styleSheet = StyleLoader::read("../src/widgets/StandardIO/StandardIOSection/StandardIOSection.qss");
    if (!styleSheet.isEmpty()) {
        setStyleSheet(styleSheet);
    }
}

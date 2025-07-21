#include "ExecutionOptionsContainer.h"
#include "../utils/StyleLoader/StyleReader.h"

ExecutionOptionsContainer::ExecutionOptionsContainer(QWidget *parent) : QWidget(parent) {
    // Initialize buttons and labels
    run_button = new QPushButton("Run", this);
    run_in_terminal_label = new QLabel("Run in Terminal:", this);
    run_in_terminal_checkbox = new QCheckBox(this);

    // Layout for the execution options
    layout = new QHBoxLayout(this);
    layout->addWidget(run_button, 1);
    layout->addStretch();
    layout->addWidget(run_in_terminal_label);
    layout->addWidget(run_in_terminal_checkbox);
    setLayout(layout);

    // Styles
    setAttribute(Qt::WA_StyledBackground, true);
    assignObjectNames();
    applyQtStyles();
    loadStyleSheet();
}
void ExecutionOptionsContainer::assignObjectNames() {
    setObjectName("execution_options_container");
    run_button->setObjectName("run_button");
}
void ExecutionOptionsContainer::applyQtStyles() {
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);
    run_button->setCursor(Qt::PointingHandCursor);
    run_in_terminal_checkbox->setCursor(Qt::PointingHandCursor);
}
void ExecutionOptionsContainer::loadStyleSheet() {
    QString styleSheet = StyleLoader::read("../src/widgets/StandardIO/ExecutionOptionsContainer/ExecutionOptionsContainer.qss");
    if (!styleSheet.isEmpty()) {
        setStyleSheet(styleSheet);
    }
}

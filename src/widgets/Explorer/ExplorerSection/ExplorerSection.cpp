#include "ExplorerSection.h"
#include "../utils/StyleLoader/StyleReader.h"

ExplorerSection::ExplorerSection(QWidget *parent) : QWidget(parent) {
    // Childs initialization
    explorer_card = new ExplorerCard(this);

    // Layout
    layout = new QVBoxLayout(this);
    layout->addWidget(explorer_card);
    setLayout(layout);

    // Styles
    setAttribute(Qt::WA_StyledBackground, true);
    assignObjectNames();
    applyQtStyles();
    loadStyleSheet();
}
void ExplorerSection::assignObjectNames() {
    setObjectName("explorer_section");
}
void ExplorerSection::applyQtStyles() {
    layout->setContentsMargins(0, 10, 10, 10);
    layout->setSpacing(0);
    explorer_card->setStyleSheet("background-color: #000000; border: none; border-radius: 4px;");
}
void ExplorerSection::loadStyleSheet() {
    QString styleSheet = StyleLoader::read("../src/widgets/Explorer/ExplorerSection/ExplorerSection.qss");
    if (!styleSheet.isEmpty()) {
        setStyleSheet(styleSheet);
    }
}

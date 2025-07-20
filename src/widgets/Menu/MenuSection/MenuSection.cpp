#include "MenuSection.h"
#include "../../../utils/StyleLoader/StyleReader.h"

MenuSection::MenuSection(QWidget *parent) : QWidget(parent) {
    file_button = new QPushButton("File", this);
    file_menu = new QMenu(this);

    // Add actions to the File menu
    file_menu->addAction("Open file");
    file_menu->addAction("New file");
    file_menu->addAction("Save file");
    file_menu->addAction("Open folder");
    file_menu->addAction("New foler");

    file_button->setMenu(file_menu);
    file_button->setCursor(Qt::PointingHandCursor);

    // Layout
    layout = new QVBoxLayout(this);
    setLayout(layout);

    // Styles
    setAttribute(Qt::WA_StyledBackground, true);
    assignObjectNames();
    applyQtStyles();
    loadStyleSheet();
}
void MenuSection::assignObjectNames() {
    setObjectName("menu_section");
}
void MenuSection::applyQtStyles() {
    layout->addWidget(file_button, 0, Qt::AlignVCenter | Qt::AlignLeft);
}
void MenuSection::loadStyleSheet() {
    QString styleSheet = StyleLoader::read("../src/widgets/Menu/MenuSection/MenuSection.qss");
    if (!styleSheet.isEmpty()) {
        setStyleSheet(styleSheet);
    }
}

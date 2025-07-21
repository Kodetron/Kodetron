#include "MenuSection.h"
#include "../../../utils/StyleLoader/StyleReader.h"
#include "../../../FileSystemOperations/FileDialog/FileDialog.h"
#include "../../../Global/AppState.h"

MenuSection::MenuSection(QWidget *parent) : QWidget(parent) {
    file_button = new QPushButton("File", this);
    file_menu = new QMenu(this);

    // Add actions to the File menu
    open_file_action = file_menu->addAction("Open file");
    open_dir_action = file_menu->addAction("Open folder");
    file_menu->addAction("New file");
    file_menu->addAction("Save file");
    file_menu->addAction("New foler");

    // Connect actions to slots
    connect(open_file_action, &QAction::triggered, this, &MenuSection::onOpenFile);
    connect(open_dir_action, &QAction::triggered, this, &MenuSection::onOpenDir);

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
void MenuSection::onOpenDir() {
    QString dirPath = FileDialog::getOpenDirectoryPath(this);
    if (!dirPath.isEmpty()) {
        AppState::instance().setSelectedExplorerPath(dirPath, AppState::instance().enumExplorerPathType.dir_type);
    }
}
void MenuSection::onOpenFile() {
    QString filePath = FileDialog::getOpenFilePath(this);
    if (!filePath.isEmpty()) {
        AppState::instance().setSelectedExplorerPath(filePath, AppState::instance().enumExplorerPathType.file_type);
    }
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

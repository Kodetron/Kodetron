#include "ExplorerCard.h"
#include "../../../Global/AppState.h"
#include "../../../utils/StyleLoader/StyleReader.h"

ExplorerCard::ExplorerCard(QWidget *parent) : QWidget(parent) {
    // Models initialization
    file_system_model = new QFileSystemModel();
    single_file_model = new QStandardItemModel();
    
    // Tree initialization
    tree_view = new QTreeView();

    // Subscribe to the AppState signal to update the tree view when the selected path changes
    connect(&AppState::instance(), &AppState::selectedExplorerPathModified, this, &ExplorerCard::onSelectedExplorerPathModified);
    connect(tree_view, &QTreeView::clicked, this, &ExplorerCard::onTreeViewItemClicked);
    
    // Layout
    layout = new QVBoxLayout();
    layout->addWidget(tree_view);
    setLayout(layout);

    // Styles
    assignObjectNames();
    applyQtStyles();
    loadStyleSheet();
}

void ExplorerCard::renderDir(const QString &dir_path) {
    QModelIndex index = file_system_model->index(dir_path);
    if (index.isValid()) {
        file_system_model->setNameFilters(QStringList() << "*.cpp");
        file_system_model->setNameFilterDisables(false);
        tree_view->setModel(file_system_model);
        file_system_model->setRootPath(dir_path);
        tree_view->setRootIndex(file_system_model->index(dir_path));

        for (int i = 1; i < file_system_model->columnCount(); ++i) {
            tree_view->hideColumn(i);
        }
        AppState::instance().setSelectedFilePath(QString());
        AppState::instance().setSelectedDirPath(dir_path);
    }
}
void ExplorerCard::renderFile(const QString &file_path) {
    QFileInfo file_info(file_path);
    QFileIconProvider icon_provider;
    
    QStandardItem *item = new QStandardItem(icon_provider.icon(file_info), file_info.fileName());
    item->setData(file_path, Qt::UserRole);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);

    single_file_model->clear();
    single_file_model->appendRow(item);
    tree_view->setModel(single_file_model);
    tree_view->expandAll();

    for (int i = 1; i < file_system_model->columnCount(); ++i) {
        tree_view->hideColumn(i);
    }
    AppState::instance().setSelectedFilePath(file_path);
    AppState::instance().setSelectedDirPath(QString());
}

void ExplorerCard::onTreeViewItemClicked(const QModelIndex &index) {
    QString file_path;
    // Try to get file path from Qt::UserRole first (single_file_model)
    file_path = index.data(Qt::UserRole).toString();
    if (file_path.isEmpty()) {
        // If not present, try to get file path from QFileSystemModel (column 0)
        file_path = file_system_model->filePath(index);
    }
    if (!file_path.isEmpty()) {
        QFileInfo info(file_path);
        if (info.isFile()) {
            AppState::instance().setSelectedFilePath(file_path);
        }
    }
}

void ExplorerCard::onSelectedExplorerPathModified(const QString &new_path, const std::string &path_type) {  
    if (path_type == AppState::instance().enumExplorerPathType.dir_type) {
        renderDir(new_path);
    } 
    if (path_type == AppState::instance().enumExplorerPathType.file_type) {
        renderFile(new_path);
    }
}

void ExplorerCard::assignObjectNames() {}
void ExplorerCard::applyQtStyles() {
}
void ExplorerCard::loadStyleSheet() {
    QString styleSheet = StyleLoader::read("../src/widgets/Explorer/ExplorerCard/ExplorerCard.qss");
    if (!styleSheet.isEmpty()) {
        tree_view->setStyleSheet(styleSheet);
    }
}

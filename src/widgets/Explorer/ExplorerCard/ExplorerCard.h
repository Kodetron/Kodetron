#ifndef EXPLORERCARD_H
#define EXPLORERCARD_H

#include <QFileIconProvider>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>

class ExplorerCard : public QWidget {
    Q_OBJECT

  public:
    explicit ExplorerCard(QWidget *parent = nullptr);
    void onSelectedExplorerPathModified(const QString &new_path, const std::string &path_type);
    void renderDir(const QString &dir_path);
    void renderFile(const QString &file_path);
    void assignObjectNames();
    void applyQtStyles();
    void loadStyleSheet();

  private:
    QFileSystemModel *file_system_model;
    QStandardItemModel *single_file_model;
    QTreeView *tree_view;
    QVBoxLayout *layout;
};

#endif // EXPLORERCARD_H
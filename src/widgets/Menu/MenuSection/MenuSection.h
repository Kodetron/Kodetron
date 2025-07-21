#ifndef MENUSECTION_H
#define MENUSECTION_H

#include <QAction>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class MenuSection : public QWidget {
    Q_OBJECT
  public:
    MenuSection(QWidget *parent = nullptr);
    void onOpenFile();
    void onOpenDir();
    void assignObjectNames();
    void applyQtStyles();
    void loadStyleSheet();

  private:
    QPushButton *file_button;
    QMenu *file_menu;
    QVBoxLayout *layout;

    QAction *open_dir_action;
    QAction *open_file_action;
};

#endif // MENUSECTION_H
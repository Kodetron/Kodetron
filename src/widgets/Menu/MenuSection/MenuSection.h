#ifndef MENUSECTION_H
#define MENUSECTION_H

#include <QMenu>
#include <QpushButton>
#include <QVBoxLayout>
#include <QWidget>

class MenuSection : public QWidget {
    Q_OBJECT
  public:
    MenuSection(QWidget *parent = nullptr);
    void assignObjectNames();
    void applyQtStyles();
    void loadStyleSheet();

  private:
    QPushButton *file_button;
    QMenu *file_menu;
    QVBoxLayout *layout;
};

#endif // MENUSECTION_H
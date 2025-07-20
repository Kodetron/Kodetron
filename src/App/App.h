#ifndef APP_H
#define APP_H

#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include <QFile>
#include <QDir>
#include "../widgets/Menu/MenuSection/MenuSection.h"

class App : public QWidget {
    Q_OBJECT

  public:
    App(QWidget *parent = nullptr);
    void loadStyleSheet();
    void applyQtStyles();
    
  private:
    MenuSection *menu_section;
    QWidget *toolbar_section;
    QWidget *explorer_section;
    QWidget *editor_section;
    QWidget *standardio_section;
    QSplitter *file_editor_standardio_splitter;
    QVBoxLayout *layout;
};

#endif // APP_H
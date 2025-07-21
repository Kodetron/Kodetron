#ifndef APP_H
#define APP_H

#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFile>
#include <QDir>
#include "../widgets/Menu/MenuSection/MenuSection.h"
#include "../widgets/Toolbar/ToolbarSection/ToolbarSection.h"

class App : public QWidget {
    Q_OBJECT

  public:
    App(QWidget *parent = nullptr);
    void loadStyleSheet();
    void applyQtStyles();
    
  private:
    MenuSection *menu_section;
    ToolbarSection *toolbar_section;
    QWidget *explorer_section;
    QWidget *editor_section;
    QWidget *standardio_section;
    QWidget *content_wrapper; // content = all - menu_section
    QSplitter *file_editor_standardio_splitter;
    QVBoxLayout *vertical_layout;
    QHBoxLayout *horizontal_layout;
};

#endif // APP_H
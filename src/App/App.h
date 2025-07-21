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
#include "../widgets/Explorer/ExplorerSection/ExplorerSection.h"
#include "../widgets/Editor/EditorSection/EditorSection.h"
#include "../widgets/StandardIO/StandardIOSection/StandardIOSection.h"

class App : public QWidget {
    Q_OBJECT

  public:
    App(QWidget *parent = nullptr);
    void assignObjectNames();
    void applyQtStyles();
    void loadStyleSheet();
    
    
  private:
    MenuSection *menu_section;
    ToolbarSection *toolbar_section;
    ExplorerSection *explorer_section;
    EditorSection *editor_section;
    StandardIOSection *standardio_section;
    QWidget *content_wrapper; // content = all - menu_section
    QSplitter *file_editor_standardio_splitter;
    QVBoxLayout *vertical_layout;
    QHBoxLayout *horizontal_layout;
};

#endif // APP_H
#ifndef EDITORSECTION_H
#define EDITORSECTION_H

#include <QWidget>
#include <QVBoxLayout>

class EditorSection : public QWidget {
    Q_OBJECT

  public:
    explicit EditorSection(QWidget *parent = nullptr);
    void assignObjectNames();
    void applyQtStyles();
    void loadStyleSheet();

  private:
    QWidget *files_container;
    QWidget *code_editor;
    QVBoxLayout *layout;
};

#endif // EDITORSECTION_H
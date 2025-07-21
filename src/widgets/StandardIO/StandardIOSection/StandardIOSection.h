#ifndef STANDARDIOSECTION_H
#define STANDARDIOSECTION_H

#include "../ExecutionOptionsContainer/ExecutionOptionsContainer.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QTextEdit>


class StandardIOSection : public QWidget {
    Q_OBJECT

  public:
    explicit StandardIOSection(QWidget *parent = nullptr);
    void assignObjectNames();
    void applyQtStyles();
    void loadStyleSheet();

  private:
    ExecutionOptionsContainer *execution_options_container;
    QTextEdit *input_text_box;
    QTextEdit *output_text_box;
    QVBoxLayout *layout;
};

#endif // STANDARDIOSECTION_H
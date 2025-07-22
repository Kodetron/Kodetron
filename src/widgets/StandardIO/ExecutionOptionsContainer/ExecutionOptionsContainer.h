#ifndef EXECUTIONOPTIONSCONTAINER_H
#define EXECUTIONOPTIONSCONTAINER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

class ExecutionOptionsContainer : public QWidget {
    Q_OBJECT

  public:
    explicit ExecutionOptionsContainer(QWidget *parent = nullptr);
    void assignObjectNames();
    void applyQtStyles();
    void loadStyleSheet();
    QPushButton* getRunButton() const { return run_button; }

  private:
    QPushButton *run_button;
    QLabel *run_in_terminal_label;
    QCheckBox *run_in_terminal_checkbox;
    QHBoxLayout *layout;
};

#endif // EXECUTIONOPTIONSCONTAINER_H

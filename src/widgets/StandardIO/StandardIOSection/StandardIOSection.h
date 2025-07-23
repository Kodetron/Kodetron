#ifndef STANDARDIOSECTION_H
#define STANDARDIOSECTION_H

#include "../ExecutionOptionsContainer/ExecutionOptionsContainer.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QTextEdit>
#include "../../KodetronEditor/KodetronEditor.h"


class StandardIOSection : public QWidget {
    Q_OBJECT

  public:
    explicit StandardIOSection(KodetronEditor* code_editor, QWidget *parent = nullptr);
    void assignObjectNames();
    void applyQtStyles();
    void loadStyleSheet();

  private slots:
    void onRunClicked();

  private:
    ExecutionOptionsContainer *execution_options_container;
    QTextEdit *input_text_box;
    QTextEdit *output_text_box;
    QVBoxLayout *layout;
    KodetronEditor* code_editor;
    // Run/compile settings
    static constexpr int COMPILE_TIMEOUT_MS = 4000; // 4 seconds
    static constexpr int RUN_TIMEOUT_MS = 5000;     // 5 seconds
    static constexpr const char* TEMP_CPP_FILENAME = "/temp.cpp";
    static constexpr const char* TEMP_EXE_FILENAME = "/temp_exe.exe";
};

#endif // STANDARDIOSECTION_H
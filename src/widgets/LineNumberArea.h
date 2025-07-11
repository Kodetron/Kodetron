#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>

class CodeEditor;

class LineNumberArea : public QWidget {
    Q_OBJECT // This macro is necessary for Qt's meta-object system (signals and slots)

public:
    LineNumberArea(CodeEditor *editor);

    // Returns the preferred size hint for the widget
    QSize sizeHint() const override;

protected:
    // paintEvent to handle painting of the line numbers
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *codeEditor;
};

#endif
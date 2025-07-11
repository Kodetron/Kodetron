#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject> // Required for Q_OBJECT macro

class LineNumberArea;

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT // This macro is necessary for Qt's meta-object system (signals and slots)

public:
    CodeEditor(QWidget *parent = nullptr);

    // Calculates the width needed for the line number area
    int lineNumberAreaWidth();

    // Updates the viewport margins to accommodate the line number area
    void updateLineNumberAreaWidth(int newBlockCount);

    // Updates the line number area's geometry and content
    void updateLineNumberArea(const QRect &rect, int dy);

    // Paints the line numbers in the LineNumberArea
    void lineNumberAreaPaintEvent(QPaintEvent *event);

protected:
    // Override resizeEvent to adjust line number area geometry
    void resizeEvent(QResizeEvent *event) override;

private:

    LineNumberArea *lineNumberArea;
    friend class LineNumberArea;
};

#endif
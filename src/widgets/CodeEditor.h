#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject> // Required for Q_OBJECT macro
#include <QFocusEvent>
#include <QKeyEvent>

class LineNumberArea;

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT // This macro is necessary for Qt's meta-object system (signals and slots)

public:
    enum EditorType {
        MainEditor,     // Full featured editor
        InputOutput     // Simple editor for input/output
    };
    
    CodeEditor(QWidget *parent = nullptr, EditorType type = MainEditor);

    // Calculates the width needed for the line number area
    int lineNumberAreaWidth();

    // Updates the viewport margins to accommodate the line number area
    void updateLineNumberAreaWidth(int newBlockCount);

    // Updates the line number area's geometry and content
    void updateLineNumberArea(const QRect &rect, int dy);

    // Paints the line numbers in the LineNumberArea
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    
    // Placeholder text support
    void setPlaceholderText(const QString &text);
    QString placeholderText() const;

protected:
    // Override resizeEvent to adjust line number area geometry
    void resizeEvent(QResizeEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void setupDarkTheme();
    void showPlaceholder();
    void hidePlaceholder();
    bool isPlaceholderVisible() const;

    LineNumberArea *lineNumberArea;
    EditorType m_editorType;
    QString m_placeholderText;
    bool m_placeholderVisible;
    friend class LineNumberArea;
};

#endif
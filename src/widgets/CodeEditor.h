#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <QObject>
#include <QFocusEvent>
#include <QKeyEvent>

class CodeEditor : public QsciScintilla {
    Q_OBJECT

public:
    enum EditorType {
        MainEditor,     // Full featured editor with folding
        InputOutput     // Simple editor without folding for input/output
    };
    
    CodeEditor(QWidget *parent = nullptr, EditorType type = MainEditor);
    
    // Compatibility methods for existing code
    void setPlainText(const QString &text);
    QString toPlainText() const;
    void setPlaceholderText(const QString &text);
    void setReadOnly(bool readOnly);
    void clear();
    
    // Line number synchronization
    void setLineNumberOffset(int offset);
    int getLineNumberOffset() const;

protected:
    // Event overrides for placeholder behavior
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onTextChanged();
    void onModificationAttempted();
    void onCursorPositionChanged();
    void updateMarginWidth();

private:
    void setupModernTheme();
    void setupSyntaxHighlighting();
    void setupEditorFeatures();
    void setupMargins();
    void setupAutoCompletion();
    void setupIndentation();
    void setupPlaceholderSystem();
    void showPlaceholder();
    void hidePlaceholder();
    bool isPlaceholderVisible() const;
    void calculateAndSetMarginWidth();
    QString calculateMarginWidthString() const;
    
    QsciLexerCPP *m_lexer;
    QsciLexerCPP *m_placeholderLexer;
    QString m_placeholderText;
    QString m_realText;
    bool m_placeholderMode;
    bool m_placeholderActive;  // New flag for focus-based placeholder
    EditorType m_editorType;
    int m_lineNumberOffset;
};

#endif
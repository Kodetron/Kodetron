#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <QObject>

class CodeEditor : public QsciScintilla {
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    
    // Compatibility methods for existing code
    void setPlainText(const QString &text);
    QString toPlainText() const;
    void setPlaceholderText(const QString &text);
    void setReadOnly(bool readOnly);
    void clear();

private slots:
    void onTextChanged();
    void onModificationAttempted();
    void onCursorPositionChanged();

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
    
    QsciLexerCPP *m_lexer;
    QsciLexerCPP *m_placeholderLexer;
    QString m_placeholderText;
    QString m_realText;
    bool m_placeholderMode;
};

#endif
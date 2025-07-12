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

private:
    void setupModernTheme();
    void setupSyntaxHighlighting();
    void setupEditorFeatures();
    void setupMargins();
    void setupAutoCompletion();
    void setupIndentation();
    
    QsciLexerCPP *m_lexer;
    QString m_placeholderText;
};

#endif
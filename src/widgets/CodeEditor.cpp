#include "CodeEditor.h"

#include <QFont>
#include <QFontMetrics>
#include <QColor>
#include <QApplication>
#include <QPalette>

CodeEditor::CodeEditor(QWidget *parent) 
    : QsciScintilla(parent)
    , m_lexer(nullptr)
{
    setupModernTheme();
    setupSyntaxHighlighting();
    setupEditorFeatures();
    setupMargins();
    setupAutoCompletion();
    setupIndentation();
}

// Compatibility methods for existing code
void CodeEditor::setPlainText(const QString &text) {
    setText(text);
}

QString CodeEditor::toPlainText() const {
    return text();
}

void CodeEditor::setPlaceholderText(const QString &text) {
    m_placeholderText = text;
    // QScintilla doesn't have built-in placeholders, so we'll implement a simple version
    if (text().isEmpty() && !m_placeholderText.isEmpty()) {
        setText(m_placeholderText);
    }
}

void CodeEditor::setReadOnly(bool readOnly) {
    QsciScintilla::setReadOnly(readOnly);
}

void CodeEditor::clear() {
    setText("");
}

// Modern theme setup
void CodeEditor::setupModernTheme() {
    // Set up a modern monospace font
    QFont font("JetBrains Mono", 12);
    font.setFixedPitch(true);
    setFont(font);
    
    // Modern dark theme colors (inspired by VS Code Dark+)
    setColor(QColor("#D4D4D4")); // Default text color
    setPaper(QColor("#1E1E1E")); // Background color
    
    // Selection colors
    setSelectionBackgroundColor(QColor("#264F78"));
    setSelectionForegroundColor(QColor("#FFFFFF"));
    
    // Current line highlighting
    setCaretLineBackgroundColor(QColor("#2A2D2E"));
    setCaretLineVisible(true);
    
    // Margin colors
    setMarginsBackgroundColor(QColor("#252526"));
    setMarginsForegroundColor(QColor("#858585"));
    
    // Edge line (column guide)
    setEdgeMode(QsciScintilla::EdgeLine);
    setEdgeColumn(80);
    setEdgeColor(QColor("#3C3C3C"));
}

void CodeEditor::setupSyntaxHighlighting() {
    m_lexer = new QsciLexerCPP(this);
    
    // Configure the lexer with modern colors (VS Code Dark+ inspired)
    m_lexer->setColor(QColor("#D4D4D4"), QsciLexerCPP::Default);           // Default text
    m_lexer->setColor(QColor("#6A9955"), QsciLexerCPP::Comment);           // Comments
    m_lexer->setColor(QColor("#6A9955"), QsciLexerCPP::CommentLine);       // Line comments
    m_lexer->setColor(QColor("#6A9955"), QsciLexerCPP::CommentDoc);        // Documentation comments
    m_lexer->setColor(QColor("#B5CEA8"), QsciLexerCPP::Number);            // Numbers
    m_lexer->setColor(QColor("#CE9178"), QsciLexerCPP::DoubleQuotedString); // Double-quoted strings
    m_lexer->setColor(QColor("#CE9178"), QsciLexerCPP::SingleQuotedString); // Single-quoted strings
    m_lexer->setColor(QColor("#D4D4D4"), QsciLexerCPP::Operator);          // Operators
    m_lexer->setColor(QColor("#9CDCFE"), QsciLexerCPP::Identifier);        // Identifiers
    m_lexer->setColor(QColor("#569CD6"), QsciLexerCPP::Keyword);           // Keywords (if, for, while, etc.)
    m_lexer->setColor(QColor("#569CD6"), QsciLexerCPP::KeywordSet2);       // Additional keywords
    m_lexer->setColor(QColor("#C586C0"), QsciLexerCPP::PreProcessor);      // Preprocessor directives
    
    // Additional C++ specific highlighting
    m_lexer->setColor(QColor("#4EC9B0"), QsciLexerCPP::GlobalClass);       // Class names
    m_lexer->setColor(QColor("#DCDCAA"), QsciLexerCPP::CommentLineDoc);    // Documentation line comments
    
    // Set the lexer
    setLexer(m_lexer);
}

void CodeEditor::setupEditorFeatures() {
    // Enable line numbers
    setMarginType(0, QsciScintilla::NumberMargin);
    setMarginLineNumbers(0, true);
    setMarginWidth(0, "0000");
    
    // Enable folding
    setFolding(QsciScintilla::BoxedTreeFoldStyle);
    setMarginType(1, QsciScintilla::SymbolMargin);
    setMarginWidth(1, "20");
    
    // Set up brace matching
    setBraceMatching(QsciScintilla::SloppyBraceMatch);
    
    // Enable auto-indentation
    setAutoIndent(true);
    
    // Set tab width
    setTabWidth(4);
    setIndentationsUseTabs(false);
    
    // Enable undo/redo
    setUndoRedoEnabled(true);
}

void CodeEditor::setupMargins() {
    // Set up the line number margin
    setMarginType(0, QsciScintilla::NumberMargin);
    setMarginLineNumbers(0, true);
    setMarginWidth(0, "0000");
    
    // Set up the folding margin
    setMarginType(1, QsciScintilla::SymbolMargin);
    setMarginWidth(1, "20");
    setMarginSensitivity(1, true);
}

void CodeEditor::setupAutoCompletion() {
    // Enable auto-completion
    setAutoCompletionSource(QsciScintilla::AcsAll);
    setAutoCompletionThreshold(2);
    setAutoCompletionCaseSensitivity(false);
    setAutoCompletionReplaceWord(false);
    
    // Set up call tips
    setCallTipsStyle(QsciScintilla::CallTipsNoContext);
    setCallTipsVisible(0);
}

void CodeEditor::setupIndentation() {
    // Set up indentation guides
    setIndentationGuides(true);
    setIndentationGuidesForegroundColor(QColor("#404040"));
    setIndentationGuidesBackgroundColor(QColor("#1E1E1E"));
    
    // Set up backspace unindents
    setBackspaceUnindents(true);
}
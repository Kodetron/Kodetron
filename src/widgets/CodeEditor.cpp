#include "CodeEditor.h"

#include <QFont>
#include <QFontMetrics>
#include <QColor>
#include <QApplication>
#include <QPalette>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QTextStream>

CodeEditor::CodeEditor(QWidget *parent, EditorType type) 
    : QsciScintilla(parent)
    , m_lexer(nullptr)
    , m_placeholderLexer(nullptr)
    , m_placeholderMode(false)
    , m_placeholderActive(false)
    , m_editorType(type)
    , m_lineNumberOffset(0)
{
    setupModernTheme();
    setupSyntaxHighlighting();
    setupEditorFeatures();
    setupMargins();
    setupAutoCompletion();
    setupIndentation();
    setupPlaceholderSystem();
    
    // Connect text changes to margin width updates
    connect(this, &QsciScintilla::textChanged, this, &CodeEditor::updateMarginWidth);
    connect(this, &QsciScintilla::linesChanged, this, &CodeEditor::updateMarginWidth);
}

// Compatibility methods for existing code
void CodeEditor::setPlainText(const QString &text) {
    if (text.isEmpty() && !m_placeholderText.isEmpty()) {
        showPlaceholder();
    } else {
        hidePlaceholder();
        setText(text);
        m_realText = text;
    }
}

QString CodeEditor::toPlainText() const {
    if (m_placeholderMode) {
        return QString();
    }
    return text();
}

void CodeEditor::setPlaceholderText(const QString &text) {
    m_placeholderText = text;
    if (this->text().isEmpty() && !m_placeholderText.isEmpty()) {
        showPlaceholder();
    }
}

void CodeEditor::setReadOnly(bool readOnly) {
    QsciScintilla::setReadOnly(readOnly);
}

void CodeEditor::clear() {
    hidePlaceholder();
    setText("");
    m_realText = "";
}

void CodeEditor::setupPlaceholderSystem() {
    // Create a special lexer for placeholder text
    m_placeholderLexer = new QsciLexerCPP(this);
    
    // Set the placeholder lexer's background color to match the editor
    m_placeholderLexer->setPaper(QColor("#1E1E1E"));
    m_placeholderLexer->setDefaultPaper(QColor("#1E1E1E"));
    
    // Configure placeholder lexer with translucent colors (gray with transparency)
    QColor placeholderColor(180, 180, 180);
    placeholderColor.setAlpha(120); // Semi-transparent
    
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::Default);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::Comment);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::CommentLine);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::CommentDoc);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::Number);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::DoubleQuotedString);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::SingleQuotedString);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::Operator);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::Identifier);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::Keyword);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::KeywordSet2);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::PreProcessor);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::GlobalClass);
    m_placeholderLexer->setColor(placeholderColor, QsciLexerCPP::CommentLineDoc);
    
    // Connect signals for text changes - use a more reliable approach
    connect(this, &QsciScintilla::textChanged, this, &CodeEditor::onTextChanged);
    connect(this, &QsciScintilla::modificationAttempted, this, &CodeEditor::onModificationAttempted);
    connect(this, &QsciScintilla::cursorPositionChanged, this, &CodeEditor::onCursorPositionChanged);
}

void CodeEditor::onTextChanged() {
    if (m_placeholderMode) {
        // User started typing, hide placeholder
        hidePlaceholder();
    }
}

void CodeEditor::onModificationAttempted() {
    if (m_placeholderMode) {
        // User tried to modify placeholder text, hide it
        hidePlaceholder();
    }
}

void CodeEditor::onCursorPositionChanged() {
    if (m_placeholderMode) {
        // User moved cursor, hide placeholder
        hidePlaceholder();
    }
}

void CodeEditor::showPlaceholder() {
    if (!m_placeholderText.isEmpty() && text().isEmpty()) {
        m_placeholderMode = true;
        m_placeholderActive = true;
        m_realText = text();
        
        // Temporarily disconnect textChanged to avoid triggering onTextChanged
        disconnect(this, &QsciScintilla::textChanged, this, &CodeEditor::onTextChanged);
        
        // Set placeholder lexer and text
        setLexer(m_placeholderLexer);
        setText(m_placeholderText);
        
        // Reconnect the signal
        connect(this, &QsciScintilla::textChanged, this, &CodeEditor::onTextChanged);
    }
}

void CodeEditor::hidePlaceholder() {
    if (m_placeholderMode) {
        m_placeholderMode = false;
        m_placeholderActive = false;
        
        // Temporarily disconnect textChanged
        disconnect(this, &QsciScintilla::textChanged, this, &CodeEditor::onTextChanged);
        
        // Restore normal lexer
        setLexer(m_lexer);
        
        // Clear the text and restore real text if any
        setText(m_realText);
        
        // Reconnect the signal
        connect(this, &QsciScintilla::textChanged, this, &CodeEditor::onTextChanged);
    }
}

bool CodeEditor::isPlaceholderVisible() const {
    return m_placeholderMode;
}

void CodeEditor::setLineNumberOffset(int offset) {
    m_lineNumberOffset = offset;
    // Force a redraw of the margin to show updated line numbers
    setMarginWidth(0, marginWidth(0));
}

int CodeEditor::getLineNumberOffset() const {
    return m_lineNumberOffset;
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
    
    // Cursor color - make it white so it's visible on dark background
    setCaretForegroundColor(QColor("#FFFFFF"));
    
    // Margin colors
    setMarginsBackgroundColor(QColor("#1E1E1E")); // Match editor background
    setMarginsForegroundColor(QColor("#858585"));
    
    // Edge line (column guide)
    setEdgeMode(QsciScintilla::EdgeLine);
    setEdgeColumn(80);
    setEdgeColor(QColor("#3C3C3C"));
}

void CodeEditor::setupSyntaxHighlighting() {
    m_lexer = new QsciLexerCPP(this);
    
    // Set the lexer's background color to match the editor
    m_lexer->setPaper(QColor("#1E1E1E"));
    m_lexer->setDefaultPaper(QColor("#1E1E1E"));
    
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
    // (No need to call colorize/colourise)
}

void CodeEditor::setupEditorFeatures() {
    // Enable line numbers
    setMarginType(0, QsciScintilla::NumberMargin);
    setMarginLineNumbers(0, true);
    calculateAndSetMarginWidth(); // Use dynamic width calculation
    
    // Only enable folding for main editor, not for input/output panels
    if (m_editorType == MainEditor) {
        setFolding(QsciScintilla::BoxedTreeFoldStyle);
        setMarginType(1, QsciScintilla::SymbolMargin);
        setMarginWidth(1, "20");
        setMarginMarkerMask(1, 0xFFFFFF); // Show all folding markers
    }
    
    // Set up brace matching
    setBraceMatching(QsciScintilla::SloppyBraceMatch);
    
    // Enable auto-indentation
    setAutoIndent(true);
    
    // Set tab width
    setTabWidth(4);
    setIndentationsUseTabs(false);
}

void CodeEditor::setupMargins() {
    // Set up the line number margin
    setMarginType(0, QsciScintilla::NumberMargin);
    setMarginLineNumbers(0, true);
    calculateAndSetMarginWidth(); // Use dynamic width calculation
    
    // Remove any gaps by setting margin background to match editor
    setMarginsBackgroundColor(QColor("#1E1E1E"));
    
    // Only set up folding margin for main editor
    if (m_editorType == MainEditor) {
        setMarginType(1, QsciScintilla::SymbolMargin);
        setMarginWidth(1, "20");
        setMarginSensitivity(1, true);
        setMarginMarkerMask(1, 0xFFFFFF); // Show all folding markers
    } else {
        // For input/output editors, disable the folding margin to save space
        setMarginType(1, QsciScintilla::SymbolMargin);
        setMarginWidth(1, "0"); // Zero width to remove the gap
    }
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

// Event overrides for improved placeholder behavior
void CodeEditor::focusInEvent(QFocusEvent *event) {
    QsciScintilla::focusInEvent(event);
    
    // Show placeholder if editor is empty and has placeholder text
    if (text().isEmpty() && !m_placeholderText.isEmpty() && !m_placeholderActive) {
        showPlaceholder();
    }
}

void CodeEditor::focusOutEvent(QFocusEvent *event) {
    QsciScintilla::focusOutEvent(event);
    
    // If placeholder is active and editor is empty, keep placeholder visible
    if (m_placeholderActive && text().isEmpty()) {
        // Don't hide placeholder on focus out if still empty
        return;
    }
    
    // Hide placeholder if it was active and now has content
    if (m_placeholderActive && !text().isEmpty()) {
        hidePlaceholder();
    }
}

void CodeEditor::keyPressEvent(QKeyEvent *event) {
    // Hide placeholder on any key press if it's active
    if (m_placeholderActive) {
        hidePlaceholder();
    }
    
    QsciScintilla::keyPressEvent(event);
}

void CodeEditor::updateMarginWidth() {
    calculateAndSetMarginWidth();
}

void CodeEditor::calculateAndSetMarginWidth() {
    QString widthString = calculateMarginWidthString();
    setMarginWidth(0, widthString);
}

QString CodeEditor::calculateMarginWidthString() const {
    // Get the number of lines in the document
    int lineCount = lines();
    if (lineCount == 0) lineCount = 1;
    
    // Calculate the number of digits needed
    int digits = 1;
    int temp = lineCount;
    while (temp /= 10) {
        digits++;
    }
    
    // Ensure minimum width for at least 3 digits
    digits = qMax(digits, 3);
    
    // Create a string with the appropriate number of '0' characters
    QString widthString;
    for (int i = 0; i < digits; ++i) {
        widthString += "0";
    }
    
    return widthString;
}
#include "KodetronEditor.h"
#include "KodetronTheme.h"

KodetronEditor::KodetronEditor(QWidget* parent)
    : QsciScintilla(parent)
{
    // Set up C++ syntax highlighting by default for competitive programming
    setupCppSyntaxHighlighting();
    // Configure margins for line numbers and code folding only
    setupMargins();
    setupBraceMatching();
    setupIndentation();
    setupCaretLineHighlight();
    setupAutocompletion();
    setupDefaultTheme();

    // Subscribe to AppState file path changes
    connect(&AppState::instance(), &AppState::selectedFilePathModified,
            this, &KodetronEditor::onFilePathChanged);

    // Add Ctrl+S shortcut for saving
    QShortcut* saveShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    connect(saveShortcut, &QShortcut::activated, this, &KodetronEditor::saveCurrentFile);
}

void KodetronEditor::saveCurrentFile() {
    QString file_path = AppState::instance().getSelectedFilePath();
    QString content = text();
    if (!file_path.isEmpty()) {
        bool success = FileDialog::writeFileContents(file_path, content);
    }
}

void KodetronEditor::onFilePathChanged(const QString& file_path) {
    if (file_path.isEmpty()) {
        setText(QString()); // Clear editor if no file path is set
        return;
    }
    QString content = FileDialog::readFileContents(file_path);
    setText(content);
}

void KodetronEditor::setupCppSyntaxHighlighting() {
    if (!cppLexer) {
        cppLexer = new QsciLexerCPP(this);
    }
    setLexer(cppLexer);
}

void KodetronEditor::setupMargins() {
    setMarginType(0, QsciScintilla::NumberMargin);
    setMarginWidth(0, "0000");
    setMarginType(2, QsciScintilla::SymbolMargin);
    setMarginWidth(2, 14);
    setMarginSensitivity(2, true);
    setFolding(QsciScintilla::CircledTreeFoldStyle, 2);
}

void KodetronEditor::setupBraceMatching() {
    setBraceMatching(QsciScintilla::SloppyBraceMatch);
}

void KodetronEditor::setupIndentation() {
    setAutoIndent(true);
    setIndentationWidth(4);
    setIndentationsUseTabs(false);
    setBackspaceUnindents(true);
    setIndentationGuides(true);
}

void KodetronEditor::setupCaretLineHighlight() {
    setCaretLineVisible(true);
}

void KodetronEditor::setupAutocompletion() {
    if (!cppLexer) return;
    if (!cppAPIs) {
        cppAPIs = new QsciAPIs(cppLexer);
        const char* keywords[] = {
            "int", "long", "double", "char", "bool", "string", "vector", "pair", "map", "set", "queue", "stack", "priority_queue", "for", "while", "else", "return", "break", "continue", "const", "auto", "void", "true", "false"
        };
        for (const char* kw : keywords) {
            cppAPIs->add(QString::fromLatin1(kw));
        }
        cppAPIs->prepare();
    }
    setAutoCompletionSource(QsciScintilla::AcsAll);
    setAutoCompletionCaseSensitivity(false);
    setAutoCompletionThreshold(2);
}

void KodetronEditor::setupDefaultTheme() {
    KodetronTheme theme;

    // Main editor colors
    //setPaper(theme.editorBackground);
    //setColor(theme.editorForeground);
    // setFont(theme.editorFont);

    // Ensure lexer respects theme colors
    if (cppLexer) {
        //cppLexer->setDefaultColor(theme.editorForeground);
        //cppLexer->setDefaultPaper(theme.editorBackground);
        cppLexer->setDefaultFont(theme.editorFont);
    }

    // Line numbers (Margin 0)
    setMarginsFont(theme.lineNumberFont);
    setMarginsBackgroundColor(theme.lineNumberBackground);
    setMarginsForegroundColor(theme.lineNumberForeground);


    // Caret line, selection, and caret
    
    setCaretLineBackgroundColor(theme.caretLineBackground);
    // setSelectionBackgroundColor(theme.selectionBackground);
    // setSelectionForegroundColor(theme.selectionForeground);
    
    setCaretForegroundColor(theme.caretForeground);

    // Folding markers
    int markers[] = {
        QsciScintilla::SC_MARKNUM_FOLDER,
        QsciScintilla::SC_MARKNUM_FOLDEROPEN,
        QsciScintilla::SC_MARKNUM_FOLDERSUB,
        QsciScintilla::SC_MARKNUM_FOLDERTAIL,
        QsciScintilla::SC_MARKNUM_FOLDEREND,
        QsciScintilla::SC_MARKNUM_FOLDEROPENMID,
        QsciScintilla::SC_MARKNUM_FOLDERMIDTAIL
    };
    for (int m : markers) {
        setMarkerForegroundColor(theme.foldingMarginBackground, m);
        setMarkerBackgroundColor(theme.foldingIconColor, m);
    }

    // Fold margin colors
    setFoldMarginColors(theme.foldingMarginBackground, theme.foldingMarginBackground);
    
    applyCodeColors();
}

void KodetronEditor::applyCodeColors() {
    if (!cppLexer)
        return;

    KodetronTheme theme;  // or use a stored member if you persist theme

    // ----------------------------
    // Baseline: apply background, font, and default text color to *all* styles
    // ----------------------------
    const int all_styles[] = {
        QsciLexerCPP::Default,
        QsciLexerCPP::Identifier,
        QsciLexerCPP::Operator,
        QsciLexerCPP::Number,
        QsciLexerCPP::Keyword,
        QsciLexerCPP::DoubleQuotedString,
        QsciLexerCPP::SingleQuotedString,
        QsciLexerCPP::Comment,
        QsciLexerCPP::CommentLine,
        QsciLexerCPP::CommentDoc,
        QsciLexerCPP::PreProcessor,
        QsciLexerCPP::UnclosedString,
        QsciLexerCPP::VerbatimString,
    };

    for (int st : all_styles) {
        //cppLexer->setPaper(theme.editorBackground, st);
        cppLexer->setFont(theme.editorFont, st);
        //cppLexer->setColor(theme.synDefault, st);  // overridden below per style
    }

    // ----------------------------
    // Style-specific overrides
    // ----------------------------
    cppLexer->setColor(theme.synKeyword,        QsciLexerCPP::Keyword);
    cppLexer->setColor(theme.synNumber,         QsciLexerCPP::Number);
    cppLexer->setColor(theme.synString,         QsciLexerCPP::DoubleQuotedString);
    cppLexer->setColor(theme.synChar,           QsciLexerCPP::SingleQuotedString);
    cppLexer->setColor(theme.synPreproc,        QsciLexerCPP::PreProcessor);
    cppLexer->setColor(theme.synOperator,       QsciLexerCPP::Operator);
    cppLexer->setColor(theme.synIdentifier,     QsciLexerCPP::Identifier);
    cppLexer->setColor(theme.synComment,        QsciLexerCPP::Comment);
    cppLexer->setColor(theme.synComment,        QsciLexerCPP::CommentLine);
    cppLexer->setColor(theme.synDocComment,     QsciLexerCPP::CommentDoc);
    cppLexer->setColor(theme.synUnclosed,       QsciLexerCPP::UnclosedString);
    cppLexer->setColor(theme.synString,         QsciLexerCPP::VerbatimString);

}

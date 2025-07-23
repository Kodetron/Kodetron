#pragma once
#include <QColor>
#include <QFont>

struct KodetronTheme {
    // Main backgrounds
    QColor appBackground        = QColor("#0A1220");   // deep navy
    QColor editorBackground     = QColor("#1E1E1E");   // VSCode-like dark
    QColor editorForeground     = QColor("#D4D4D4");   // light gray text
    QFont editorFont            = QFont("Consolas", 11);

    // Line numbers
    QColor lineNumberBackground = QColor("#252526");   // slightly lighter than editor
    QColor lineNumberForeground = QColor("#858585");   // muted gray
    QFont lineNumberFont        = QFont("Consolas", 11, QFont::Bold);

    // Caret and selection
    QColor caretLineBackground  = QColor("#2C2C2C");   // subtle highlight
    QColor caretForeground      = QColor("#FFFFFF");   // White caret
    QColor selectionBackground  = QColor("#264F78");   // blue highlight
    QColor selectionForeground  = QColor("#FFFFFF");   // White text on selection

    // Folding and gutter
    QColor foldingMarginBackground = QColor("#252526"); // matches line number margin
    QColor foldingIconColor        = QColor("#C5C5C5");  // grayish folding icons
    QColor borderColor             = QColor("#141824");  // subtle border

    // Syntax colors
    QColor synDefault      = QColor("#D4D4D4"); // general code
    QColor synIdentifier   = QColor("#D4D4D4");
    QColor synKeyword      = QColor("#C586C0"); // purple
    QColor synType         = QColor("#4EC9B0"); // types / class names (optional use)
    QColor synNumber       = QColor("#B5CEA8"); // soft green-cyan
    QColor synString       = QColor("#CE9178"); // warm string
    QColor synChar         = QColor("#CE9178");
    QColor synComment      = QColor("#6A9955"); // comment green
    QColor synDocComment   = QColor("#608B4E");
    QColor synPreproc      = QColor("#DCDCAA"); // #include / #define
    QColor synOperator     = QColor("#D4D4D4");
    QColor synUnclosed     = QColor("#F44747"); // error-ish

};

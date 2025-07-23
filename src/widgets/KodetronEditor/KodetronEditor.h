#pragma once
#include <Qsci/qsciscintilla.h>
#include <QObject>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>
#include <QShortcut>
#include "KodetronTheme.h"
#include "../../Global/AppState.h"
#include "../../FileSystemOperations/FileDialog/FileDialog.h"

class KodetronEditor : public QsciScintilla {
    Q_OBJECT
public:
    explicit KodetronEditor(QWidget* parent = nullptr);
    void showThemeDialog(QWidget* parent = nullptr);
private:
    QsciLexerCPP* cppLexer = nullptr;
    QsciAPIs* cppAPIs = nullptr;
    void setupCppSyntaxHighlighting();
    void setupMargins();
    void setupBraceMatching();
    void setupIndentation();
    void setupCaretLineHighlight();
    void setupAutocompletion();
    void setupDefaultTheme();
    void applyCodeColors();
    void onFilePathChanged(const QString& new_path);
    void saveCurrentFile();
};
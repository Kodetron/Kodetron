#include "StandardIOSection.h"
#include "../utils/StyleLoader/StyleReader.h"
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QTemporaryDir>
#include <QTemporaryFile>

StandardIOSection::StandardIOSection(KodetronEditor* code_editor, QWidget *parent) : QWidget(parent), code_editor(code_editor) {
    // Childs initialization
    execution_options_container = new ExecutionOptionsContainer(this);
    input_text_box = new QTextEdit(this);
    output_text_box = new QTextEdit(this); 
    output_text_box->setReadOnly(true);

    // Layout
    layout = new QVBoxLayout(this);
    layout->addWidget(execution_options_container);
    layout->addWidget(input_text_box);
    layout->addWidget(output_text_box);
    setLayout(layout);

    // Connect Run button
    connect(execution_options_container->getRunButton(), &QPushButton::clicked, this, &StandardIOSection::onRunClicked);

    // Styles
    setAttribute(Qt::WA_StyledBackground, true);
    assignObjectNames();
    applyQtStyles();
    loadStyleSheet();
}
void StandardIOSection::assignObjectNames() {
    setObjectName("standard_io_section");
    input_text_box->setObjectName("input_text_box");
    output_text_box->setObjectName("output_text_box");
}
void StandardIOSection::applyQtStyles() {
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(20);
}
void StandardIOSection::loadStyleSheet() {
    QString styleSheet = StyleLoader::read("../src/widgets/StandardIO/StandardIOSection/StandardIOSection.qss");
    if (!styleSheet.isEmpty()) {
        setStyleSheet(styleSheet);
    }
}

void StandardIOSection::onRunClicked() {
    // 1. Get code from the editor
    QString code = code_editor ? code_editor->text() : QString();
    if (code.isEmpty()) {
        output_text_box->setPlainText("No code to run.");
        return;
    }

    // 2. Get input from the input box
    QString input = input_text_box->toPlainText();

    // 3. Save code to a temp file
    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        output_text_box->setPlainText("Failed to create temporary directory.");
        return;
    }
    QString cppFilePath = tempDir.path() + TEMP_CPP_FILENAME;
    QFile cppFile(cppFilePath);
    if (!cppFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        output_text_box->setPlainText("Failed to write temp.cpp");
        return;
    }
    QTextStream out(&cppFile);
    out << code;
    cppFile.close();

    // 4. Compile with g++
    QString exeFilePath = tempDir.path() + TEMP_EXE_FILENAME;
    QProcess compiler;
    QStringList args;
    args << cppFilePath << "-o" << exeFilePath;
    compiler.start("g++", args);
    if (!compiler.waitForFinished(COMPILE_TIMEOUT_MS)) { // 10s timeout
        output_text_box->setPlainText("Compilation timed out.");
        return;
    }
    QString compileStdErr = compiler.readAllStandardError();
    if (!compileStdErr.isEmpty()) {
        output_text_box->setPlainText("Compilation error:\n" + compileStdErr);
        return;
    }
    if (!QFile::exists(exeFilePath)) {
        output_text_box->setPlainText("Compilation failed: Executable not created.");
        return;
    }

    // 5. Run the program
    QProcess program;
    program.setProgram(exeFilePath);
    program.setProcessChannelMode(QProcess::MergedChannels);
    program.start();
    if (!input.isEmpty()) {
        program.write(input.toUtf8());
        program.closeWriteChannel();
    }
    if (!program.waitForFinished(RUN_TIMEOUT_MS)) { // 10s timeout
        output_text_box->setPlainText("Program execution timed out.");
        program.kill();
        return;
    }
    QString programOutput = program.readAllStandardOutput();
    output_text_box->setPlainText(programOutput);
    // 6. Clean up handled by QTemporaryDir
}

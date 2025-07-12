#include "CodeEditor.h"
#include "LineNumberArea.h"

#include <QPainter>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include <QColor>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QFont>
#include <QFontMetrics>

CodeEditor::CodeEditor(QWidget *parent, EditorType type) 
    : QPlainTextEdit(parent)
    , m_editorType(type)
    , m_placeholderVisible(false)
{
    lineNumberArea = new LineNumberArea(this);

    // Connect signals from CodeEditor to slots in CodeEditor to manage line number area
    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);

    updateLineNumberAreaWidth(0);
    
    // Setup dark theme
    setupDarkTheme();
}

int CodeEditor::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy)
        // If the content is scrolled vertically, scroll the line number area
        lineNumberArea->scroll(0, dy);
    else
        // Otherwise, update the specific rectangle of the line number area
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    // If the entire viewport is contained within the update rectangle, update the width
    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *event) {
    QPlainTextEdit::resizeEvent(event);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    
    // Fill background to match editor background (eliminates gap)
    QColor editorBackgroundColor = palette().color(QPalette::Base);
    painter.fillRect(event->rect(), editorBackgroundColor);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    // Loop through all visible blocks
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            // Use consistent color for line numbers (dark theme)
            painter.setPen(QColor("#858585")); // Gray color for line numbers
            painter.drawText(0, top, lineNumberArea->width()-2, fontMetrics().height(),
                             Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::setPlaceholderText(const QString &text) {
    m_placeholderText = text;
    if (toPlainText().isEmpty() && !m_placeholderText.isEmpty()) {
        showPlaceholder();
    }
}

QString CodeEditor::placeholderText() const {
    return m_placeholderText;
}

void CodeEditor::focusInEvent(QFocusEvent *event) {
    QPlainTextEdit::focusInEvent(event);
    
    // Show placeholder if editor is empty and has placeholder text
    if (toPlainText().isEmpty() && !m_placeholderText.isEmpty() && !m_placeholderVisible) {
        showPlaceholder();
    }
}

void CodeEditor::focusOutEvent(QFocusEvent *event) {
    QPlainTextEdit::focusOutEvent(event);
    
    // If placeholder is visible and editor is empty, keep placeholder visible
    if (m_placeholderVisible && toPlainText().isEmpty()) {
        // Don't hide placeholder on focus out if still empty
        return;
    }
    
    // Hide placeholder if it was visible and now has content
    if (m_placeholderVisible && !toPlainText().isEmpty()) {
        hidePlaceholder();
    }
}

void CodeEditor::paintEvent(QPaintEvent *event) {
    QPlainTextEdit::paintEvent(event);
    
    // Draw placeholder text if needed
    if (m_placeholderVisible && toPlainText().isEmpty()) {
        QPainter painter(viewport());
        painter.setPen(QColor(180, 180, 180, 120)); // Semi-transparent gray
        
        QFont font = this->font();
        painter.setFont(font);
        
        QRect rect = viewport()->rect();
        rect.setLeft(rect.left() + 4); // Small margin
        
        painter.drawText(rect, Qt::AlignTop | Qt::AlignLeft, m_placeholderText);
    }
}

void CodeEditor::setupDarkTheme() {
    // Set up a modern monospace font
    QFont font("JetBrains Mono", 12);
    font.setFixedPitch(true);
    setFont(font);
    
    // Modern dark theme colors (inspired by VS Code Dark+)
    QPalette palette = this->palette();
    palette.setColor(QPalette::Base, QColor("#1E1E1E")); // Background
    palette.setColor(QPalette::Text, QColor("#D4D4D4")); // Text color
    palette.setColor(QPalette::Window, QColor("#1E1E1E")); // Window background
    setPalette(palette);
}

void CodeEditor::showPlaceholder() {
    if (!m_placeholderText.isEmpty() && toPlainText().isEmpty()) {
        m_placeholderVisible = true;
        viewport()->update(); // Trigger repaint to show placeholder
    }
}

void CodeEditor::hidePlaceholder() {
    if (m_placeholderVisible) {
        m_placeholderVisible = false;
        viewport()->update(); // Trigger repaint to hide placeholder
    }
}

bool CodeEditor::isPlaceholderVisible() const {
    return m_placeholderVisible;
}
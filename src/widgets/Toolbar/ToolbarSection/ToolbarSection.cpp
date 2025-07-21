#include "ToolbarSection.h"
#include "../../../utils/StyleLoader/StyleReader.h"

QToolButton *ToolbarSection::createToolButton(const QString &iconPath, const QString &toolTip) {
    QToolButton *button = new QToolButton(this);
    button->setIcon(QIcon(iconPath));
    button->setToolTip(toolTip);
    return button;
}

ToolbarSection::ToolbarSection(QWidget *parent) : QWidget(parent) {
    // Initialize buttons with icons and tooltips
    open_snippets_button = createToolButton("../assets/open_snippets_icon.png", "Open Snippets");
    connect(open_snippets_button, &QToolButton::clicked, this, &ToolbarSection::openSnippets);

    open_templates_button = createToolButton("../assets/open_templates_icon.png", "Open Templates");
    connect(open_templates_button, &QToolButton::clicked, this, &ToolbarSection::openTemplates);

    open_settings_button = createToolButton("../assets/open_settings_icon.png", "Open Settings");
    connect(open_settings_button, &QToolButton::clicked, this, &ToolbarSection::openSettings);

    // Layout for the toolbar buttons
    layout = new QVBoxLayout(this);
    layout->addWidget(open_snippets_button);
    layout->addWidget(open_templates_button);
    layout->addStretch(); // Separate upper and lower buttons
    layout->addWidget(open_settings_button);
    setLayout(layout);

    // Styles
    setAttribute(Qt::WA_StyledBackground, true);
    assignObjectNames();
    applyQtStyles();
    loadStyleSheet();
}
void ToolbarSection::openSnippets() {
    // Logic to open snippets modal
}
void ToolbarSection::openTemplates() {
    // Logic to open templates modal
}
void ToolbarSection::openSettings() {
    // Logic to open settings modal
}

void ToolbarSection::assignObjectNames() {
    open_snippets_button->setObjectName("open_snippets_button");
    open_templates_button->setObjectName("open_templates_button");
    open_settings_button->setObjectName("open_settings_button");
}
void ToolbarSection::applyQtStyles() {
  layout->setContentsMargins(10, 10, 10, 10);
  layout->setSpacing(10);
  open_snippets_button->setCursor(Qt::PointingHandCursor);
  open_templates_button->setCursor(Qt::PointingHandCursor);
  open_settings_button->setCursor(Qt::PointingHandCursor);
}
void ToolbarSection::loadStyleSheet() {
    QString styleSheet = StyleLoader::read("../src/widgets/Toolbar/ToolbarSection/ToolbarSection.qss");
    if (!styleSheet.isEmpty()) {
        setStyleSheet(styleSheet);
    }
}

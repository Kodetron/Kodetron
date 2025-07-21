#include "ToolbarSection.h"
#include "../../../utils/StyleLoader/StyleReader.h"

QToolButton *ToolbarSection::createToolButton(const QString &iconPath, const QString &toolTip) {
    QToolButton *button = new QToolButton(this);
    button->setIcon(QIcon(iconPath));
    button->setToolTip(toolTip);
    return button;
}

ToolbarSection::ToolbarSection(DatabaseManager* dbManager, int userId, QWidget *parent):
    QWidget(parent),
    dbManager(dbManager), 
    currentUserId(userId),
    settingsModal(nullptr), // Initialize modal pointers to nullptr
    snippetsModal(nullptr),
    templatesModal(nullptr)
{
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
    if (!snippetsModal) {
        // Create the SnippetsModal if it hasn't been created yet
        snippetsModal = new SnippetsModal(dbManager, currentUserId, this);
    }
    snippetsModal->exec();
}
void ToolbarSection::openTemplates() {
    if (!templatesModal) {
        // Create the TemplatesModal if it hasn't been created yet
        templatesModal = new TemplatesModal(dbManager, currentUserId, this);
    }
    templatesModal->exec();
}
void ToolbarSection::openSettings() {
    if (!settingsModal) {
        // Create the SettingsModal if it hasn't been created yet
        settingsModal = new SettingsModal(dbManager, currentUserId, this);
    }
    settingsModal->exec();
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

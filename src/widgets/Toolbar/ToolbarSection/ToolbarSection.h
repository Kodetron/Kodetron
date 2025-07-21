#ifndef TOOLBARSECTION_H
#define TOOLBARSECTION_H

#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

#include "..\Buttons\SettingsModal.h"
#include "..\Buttons\SnippetsModal.h"
#include "..\Buttons\TemplatesModal.h"

#include "..\Database\DatabaseManager.h"

class ToolbarSection : public QWidget {
    Q_OBJECT

  public:
    explicit ToolbarSection(DatabaseManager* dbManager, int userId, QWidget *parent = nullptr); 
    QToolButton *createToolButton(const QString &iconPath, const QString &toolTip);
    void openSnippets();
    void openTemplates();
    void openSettings();
    void assignObjectNames();
    void applyQtStyles();
    void loadStyleSheet();

  private:
    QToolButton *open_snippets_button;
    QToolButton *open_templates_button;
    QToolButton *open_settings_button;
    QVBoxLayout *layout;

    DatabaseManager* dbManager;
    int currentUserId;
    SettingsModal* settingsModal;
    SnippetsModal* snippetsModal;
    TemplatesModal* templatesModal;
};

#endif // TOOLBARSECTION_H

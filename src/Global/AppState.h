#ifndef APPSTATE_H
#define APPSTATE_H

#include <QObject>
#include <QString>

class AppState : public QObject {
    Q_OBJECT
  public:
    static AppState &instance(); // Global access to the singleton

    QString getSelectedExplorerPath() const;
    QString getSelectedDirPath() const;
    QString getSelectedFilePath() const;
    bool isSelectedExplorerPath() const;
    bool isSelectedDirPath() const;
    bool isSelectedFilePath() const;

  public slots:
    void setSelectedExplorerPath(const QString &new_path, const QString &path_type);
    void setSelectedDirPath(const QString &new_path);
    void setSelectedFilePath(const QString &new_path);

  signals:
    void selectedExplorerPathModified(const QString &new_path, const QString &path_type);
    void selectedDirPathModified(const QString &new_path);
    void selectedFilePathModified(const QString &new_path);

  private:
    explicit AppState(QObject *parent = nullptr);
    QString selected_explorer_path_type;
    QString selected_explorer_path;
    QString selected_dir_path;
    QString selected_file_path;
    bool is_selected_explorer_path;
    bool is_selected_dir_path;
    bool is_selected_file_path;
};

struct enumExplorerPathType {
    std::string dir_type;
    std::string file_type;
    std::string empty;
};

#endif // APPSTATE_H

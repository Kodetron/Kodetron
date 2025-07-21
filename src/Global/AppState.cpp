#include "AppState.h"

AppState &AppState::instance() {
    static AppState instance;
    return instance;
}

AppState::AppState(QObject *parent)
    : QObject(parent),
      selected_explorer_path_type(""),
      selected_explorer_path(""),
      selected_dir_path(""),
      selected_file_path(""),
      is_selected_explorer_path(false),
      is_selected_dir_path(false),
      is_selected_file_path(false) {}

QString AppState::getSelectedExplorerPath() const {
    return selected_explorer_path;
}
QString AppState::getSelectedDirPath() const {
    return selected_dir_path;
}
QString AppState::getSelectedFilePath() const {
    return selected_file_path;
}
bool AppState::isSelectedExplorerPath() const {
    return is_selected_explorer_path;
}
bool AppState::isSelectedDirPath() const {
    return is_selected_dir_path;
}
bool AppState::isSelectedFilePath() const {
    return is_selected_file_path;
}

void AppState::setSelectedExplorerPath(const QString &new_path, const QString &path_type) {
    if (selected_explorer_path != new_path || selected_explorer_path_type != path_type) {
        selected_explorer_path = new_path;
        selected_explorer_path_type = path_type;
        is_selected_explorer_path = !new_path.isEmpty();
        emit selectedExplorerPathModified(new_path, path_type);
    }
}

void AppState::setSelectedDirPath(const QString &new_path) {
    if (selected_dir_path != new_path) {
        selected_dir_path = new_path;
        is_selected_dir_path = !new_path.isEmpty();
        emit selectedDirPathModified(new_path);
    }
}

void AppState::setSelectedFilePath(const QString &new_path) {
    if (selected_file_path != new_path) {
        selected_file_path = new_path;
        is_selected_file_path = !new_path.isEmpty();
        emit selectedFilePathModified(new_path);
    }
}
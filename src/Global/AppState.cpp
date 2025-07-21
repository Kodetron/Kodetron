#include "AppState.h"

AppState &AppState::instance() {
    static AppState instance;
    return instance;
}

AppState::AppState(QObject *parent) : QObject(parent) {
    selected_explorer_path = "";
    selected_dir_path = "";
    selected_file_path = "";
    is_selected_explorer_path = false;
    is_selected_dir_path = false;
    is_selected_file_path = false;
    enumExplorerPathType.dir_type = "DIR_TYPE";
    enumExplorerPathType.file_type = "FILE_TYPE";
    enumExplorerPathType.empty = "EMPTY";
    selected_explorer_path_type = enumExplorerPathType.empty;
}

// Getters
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

// Setters with emitters
void AppState::setSelectedExplorerPath(const QString &new_path, const std::string &new_path_type) {
    if (selected_explorer_path != new_path || selected_explorer_path_type != new_path_type) {
        selected_explorer_path = new_path;
        selected_explorer_path_type = new_path_type;
        is_selected_explorer_path = (new_path_type != enumExplorerPathType.empty);
        emit selectedExplorerPathModified(new_path, new_path_type);
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

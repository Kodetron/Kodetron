cmake_minimum_required(VERSION 3.14)
project(Kodetron LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
qt_standard_project_setup()   # sets up MOC/UIC etc.

# 1) First, try the shipped CMake config
find_package(QScintilla2Qt6 QUIET)

# 2) If that failed, try pkg-config (Ubuntu/Debian, Fedora, etc.)
if (NOT QScintilla2Qt6_FOUND)
  find_package(PkgConfig QUIET)
  if (PkgConfig_FOUND)
    pkg_check_modules(QSCI_PKG QUIET IMPORTED_TARGET qscintilla2-qt6)
    if (QSCI_PKG_FOUND)
      # alias the pkg-config target to the same name
      add_library(QScintilla2Qt6::QScintilla2Qt6 ALIAS PkgConfig::qscintilla2-qt6)
      set(QScintilla2Qt6_FOUND TRUE)
    endif()
  endif()
endif()

# 3) If still not found, do our Arch-style manual probe
if (NOT QScintilla2Qt6_FOUND)
  find_path(QSCI_INCLUDE_DIR
    NAMES Qsci/qsciscintilla.h
    HINTS /usr/include/qt6 /usr/include
  )
  find_library(QSCI_LIBRARY
    NAMES qscintilla2_qt6
    HINTS /usr/lib /usr/local/lib
  )
  if (QSCI_INCLUDE_DIR AND QSCI_LIBRARY)
    add_library(QScintilla2Qt6::QScintilla2Qt6 UNKNOWN IMPORTED)
    set_target_properties(QScintilla2Qt6::QScintilla2Qt6 PROPERTIES
      IMPORTED_LOCATION            "${QSCI_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${QSCI_INCLUDE_DIR}"
    )
    set(QScintilla2Qt6_FOUND TRUE)
  endif()
endif()

# 4) Fatal error if we still didn’t find it
if (NOT QScintilla2Qt6_FOUND)
  message(FATAL_ERROR
    "QScintilla2Qt6 not found!\n"
    "  • On Ubuntu/Debian: sudo apt install qscintilla2-qt6-dev\n"
    "  • On Fedora:        sudo dnf install qscintilla-qt6-devel\n"
    "  • On Arch/Manjaro:  sudo pacman -S qscintilla-qt6\n"
    "  • On Windows:       install QScintilla for Qt6 via the Qt Online Installer\n"
    "  • On macOS:         brew install qscintilla2-qt6\n"
  )
endif()


# Collect sources/headers
file(GLOB_RECURSE KODETRON_SOURCES src/*.cpp)
file(GLOB_RECURSE KODETRON_HEADERS src/*.h)

add_executable(${PROJECT_NAME}
    ${KODETRON_SOURCES}
    ${KODETRON_HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/menus
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/toolbar
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    QScintilla2Qt6::QScintilla2Qt6
)

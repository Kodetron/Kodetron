@echo off
echo Compilando Kodetron con soporte de base de datos...

REM Configurar variables
set QT_DIR=C:\Qt\6.5.0\mingw_64
set MINGW_DIR=C:\msys64\ucrt64
set BUILD_DIR=build_manual

REM Crear directorio de compilación
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

REM Configurar PATH para Qt y MinGW
set PATH=%QT_DIR%\bin;%MINGW_DIR%\bin;%PATH%

echo Verificando herramientas...
where g++
where qmake

echo.
echo Compilando archivos fuente...

REM Crear archivo .pro de Qt
echo TEMPLATE = app > %BUILD_DIR%\Kodetron.pro
echo TARGET = Kodetron >> %BUILD_DIR%\Kodetron.pro
echo QT += widgets sql >> %BUILD_DIR%\Kodetron.pro
echo CONFIG += c++17 >> %BUILD_DIR%\Kodetron.pro
echo.>> %BUILD_DIR%\Kodetron.pro
echo SOURCES += \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/main.cpp \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/App/App.cpp \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/MainWindow/MainWindow.cpp \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/utils/StyleLoader/StyleReader.cpp \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/widgets/CodeEditor.cpp \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/widgets/LineNumberArea.cpp \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/widgets/menus/FileMenuActions.cpp \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/widgets/toolbar/ToolbarManager.cpp \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/DataBase/DatabaseManager.cpp \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/DataBase/Models.cpp \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/DataBase/UserRepository.cpp \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/DataBase/ThemeRepository.cpp \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/DataBase/DatabaseService.cpp >> %BUILD_DIR%\Kodetron.pro
echo.>> %BUILD_DIR%\Kodetron.pro
echo HEADERS += \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/App/App.h \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/MainWindow/MainWindow.h \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/utils/StyleLoader/StyleReader.h \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/widgets/CodeEditor.h \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/widgets/LineNumberArea.h \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/widgets/menus/FileMenuActions.h \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/widgets/toolbar/ToolbarManager.h \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/DataBase/DatabaseManager.h \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/DataBase/Models.h \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/DataBase/UserRepository.h \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/DataBase/ThemeRepository.h \\ >> %BUILD_DIR%\Kodetron.pro
echo     ../src/DataBase/DatabaseService.h >> %BUILD_DIR%\Kodetron.pro

cd %BUILD_DIR%

echo.
echo Ejecutando qmake...
qmake Kodetron.pro

echo.
echo Compilando con make...
mingw32-make

echo.
echo ¡Compilación completada!
echo El ejecutable debe estar en: %BUILD_DIR%\release\Kodetron.exe o %BUILD_DIR%\debug\Kodetron.exe

cd ..
pause

@echo off
echo Compilando test de base de datos...

REM Configurar variables
set QT_DIR=C:\Qt\6.5.0\mingw_64
set MINGW_DIR=C:\msys64\ucrt64

REM Configurar PATH para Qt y MinGW
set PATH=%QT_DIR%\bin;%MINGW_DIR%\bin;%PATH%

echo Compilando database_test.cpp...

g++ -std=c++17 ^
    -I%QT_DIR%\include ^
    -I%QT_DIR%\include\QtCore ^
    -I%QT_DIR%\include\QtWidgets ^
    -I%QT_DIR%\include\QtSql ^
    -Isrc ^
    -Isrc\DataBase ^
    database_test.cpp ^
    src\DataBase\DatabaseManager.cpp ^
    src\DataBase\Models.cpp ^
    src\DataBase\UserRepository.cpp ^
    src\DataBase\ThemeRepository.cpp ^
    src\DataBase\DatabaseService.cpp ^
    -L%QT_DIR%\lib ^
    -lQt6Core -lQt6Widgets -lQt6Sql ^
    -o database_test.exe

if %ERRORLEVEL% == 0 (
    echo.
    echo ¡Compilación exitosa!
    echo Ejecutando test...
    echo.
    database_test.exe
) else (
    echo.
    echo Error en la compilación
)

pause

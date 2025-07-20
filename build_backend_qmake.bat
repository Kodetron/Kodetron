@echo off
REM =============================================================================
REM Kodetron Backend Build Script (Alternativo sin CMake)
REM Este script intenta compilar el backend usando qmake si CMake no está disponible
REM =============================================================================

echo.
echo =========================================
echo   🚀 Kodetron Backend Build (qmake)
echo =========================================
echo.

REM Verificar si estamos en el directorio correcto
if not exist "backend" (
    echo ❌ Error: Directorio backend no encontrado
    echo    Ejecuta este script desde el directorio raíz del proyecto Kodetron
    echo.
    pause
    exit /b 1
)

REM Verificar si qmake está disponible
qmake --version >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo ❌ Error: qmake no está disponible
    echo    Necesitas instalar Qt6 para usar qmake
    echo.
    echo 📋 Pasos para continuar:
    echo    1. Instalar Qt6 desde https://www.qt.io/download
    echo    2. Agregar Qt6/bin al PATH del sistema
    echo    3. O instalar CMake desde https://cmake.org/download/
    echo.
    pause
    exit /b 1
)

REM Crear archivo .pro temporal para compilar el backend
echo 📝 Creando archivo de proyecto Qt...
echo TARGET = kodetron_server > backend_temp.pro
echo CONFIG += c++17 console >> backend_temp.pro
echo CONFIG -= app_bundle >> backend_temp.pro
echo QT += core sql >> backend_temp.pro
echo. >> backend_temp.pro
echo SOURCES += backend/main_server.cpp \ >> backend_temp.pro
echo           backend/controllers/UsersController.cpp \ >> backend_temp.pro
echo           backend/controllers/ThemesController.cpp \ >> backend_temp.pro
echo           backend/controllers/SnippetsController.cpp \ >> backend_temp.pro
echo           backend/controllers/TemplatesController.cpp \ >> backend_temp.pro
echo           backend/controllers/ContestsController.cpp \ >> backend_temp.pro
echo           backend/controllers/CodeforcesController.cpp \ >> backend_temp.pro
echo           backend/middleware/AuthMiddleware.cpp \ >> backend_temp.pro
echo           backend/utils/JsonSerializer.cpp \ >> backend_temp.pro
echo           src/DataBase/DatabaseManager.cpp \ >> backend_temp.pro
echo           src/DataBase/Models.cpp \ >> backend_temp.pro
echo           src/DataBase/UserRepository.cpp \ >> backend_temp.pro
echo           src/DataBase/ThemeRepository.cpp >> backend_temp.pro
echo. >> backend_temp.pro
echo HEADERS += backend/controllers/UsersController.h \ >> backend_temp.pro
echo           backend/controllers/ThemesController.h \ >> backend_temp.pro
echo           backend/controllers/SnippetsController.h \ >> backend_temp.pro
echo           backend/controllers/TemplatesController.h \ >> backend_temp.pro
echo           backend/controllers/ContestsController.h \ >> backend_temp.pro
echo           backend/controllers/CodeforcesController.h \ >> backend_temp.pro
echo           backend/middleware/AuthMiddleware.h \ >> backend_temp.pro
echo           backend/utils/JsonSerializer.h \ >> backend_temp.pro
echo           src/DataBase/DatabaseManager.h \ >> backend_temp.pro
echo           src/DataBase/Models.h \ >> backend_temp.pro
echo           src/DataBase/UserRepository.h \ >> backend_temp.pro
echo           src/DataBase/ThemeRepository.h >> backend_temp.pro
echo. >> backend_temp.pro
echo INCLUDEPATH += src backend >> backend_temp.pro

echo ⚠️  NOTA: Este método NO incluye Crow framework
echo    El servidor no funcionará completamente sin Crow
echo    Instala CMake y Crow para compilación completa
echo.

REM Crear directorio de compilación
if not exist "build_backend_qmake" mkdir build_backend_qmake
cd build_backend_qmake

REM Generar Makefile
echo 🔧 Generando Makefile con qmake...
qmake ..\backend_temp.pro

if %ERRORLEVEL% neq 0 (
    echo ❌ Error durante la generación del Makefile
    cd ..
    del backend_temp.pro
    pause
    exit /b 1
)

REM Compilar (intentar con diferentes comandos make)
echo 🔨 Compilando backend...
if exist "C:\Qt\Tools\mingw1120_64\bin\mingw32-make.exe" (
    C:\Qt\Tools\mingw1120_64\bin\mingw32-make.exe
) else if exist "mingw32-make.exe" (
    mingw32-make
) else if exist "make.exe" (
    make
) else (
    echo ❌ No se encontró herramienta de compilación make
    echo    Instala Qt Creator o herramientas de desarrollo
    cd ..
    del backend_temp.pro
    pause
    exit /b 1
)

cd ..
del backend_temp.pro

if exist "build_backend_qmake\kodetron_server.exe" (
    echo.
    echo ⚠️  Compilación parcial exitosa (sin Crow)
    echo 📍 Ejecutable creado en: build_backend_qmake\kodetron_server.exe
    echo.
    echo 🚨 IMPORTANTE: Este ejecutable NO incluye el servidor HTTP
    echo    Solo contiene las clases de base de datos
    echo.
    echo 📋 Para funcionalidad completa:
    echo    1. Instalar CMake: https://cmake.org/download/
    echo    2. Instalar Crow: vcpkg install crow
    echo    3. Ejecutar: .\build_backend.bat
    echo.
) else (
    echo ❌ Error: La compilación falló
    echo    Revisa los errores de compilación arriba
)

pause

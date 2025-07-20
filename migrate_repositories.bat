@echo off
echo ============================================
echo    Kodetron Repository Migration Script
echo ============================================
echo.
echo This script will create two separate repositories:
echo 1. SQLite-scripts: Database schema and scripts
echo 2. kodetron-backend: Crow API backend
echo.

set CURRENT_DIR=%CD%
set SQLITE_REPO=SQLite-scripts
set BACKEND_REPO=kodetron-backend

echo [1/8] Creating repository directories...
if not exist "%SQLITE_REPO%" mkdir "%SQLITE_REPO%"
if not exist "%BACKEND_REPO%" mkdir "%BACKEND_REPO%"

echo [2/8] Setting up SQLite-scripts repository structure...
cd "%SQLITE_REPO%"
mkdir schema 2>nul
mkdir docs 2>nul
mkdir tests 2>nul
mkdir scripts 2>nul
mkdir migrations 2>nul

echo [3/8] Copying SQLite database schema and files...
echo   - Copying database schema...
copy "%CURRENT_DIR%\src\DataBase\DataBase" "schema\01_create_tables.sql" >nul
echo   - Copying database test...
copy "%CURRENT_DIR%\database_test.cpp" "tests\database_test.cpp" >nul
echo   - Copying setup script...
copy "%CURRENT_DIR%\test_database.bat" "scripts\setup_database.bat" >nul

echo [4/8] Creating SQLite repository documentation...
echo # Kodetron SQLite Database Scripts > README.md
echo. >> README.md
echo This repository contains the SQLite database schema and related scripts for the Kodetron project. >> README.md
echo. >> README.md
echo ## Quick Start >> README.md
echo. >> README.md
echo 1. Create database: `sqlite3 kodetron.db ^< schema/01_create_tables.sql` >> README.md
echo 2. Run tests: `scripts\setup_database.bat` >> README.md
echo. >> README.md
echo See [GitHub repository](https://github.com/Kodetron/SQLite-scripts.git) for full documentation. >> README.md

echo [5/8] Setting up backend repository structure...
cd "%CURRENT_DIR%\%BACKEND_REPO%"
mkdir controllers 2>nul
mkdir middleware 2>nul
mkdir utils 2>nul
mkdir database 2>nul

echo [6/8] Copying backend files...
echo   - Copying main server...
copy "%CURRENT_DIR%\backend\main_server.cpp" . >nul
copy "%CURRENT_DIR%\backend\CMakeLists.txt" . >nul
copy "%CURRENT_DIR%\backend\README.md" . >nul

echo   - Copying controllers...
xcopy "%CURRENT_DIR%\backend\controllers\*" "controllers\" /Y >nul

echo   - Copying middleware...
xcopy "%CURRENT_DIR%\backend\middleware\*" "middleware\" /Y >nul

echo   - Copying utils...
xcopy "%CURRENT_DIR%\backend\utils\*" "utils\" /Y >nul

echo [7/8] Copying database C++ files...
echo   - Copying database headers...
copy "%CURRENT_DIR%\src\DataBase\*.h" "database\" >nul
echo   - Copying database implementation...
copy "%CURRENT_DIR%\src\DataBase\*.cpp" "database\" >nul

echo   - Copying build scripts...
copy "%CURRENT_DIR%\build_backend.bat" . >nul
copy "%CURRENT_DIR%\build_backend_qmake.bat" . >nul
copy "%CURRENT_DIR%\compile_with_database.bat" . >nul

echo [8/8] Creating backend repository documentation...
echo # Kodetron Crow Backend API > README.md
echo. >> README.md
echo REST API backend for Kodetron competitive programming IDE. >> README.md
echo. >> README.md
echo ## Features >> README.md
echo - User management >> README.md
echo - Theme management >> README.md
echo - Template and snippet storage >> README.md
echo - Codeforces integration >> README.md
echo. >> README.md
echo ## Quick Start >> README.md
echo. >> README.md
echo 1. Install dependencies: Crow framework, SQLite3 >> README.md
echo 2. Build: `build_backend.bat` >> README.md
echo 3. Run: `./backend_server` >> README.md
echo. >> README.md
echo See [GitHub repository](https://github.com/Kodetron/Crow-server-cpp.git) for full documentation. >> README.md

cd "%CURRENT_DIR%"

echo.
echo ============================================
echo         Migration completed successfully!
echo ============================================
echo.
echo Repository structures created:
echo.
echo %SQLITE_REPO%/
echo ├── schema/01_create_tables.sql
echo ├── tests/database_test.cpp
echo ├── scripts/setup_database.bat
echo └── README.md
echo.
echo %BACKEND_REPO%/
echo ├── main_server.cpp
echo ├── controllers/
echo ├── middleware/
echo ├── utils/
echo ├── database/
echo └── README.md
echo.
echo ============================================
echo                 Next Steps
echo ============================================
echo.
echo For SQLite-scripts repository:
echo   1. cd %SQLITE_REPO%
echo   2. git init
echo   3. git remote add origin https://github.com/Kodetron/SQLite-scripts.git
echo   4. git add .
echo   5. git commit -m "Initial database schema and scripts"
echo   6. git push -u origin main
echo.
echo For kodetron-backend repository:
echo   1. cd %BACKEND_REPO%
echo   2. git init
echo   3. git remote add origin https://github.com/Kodetron/Crow-server-cpp.git
echo   4. git add .
echo   5. git commit -m "Initial Crow backend implementation"
echo   6. git push -u origin main
echo.
echo ============================================
echo.
pause

@echo off
REM =============================================================================
REM Kodetron Backend Build Script
REM This script automatically compiles the Kodetron HTTP backend server
REM =============================================================================

echo.
echo =========================================
echo   🚀 Kodetron Backend Build Script
echo =========================================
echo.

REM Check if we're in the correct directory
if not exist "CMakeLists.txt" (
    echo ❌ Error: CMakeLists.txt not found
    echo    Run this script from the Kodetron project root directory
    echo.
    pause
    exit /b 1
)

REM Create build directory
echo 📁 Creating build directory...
if not exist "build_backend" mkdir build_backend
cd build_backend

REM Configure with CMake
echo 🔧 Configuring project with CMake...
cmake .. -DCMAKE_BUILD_TYPE=Release

if %ERRORLEVEL% neq 0 (
    echo.
    echo ❌ Error during CMake configuration
    echo    Possible solutions:
    echo    1. Verify Qt6 is installed
    echo    2. Install Crow framework: vcpkg install crow
    echo    3. Configure CMAKE_TOOLCHAIN_FILE if using vcpkg
    echo.
    pause
    exit /b 1
)

REM Compile the backend
echo 🔨 Compiling backend server...
cmake --build . --target kodetron_server --config Release

if %ERRORLEVEL% neq 0 (
    echo.
    echo ❌ Error during compilation
    echo    Review compilation errors above
    echo.
    pause
    exit /b 1
)

REM Verify executable was created
if exist "Release\kodetron_server.exe" (
    echo.
    echo ✅ Compilation successful!
    echo 📍 Executable created at: build_backend\Release\kodetron_server.exe
    echo.
    echo 🚀 To run the server:
    echo    cd build_backend\Release
    echo    .\kodetron_server.exe
    echo.
    echo 🌐 Server will be available at: http://localhost:8080
    echo 📚 Complete documentation at: BACKEND_GUIDE.md
    echo.
) else (
    echo ❌ Error: Executable was not created correctly
    echo    Review compilation errors
    pause
    exit /b 1
)

cd ..
pause

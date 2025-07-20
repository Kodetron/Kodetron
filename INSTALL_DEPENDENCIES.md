# 📦 Dependencies Installation - Kodetron

This guide will help you install all necessary dependencies to compile and run Kodetron.

## 🎯 Required Dependencies

### ✅ Mandatory (Qt Desktop Client)
- **Qt6** (6.2 or higher)
  - Qt6::Core
  - Qt6::Widgets  
  - Qt6::Sql
  - Qt6::Network
- **CMake** (3.10 or higher)
- **C++17 Compiler**

### 🌐 Optional (HTTP Backend API)
- **Crow Framework** 
- **vcpkg** (package manager, recommended)

---

## 🖥️ Windows

### 1. Install Qt6

#### Option A: Qt Online Installer (Recommended)
```bash
# Download from: https://www.qt.io/download
# Run qt-online-installer-windows.exe
# Select Qt 6.5+ with the following components:
# - Qt 6.5.x Desktop (MinGW or MSVC)
# - Qt Creator (optional but useful)
# - CMake (important)
```

#### Option B: Winget
```powershell
winget install Qt.Qt
```

### 2. Install CMake

#### Option A: From Qt Installer
- Already included if you selected CMake in Qt installer

#### Option B: Direct download
```bash
# Download from: https://cmake.org/download/
# Run cmake-windows-installer.msi
# Add CMake to system PATH
```

#### Option C: Winget
```powershell
winget install Kitware.CMake
```

### 3. Verify basic installation
```powershell
qmake --version
cmake --version
g++ --version  # or cl.exe for MSVC
```

### 4. Compilar cliente Qt Desktop
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --target kodetron
```

### 5. Instalar Crow Framework (Para Backend)

#### Opción A: vcpkg (Recomendado)
```powershell
# Instalar vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Instalar Crow
.\vcpkg install crow:x64-windows

# Configurar para CMake
$env:CMAKE_TOOLCHAIN_FILE = "C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake"
```

#### Opción B: Compilación manual
```powershell
git clone https://github.com/CrowCpp/Crow.git
cd Crow
mkdir build
cd build
cmake .. -DCROW_BUILD_EXAMPLES=OFF
cmake --build . --config Release
cmake --install .
```

### 6. Compilar backend con Crow
```bash
# Configurar variable de entorno
set CMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake

# Compilar
.\build_backend.bat
```

---

## 🐧 Linux (Ubuntu/Debian)

### 1. Actualizar sistema
```bash
sudo apt update && sudo apt upgrade -y
```

### 2. Instalar Qt6 y herramientas
```bash
# Qt6 completo
sudo apt install qt6-base-dev qt6-tools-dev

# Módulos adicionales
sudo apt install qt6-base-dev-tools libqt6sql6-dev libqt6network6-dev

# CMake y compilador
sudo apt install cmake build-essential g++

# Git (si no está instalado)
sudo apt install git
```

### 3. Verificar instalación
```bash
qmake6 --version  # Nota: puede ser qmake6 en lugar de qmake
cmake --version
g++ --version
```

### 4. Compilar cliente Qt Desktop
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make kodetron
```

### 5. Instalar Crow Framework

#### Opción A: Desde paquetes (si está disponible)
```bash
# Verificar disponibilidad
apt search crow

# Si no está disponible, usar compilación manual
```

#### Opción B: vcpkg
```bash
# Instalar vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh

# Instalar Crow
./vcpkg install crow

# Configurar
export CMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

#### Opción C: Compilación manual
```bash
# Dependencias de Crow
sudo apt install libasio-dev

# Compilar Crow
git clone https://github.com/CrowCpp/Crow.git
cd Crow
mkdir build && cd build
cmake .. -DCROW_BUILD_EXAMPLES=OFF
make -j$(nproc)
sudo make install
```

### 6. Compilar backend
```bash
mkdir build_backend && cd build_backend
cmake .. -DCMAKE_BUILD_TYPE=Release
make kodetron_server
```

---

## 🍎 macOS

### 1. Instalar Homebrew (si no está instalado)
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 2. Instalar Qt6 y herramientas
```bash
# Qt6 completo
brew install qt@6

# CMake
brew install cmake

# Herramientas de desarrollo Xcode
xcode-select --install
```

### 3. Configurar variables de entorno
```bash
# Agregar a ~/.zshrc o ~/.bash_profile
export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"
export CMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@6"
```

### 4. Verificar instalación
```bash
qmake --version
cmake --version
clang++ --version
```

### 5. Compilar cliente Qt Desktop
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make kodetron
```

### 6. Instalar Crow Framework

#### Opción A: Homebrew (si está disponible)
```bash
brew search crow
# Si no está disponible, usar vcpkg o compilación manual
```

#### Opción B: vcpkg
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install crow
export CMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### 7. Compilar backend
```bash
mkdir build_backend && cd build_backend
cmake .. -DCMAKE_BUILD_TYPE=Release
make kodetron_server
```

---

## 🔧 Verificación de Instalación Completa

### 1. Verificar cliente Qt Desktop
```bash
# Después de compilar
cd build
./kodetron  # Linux/macOS
.\kodetron.exe  # Windows

# Debería abrir la interfaz gráfica de Kodetron
```

### 2. Verificar backend HTTP
```bash
# Después de compilar backend
cd build_backend
./kodetron_server  # Linux/macOS
.\Release\kodetron_server.exe  # Windows

# Debería mostrar:
# === Kodetron API Server ===
# Starting Kodetron API Server on port 8080
```

### 3. Probar API del backend
```bash
curl http://localhost:8080/health
# Respuesta: "Kodetron API Server is running"

curl http://localhost:8080/api
# Respuesta: JSON con información de la API
```

---

## ❌ Solución de Problemas Comunes

### Qt6 no encontrado
```bash
# Error: Could not find Qt6
# Solución: Configurar CMAKE_PREFIX_PATH

# Windows
set CMAKE_PREFIX_PATH=C:\Qt\6.5.0\mingw_64

# Linux
export CMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6

# macOS  
export CMAKE_PREFIX_PATH=/opt/homebrew/opt/qt@6
```

### CMake no encontrado
```bash
# Windows: Reinstalar Qt con CMake incluido
# Linux: sudo apt install cmake
# macOS: brew install cmake
```

### Crow Framework no encontrado
```bash
# Usar vcpkg:
vcpkg install crow
cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg.cmake

# O compilar solo cliente Qt sin backend:
cmake .. -DSKIP_BACKEND=ON
```

### Errores de compilación C++17
```bash
# Verificar versión del compilador
g++ --version  # Debe ser 8+ 
clang++ --version  # Debe ser 7+

# Forzar C++17
cmake .. -DCMAKE_CXX_STANDARD=17
```

### Base de datos SQLite
```bash
# Si hay problemas con SQLite, verificar:
# Linux: sudo apt install libsqlite3-dev
# macOS: brew install sqlite
# Windows: Incluido con Qt6
```

---

## 🚀 Scripts Automatizados

### Windows - Instalación completa
```powershell
# install_dependencies.ps1
winget install Qt.Qt
winget install Kitware.CMake
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install crow:x64-windows
```

### Linux - Instalación completa
```bash
#!/bin/bash
# install_dependencies.sh
sudo apt update
sudo apt install -y qt6-base-dev qt6-tools-dev cmake build-essential
sudo apt install -y libqt6sql6-dev libqt6network6-dev
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.sh
./vcpkg install crow
```

---

¡Una vez instaladas todas las dependencias, podrás compilar y ejecutar Kodetron completo! 🎉

Para continuar, consulta:
- **[BACKEND_GUIDE.md](./BACKEND_GUIDE.md)** - Guía completa de uso
- **[API_EXAMPLES.md](./API_EXAMPLES.md)** - Ejemplos prácticos

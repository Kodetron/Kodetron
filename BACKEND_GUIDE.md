# 🚀 Kodetron Backend API - Complete Guide

## 📋 Table of Contents
1. [Prerequisites](#prerequisites)
2. [Installation and Configuration](#installation-and-configuration)
3. [Backend Compilation](#backend-compilation)
4. [Server Execution](#server-execution)
5. [API Endpoints](#api-endpoints)
6. [Query Examples](#query-examples)
7. [Qt Desktop Integration](#qt-desktop-integration)
8. [Troubleshooting](#troubleshooting)

---

## 📦 Prerequisites

### Required Software
- **Qt6** (6.2 or higher) with modules:
  - Qt6::Core
  - Qt6::Sql
  - Qt6::Network
- **CMake** (3.10 or higher)
- **C++17 compatible compiler**:
  - MSVC 2019/2022 (Windows)
  - GCC 8+ (Linux)
  - Clang 7+ (macOS)
- **Crow Framework** (for HTTP backend)

### Crow Framework Installation

#### Option 1: vcpkg (Recommended)
```bash
# Install vcpkg if you don't have it
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat  # Windows
# ./bootstrap-vcpkg.sh   # Linux/macOS

# Install Crow
.\vcpkg install crow
```

#### Option 2: Manual compilation
```bash
git clone https://github.com/CrowCpp/Crow.git
cd Crow
mkdir build
cd build
cmake .. -DCROW_BUILD_EXAMPLES=OFF
cmake --build .
cmake --install .
```

---

## 🔧 Installation and Configuration

### 1. Clone the project
```bash
git clone https://github.com/Kodetron/Kodetron.git
cd Kodetron
```

### 2. Configure environment variables (if using vcpkg)
```bash
# Windows
set CMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

# Linux/macOS
export CMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### 3. Verify project structure
```
Kodetron/
├── backend/              # HTTP server code
├── src/                  # Qt Desktop application
├── CMakeLists.txt        # Main configuration
└── build_backend.bat     # Build script
```

---

## 🔨 Backend Compilation

### Method 1: Automatic Script (Windows)
```bash
# From project root directory
.\build_backend.bat
```

### Method 2: Manual CMake
```bash
# Create build directory
mkdir build_backend
cd build_backend

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compile
cmake --build . --target kodetron_server --config Release

# On Linux/macOS you can also use:
make kodetron_server
```

### Verify successful compilation
The executable should be in:
- **Windows**: `build_backend/Release/kodetron_server.exe`
- **Linux/macOS**: `build_backend/kodetron_server`

---

## 🚀 Server Execution

### Start the server
```bash
# Method 1: Default port (8080)
.\build_backend\Release\kodetron_server.exe

# Method 2: Custom port
.\build_backend\Release\kodetron_server.exe 9000

# Linux/macOS
./build_backend/kodetron_server
./build_backend/kodetron_server 9000
```

### Verify server is running
```bash
# Server health check
curl http://localhost:8080/health

# Expected response:
# Kodetron API Server is running
```

### Server output
```
=== Kodetron API Server ===
Initializing database...
Database initialized successfully!
Setting up API routes...
All routes configured successfully!
Starting Kodetron API Server on port 8080
API documentation available at: http://localhost:8080/api
```

---

## 📡 Endpoints de la API

### 🔍 Información General
| Endpoint | Método | Descripción |
|----------|--------|-------------|
| `/health` | GET | Estado del servidor |
| `/api` | GET | Información de la API |

### 👥 Gestión de Usuarios
| Endpoint | Método | Descripción | Estado |
|----------|--------|-------------|-------|
| `/api/users` | GET | Obtener todos los usuarios | ✅ Completo |
| `/api/users` | POST | Crear nuevo usuario | ✅ Completo |
| `/api/users/{id}` | GET | Obtener usuario por ID | ✅ Completo |
| `/api/users/{id}` | PUT | Actualizar usuario | ✅ Completo |
| `/api/users/{id}` | DELETE | Eliminar usuario | ✅ Completo |

### 🎨 Gestión de Temas
| Endpoint | Método | Descripción | Estado |
|----------|--------|-------------|-------|
| `/api/themes` | GET | Obtener temas (con filtro user_id) | ✅ Completo |
| `/api/themes` | POST | Crear nuevo tema | ✅ Completo |
| `/api/themes/{id}` | GET | Obtener tema por ID | ✅ Completo |
| `/api/themes/{id}` | PUT | Actualizar tema | ✅ Completo |
| `/api/themes/{id}` | DELETE | Eliminar tema | ✅ Completo |

### 📝 Otras Entidades (Pendientes)
| Endpoint | Método | Descripción | Estado |
|----------|--------|-------------|-------|
| `/api/snippets` | GET/POST/PUT/DELETE | Gestión de snippets | 🔄 En desarrollo |
| `/api/templates` | GET/POST/PUT/DELETE | Gestión de plantillas | 🔄 En desarrollo |
| `/api/contests` | GET/POST | Gestión de concursos | 🔄 En desarrollo |
| `/api/codeforces` | GET/POST | Cuentas de Codeforces | 🔄 En desarrollo |

---

## 💡 Ejemplos de Consultas

### 🔍 Verificación del Servidor

#### Verificar estado del servidor
```bash
curl http://localhost:8080/health
```

#### Obtener información de la API
```bash
curl http://localhost:8080/api
```

### 👥 Operaciones con Usuarios

#### 1. Crear un nuevo usuario
```bash
curl -X POST http://localhost:8080/api/users \
  -H "Content-Type: application/json" \
  -d '{
    "username": "desarrollador123",
    "email": "dev@kodetron.com"
  }'
```

**Respuesta esperada:**
```json
{
  "id": 1,
  "username": "desarrollador123",
  "email": "dev@kodetron.com",
  "created_at": "2025-07-20T15:30:00"
}
```

#### 2. Obtener todos los usuarios
```bash
curl http://localhost:8080/api/users
```

#### 3. Obtener usuario específico
```bash
curl http://localhost:8080/api/users/1
```

#### 4. Actualizar usuario
```bash
curl -X PUT http://localhost:8080/api/users/1 \
  -H "Content-Type: application/json" \
  -d '{
    "username": "desarrollador_pro",
    "email": "pro@kodetron.com"
  }'
```

#### 5. Eliminar usuario
```bash
curl -X DELETE http://localhost:8080/api/users/1
```

### 🎨 Operaciones con Temas

#### 1. Crear un tema personalizado
```bash
curl -X POST http://localhost:8080/api/themes \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": 1,
    "name": "Tema Oscuro Personalizado",
    "qss_content": "QWidget { background-color: #1e1e1e; color: #ffffff; } QTextEdit { background-color: #2d2d2d; border: 1px solid #555; }"
  }'
```

#### 2. Obtener todos los temas de un usuario
```bash
curl "http://localhost:8080/api/themes?user_id=1"
```

#### 3. Obtener tema específico
```bash
curl http://localhost:8080/api/themes/1
```

#### 4. Actualizar tema existente
```bash
curl -X PUT http://localhost:8080/api/themes/1 \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Tema Oscuro Mejorado",
    "qss_content": "QWidget { background-color: #121212; color: #e0e0e0; }"
  }'
```

#### 5. Eliminar tema
```bash
curl -X DELETE http://localhost:8080/api/themes/1
```

### 🔧 Ejemplos con PowerShell (Windows)

#### Crear usuario con PowerShell
```powershell
$body = @{
    username = "usuario_windows"
    email = "windows@kodetron.com"
} | ConvertTo-Json

Invoke-RestMethod -Uri "http://localhost:8080/api/users" `
  -Method POST `
  -Body $body `
  -ContentType "application/json"
```

#### Obtener temas con PowerShell
```powershell
Invoke-RestMethod -Uri "http://localhost:8080/api/themes?user_id=1" `
  -Method GET
```

---

## 🖥️ Integración con Qt Desktop

### Uso del HttpApiClient en tu aplicación Qt

#### 1. Incluir en tu proyecto
```cpp
#include "DataBase/HttpApiClient.h"
```

#### 2. Crear instancia del cliente
```cpp
// En tu MainWindow o clase principal
HttpApiClient* apiClient = new HttpApiClient("http://localhost:8080/api", this);
```

#### 3. Verificar conexión con el servidor
```cpp
void MainWindow::checkServerConnection() {
    apiClient->checkHealth([this](bool healthy, QString message) {
        if (healthy) {
            qDebug() << "✅ Servidor conectado:" << message;
            statusLabel->setText("🟢 Servidor online");
        } else {
            qDebug() << "❌ Error de servidor:" << message;
            statusLabel->setText("🔴 Servidor offline");
        }
    });
}
```

#### 4. Sincronizar temas con el servidor
```cpp
void MainWindow::syncThemesFromServer() {
    int userId = getCurrentUserId(); // Tu función para obtener user ID
    
    apiClient->getThemes(userId, [this](QList<Theme> themes, QString error) {
        if (error.isEmpty()) {
            qDebug() << "📥 Descargados" << themes.size() << "temas del servidor";
            
            // Procesar temas descargados
            for (const Theme& theme : themes) {
                addThemeToLocalList(theme);
            }
            
            refreshThemeMenu();
        } else {
            QMessageBox::warning(this, "Error de Sincronización", 
                               "No se pudieron descargar los temas: " + error);
        }
    });
}
```

#### 5. Guardar tema en el servidor
```cpp
void MainWindow::saveThemeToServer(const QString& themeName, const QString& qssContent) {
    Theme theme;
    theme.userId = getCurrentUserId();
    theme.name = themeName;
    theme.qssContent = qssContent;
    
    apiClient->createTheme(theme, [this, themeName](Theme createdTheme, QString error) {
        if (error.isEmpty()) {
            QMessageBox::information(this, "Éxito", 
                                   "Tema '" + themeName + "' guardado en el servidor");
            syncThemesFromServer(); // Refrescar lista
        } else {
            QMessageBox::warning(this, "Error", 
                               "No se pudo guardar el tema: " + error);
        }
    });
}
```

---

## 🔧 Solución de Problemas

### Error: "Crow not found"
```bash
# Solución 1: Instalar con vcpkg
vcpkg install crow

# Solución 2: Especificar toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg.cmake

# Solución 3: Compilar sin backend
# El proyecto Qt Desktop funcionará sin el servidor HTTP
```

### Error: "Database initialization failed"
```bash
# Verificar permisos de escritura en el directorio
# El servidor crea 'kodetron_server.db' automáticamente
ls -la kodetron_server.db

# Eliminar base de datos corrupta
rm kodetron_server.db
# Reiniciar servidor para recrear
```

### Error: "Port already in use"
```bash
# Verificar qué proceso usa el puerto
netstat -ano | findstr :8080

# Usar puerto diferente
.\kodetron_server.exe 9000

# O terminar proceso existente
taskkill /PID <PID_NUMBER> /F
```

### Error de CORS en navegador web
```javascript
// El servidor ya incluye headers CORS
// Si persiste, verificar que el servidor esté ejecutándose
fetch('http://localhost:8080/health')
  .then(response => response.text())
  .then(data => console.log('Servidor:', data))
  .catch(error => console.error('Error:', error));
```

### Error: "Qt6::Network not found"
```bash
# Instalar módulo Network de Qt6
# Ubuntu/Debian:
sudo apt-get install qt6-base-dev qt6-networkauth-dev

# Windows: Reinstalar Qt6 con módulo Network
# macOS: brew install qt6
```

---

## 📊 Monitoreo y Logs

### Ver logs del servidor
El servidor muestra información detallada en la consola:
```
=== Kodetron API Server ===
Initializing database...
Database initialized successfully!
Setting up API routes...
All routes configured successfully!
Starting Kodetron API Server on port 8080
```

### Verificar base de datos
```bash
# La base de datos SQLite se crea automáticamente
ls -la kodetron_server.db

# Ver contenido (requiere sqlite3)
sqlite3 kodetron_server.db ".tables"
sqlite3 kodetron_server.db "SELECT * FROM users;"
```

---

## 🌐 Pruebas con Herramientas Web

### Postman Collection
```json
{
  "info": { "name": "Kodetron API" },
  "item": [
    {
      "name": "Health Check",
      "request": {
        "method": "GET",
        "header": [],
        "url": "http://localhost:8080/health"
      }
    },
    {
      "name": "Create User",
      "request": {
        "method": "POST",
        "header": [{"key": "Content-Type", "value": "application/json"}],
        "body": {
          "raw": "{\"username\":\"test_user\",\"email\":\"test@example.com\"}"
        },
        "url": "http://localhost:8080/api/users"
      }
    }
  ]
}
```

### Insomnia/Thunder Client
- URL Base: `http://localhost:8080`
- Headers: `Content-Type: application/json`
- Body (JSON): Ver ejemplos de cada endpoint

---

## 🚀 Despliegue en Producción

### Configuraciones recomendadas
```bash
# Ejecutar en puerto estándar con logs
./kodetron_server 80 > server.log 2>&1 &

# Con systemd (Linux)
sudo systemctl enable kodetron-api
sudo systemctl start kodetron-api

# Con Docker
docker build -t kodetron-api .
docker run -p 8080:8080 kodetron-api
```

### Seguridad
- Implementar autenticación JWT real
- Configurar HTTPS/TLS
- Validar todas las entradas
- Limitar rate limiting
- Usar base de datos PostgreSQL/MySQL en producción

---

¡Tu backend Kodetron está listo para usar! 🎉

Para soporte adicional, consulta el código fuente en `backend/` o crea un issue en el repositorio.

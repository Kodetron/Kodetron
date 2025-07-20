# 🚀 Kodetron IDE

<img src="assets/Logo.png" width="200">

**Specialized code editor for competitive programming in C++**

## 👥 Development Team
**Natilla de la Abuela**

### Members
- Daniel Libardo Diaz Gonzalez - dandiazgo@unal.edu.co ([DaniDiazTech](https://danidiaztech.com/))
- Andrés Felipe León Sánchez - anleonsa@unal.edu.co ([Anfelesan](https://anfelesan.netlify.app/))
- Alejandro Medina Rojas - alemedinaro@unal.edu.co
- Angel David Ruiz Barbosa - aruizba@unal.edu.co

## 📝 Description
Kodetron is a specialized text editor for C++ development in the context of competitive programming, with both local capabilities and cloud synchronization.

## 🏗️ System Architecture

### 🖥️ Qt Desktop Client
- **Framework**: Qt6 (Core, Widgets, SQL, Network)
- **Local database**: SQLite with 7 tables
- **Synchronization**: HttpApiClient for backend communication

### 🌐 HTTP Backend API
- **Framework**: Crow (C++ REST API)
- **Database**: Shared SQLite
- **Endpoints**: RESTful for users, themes, snippets, etc.

## 🚀 Quick Start

### 1. Compile Qt Desktop application
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --target kodetron
```

### 2. Compile and run backend API
```bash
# Windows
.\build_backend.bat

# Linux/macOS
mkdir build_backend && cd build_backend
cmake .. -DCMAKE_BUILD_TYPE=Release
make kodetron_server
./kodetron_server
```

### 3. Verify backend is working
```bash
curl http://localhost:8080/health
# Response: "Kodetron API Server is running"
```

## 📚 Documentation

### 📖 Main Documentation
- **[Documentation Repository](https://github.com/Kodetron/Documentation/tree/main)** - Planning, use cases, and development journey
- **[BACKEND_GUIDE.md](./BACKEND_GUIDE.md)** - Complete HTTP API backend guide
- **[API_EXAMPLES.md](./API_EXAMPLES.md)** - Practical API query examples
- **[backend/README.md](./backend/README.md)** - Technical backend summary

### 🔗 Quick Links
- **Backend API**: http://localhost:8080/api
- **Health Check**: http://localhost:8080/health
- **Project Status**: Users and Themes ✅ | Snippets/Templates 🔄

## ✨ Características Implementadas

### 🖥️ Cliente Desktop Qt
- [x] Editor de texto con resaltado de sintaxis C++
- [x] Soporte multi-archivo con pestañas
- [x] Explorador de proyectos para navegación
- [x] Compilación de archivos C++ con captura de errores
- [x] Ejecución de binarios con soporte stdin/stdout
- [x] Paneles editables para simulación de casos de prueba
- [x] Operaciones de sistema de archivos
- [x] **Base de datos SQLite local integrada**
- [x] **Gestión de usuarios y configuraciones**
- [x] **Sistema de temas personalizables (QSS)**

### 🌐 Backend HTTP API  
- [x] **Servidor REST con Crow framework**
- [x] **CRUD completo para usuarios** (`/api/users`)
- [x] **CRUD completo para temas** (`/api/themes`)
- [x] **Middleware CORS y autenticación**
- [x] **Serialización JSON manual**
- [x] **Base de datos SQLite compartida**
- [ ] CRUD para snippets (estructura creada)
- [ ] CRUD para templates (estructura creada)
- [ ] CRUD para contests (estructura creada)
- [ ] CRUD para cuentas Codeforces (estructura creada)

### 🔄 Integración Cliente-Servidor
- [x] **HttpApiClient en Qt para comunicación**
- [x] **Sincronización de temas entre local y servidor**
- [x] **Verificación de salud del servidor**
- [x] **Manejo de errores de red**

## 🗄️ Esquema de Base de Datos

```sql
-- Usuarios del sistema
CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    email TEXT UNIQUE NOT NULL,
    created_at TEXT DEFAULT CURRENT_TIMESTAMP
);

-- Temas personalizados (QSS)
CREATE TABLE themes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER REFERENCES users(id),
    name TEXT NOT NULL,
    qss_content TEXT NOT NULL,
    created_at TEXT DEFAULT CURRENT_TIMESTAMP
);

-- Snippets de código reutilizable
CREATE TABLE snippets (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER REFERENCES users(id),
    name TEXT NOT NULL,
    language TEXT NOT NULL,
    content TEXT NOT NULL,
    description TEXT,
    created_at TEXT DEFAULT CURRENT_TIMESTAMP
);

-- Plantillas de código
CREATE TABLE templates (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER REFERENCES users(id),
    name TEXT NOT NULL,
    content TEXT NOT NULL,
    language TEXT NOT NULL,
    description TEXT,
    created_at TEXT DEFAULT CURRENT_TIMESTAMP
);

-- Y más tablas para contests, codeforces_accounts, settings...
```

## 🔧 Ejemplos de Uso del API

### Crear un usuario
```bash
curl -X POST http://localhost:8080/api/users \
  -H "Content-Type: application/json" \
  -d '{"username": "developer", "email": "dev@kodetron.com"}'
```

### Crear un tema personalizado
```bash
curl -X POST http://localhost:8080/api/themes \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": 1,
    "name": "Tema Oscuro",
    "qss_content": "QWidget { background-color: #1e1e1e; color: #ffffff; }"
  }'
```

### Obtener temas de un usuario
```bash
curl "http://localhost:8080/api/themes?user_id=1"
```

## 🎯 Objetivos
The main goal of Kodetron is to provide programmers with a specialized tool that facilitates fast, precise, and collaborative code development tailored to the demands of competitive programming.

>>>>>>> upstream/main

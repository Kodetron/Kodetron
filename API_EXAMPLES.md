# 📡 Kodetron API - Query Examples

This guide contains practical examples of all available queries in the Kodetron API.

## 🔗 Base URL
```
http://localhost:8080
```

---

## 🏥 1. Server Verification

### ✅ Server status
```bash
curl http://localhost:8080/health
```
**Expected response:**
```
Kodetron API Server is running
```

### 📋 API information
```bash
curl http://localhost:8080/api
```
**Expected response:**
```json
{
  "name": "Kodetron API",
  "version": "1.0.0",
  "description": "REST API for Kodetron code editor",
  "endpoints": {
    "health": "/health",
    "users": "/api/users",
    "themes": "/api/themes"
  }
}
```

---

## 👥 2. User Management

### 📤 Create new user
```bash
curl -X POST http://localhost:8080/api/users \
  -H "Content-Type: application/json" \
  -d '{
    "username": "developer123",
    "email": "dev@kodetron.com"
  }'
```

**Expected response:**
```json
{
  "id": 1,
  "username": "developer123",
  "email": "dev@kodetron.com",
  "created_at": "2025-07-20T15:30:00"
}
```

### 📥 Get all users
```bash
curl http://localhost:8080/api/users
```

**Expected response:**
```json
[
  {
    "id": 1,
    "username": "developer123",
    "email": "dev@kodetron.com",
    "created_at": "2025-07-20T15:30:00"
  },
  {
    "id": 2,
    "username": "demo_user",
    "email": "demo@kodetron.com",
    "created_at": "2025-07-20T16:00:00"
  }
]
```

### 🔍 Obtener usuario específico
```bash
curl http://localhost:8080/api/users/1
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

### ✏️ Actualizar usuario existente
```bash
curl -X PUT http://localhost:8080/api/users/1 \
  -H "Content-Type: application/json" \
  -d '{
    "username": "desarrollador_pro",
    "email": "pro@kodetron.com"
  }'
```

**Respuesta esperada:**
```json
{
  "id": 1,
  "username": "desarrollador_pro",
  "email": "pro@kodetron.com",
  "created_at": "2025-07-20T15:30:00"
}
```

### 🗑️ Eliminar usuario
```bash
curl -X DELETE http://localhost:8080/api/users/1
```

**Respuesta esperada:**
```json
{
  "message": "User deleted successfully"
}
```

---

## 🎨 3. Gestión de Temas

### 📤 Crear nuevo tema
```bash
curl -X POST http://localhost:8080/api/themes \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": 1,
    "name": "Tema Oscuro Personalizado",
    "qss_content": "QWidget { background-color: #1e1e1e; color: #ffffff; } QTextEdit { background-color: #2d2d2d; border: 1px solid #555; }"
  }'
```

**Respuesta esperada:**
```json
{
  "id": 1,
  "user_id": 1,
  "name": "Tema Oscuro Personalizado",
  "qss_content": "QWidget { background-color: #1e1e1e; color: #ffffff; } QTextEdit { background-color: #2d2d2d; border: 1px solid #555; }",
  "created_at": "2025-07-20T15:45:00"
}
```

### 📥 Obtener todos los temas (sin filtro)
```bash
curl http://localhost:8080/api/themes
```

### 📥 Obtener temas de un usuario específico
```bash
curl "http://localhost:8080/api/themes?user_id=1"
```

**Respuesta esperada:**
```json
[
  {
    "id": 1,
    "user_id": 1,
    "name": "Tema Oscuro Personalizado",
    "qss_content": "QWidget { background-color: #1e1e1e; color: #ffffff; }",
    "created_at": "2025-07-20T15:45:00"
  },
  {
    "id": 3,
    "user_id": 1,
    "name": "Tema Claro Personalizado",
    "qss_content": "QWidget { background-color: #ffffff; color: #000000; }",
    "created_at": "2025-07-20T16:00:00"
  }
]
```

### 🔍 Obtener tema específico
```bash
curl http://localhost:8080/api/themes/1
```

**Respuesta esperada:**
```json
{
  "id": 1,
  "user_id": 1,
  "name": "Tema Oscuro Personalizado",
  "qss_content": "QWidget { background-color: #1e1e1e; color: #ffffff; }",
  "created_at": "2025-07-20T15:45:00"
}
```

### ✏️ Actualizar tema existente
```bash
curl -X PUT http://localhost:8080/api/themes/1 \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Tema Oscuro Mejorado",
    "qss_content": "QWidget { background-color: #121212; color: #e0e0e0; border-radius: 4px; } QTextEdit { background-color: #1e1e1e; color: #ffffff; font-family: \"Consolas\"; }"
  }'
```

**Respuesta esperada:**
```json
{
  "id": 1,
  "user_id": 1,
  "name": "Tema Oscuro Mejorado",
  "qss_content": "QWidget { background-color: #121212; color: #e0e0e0; border-radius: 4px; } QTextEdit { background-color: #1e1e1e; color: #ffffff; font-family: \"Consolas\"; }",
  "created_at": "2025-07-20T15:45:00"
}
```

### 🗑️ Eliminar tema
```bash
curl -X DELETE http://localhost:8080/api/themes/1
```

**Respuesta esperada:**
```json
{
  "message": "Theme deleted successfully"
}
```

---

## 🔧 4. Ejemplos con PowerShell (Windows)

### Crear usuario con PowerShell
```powershell
$body = @{
    username = "usuario_windows"
    email = "windows@kodetron.com"
} | ConvertTo-Json

$response = Invoke-RestMethod -Uri "http://localhost:8080/api/users" `
  -Method POST `
  -Body $body `
  -ContentType "application/json"

Write-Host "Usuario creado:" -ForegroundColor Green
$response | ConvertTo-Json -Depth 2
```

### Obtener temas con PowerShell
```powershell
$themes = Invoke-RestMethod -Uri "http://localhost:8080/api/themes?user_id=1" `
  -Method GET

Write-Host "Temas encontrados: $($themes.Count)" -ForegroundColor Cyan
$themes | ForEach-Object {
    Write-Host "- $($_.name) (ID: $($_.id))" -ForegroundColor Yellow
}
```

### Actualizar tema con PowerShell
```powershell
$themeData = @{
    name = "Tema VS Code Dark"
    qss_content = @"
QWidget {
    background-color: #1e1e1e;
    color: #cccccc;
    font-family: 'Consolas', monospace;
}
QTextEdit {
    background-color: #0d1117;
    color: #f0f6fc;
    border: 1px solid #30363d;
    border-radius: 6px;
    padding: 8px;
}
"@
} | ConvertTo-Json

$updatedTheme = Invoke-RestMethod -Uri "http://localhost:8080/api/themes/1" `
  -Method PUT `
  -Body $themeData `
  -ContentType "application/json"

Write-Host "Tema actualizado exitosamente:" -ForegroundColor Green
Write-Host $updatedTheme.name -ForegroundColor Yellow
```

---

## 🌐 5. Ejemplos con JavaScript (Navegador)

### Verificar conexión con el servidor
```javascript
async function checkServerHealth() {
    try {
        const response = await fetch('http://localhost:8080/health');
        const message = await response.text();
        console.log('✅ Servidor conectado:', message);
        return true;
    } catch (error) {
        console.error('❌ Error de conexión:', error);
        return false;
    }
}

checkServerHealth();
```

### Crear usuario con JavaScript
```javascript
async function createUser(username, email) {
    try {
        const response = await fetch('http://localhost:8080/api/users', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                username: username,
                email: email
            })
        });
        
        if (response.ok) {
            const user = await response.json();
            console.log('✅ Usuario creado:', user);
            return user;
        } else {
            console.error('❌ Error al crear usuario:', response.status);
        }
    } catch (error) {
        console.error('❌ Error de red:', error);
    }
}

// Uso
createUser('usuario_js', 'js@kodetron.com');
```

### Obtener y mostrar temas
```javascript
async function loadUserThemes(userId) {
    try {
        const response = await fetch(`http://localhost:8080/api/themes?user_id=${userId}`);
        const themes = await response.json();
        
        console.log(`📁 Temas del usuario ${userId}:`);
        themes.forEach(theme => {
            console.log(`  🎨 ${theme.name} (ID: ${theme.id})`);
        });
        
        return themes;
    } catch (error) {
        console.error('❌ Error al cargar temas:', error);
    }
}

// Uso
loadUserThemes(1);
```

---

## 📱 6. Ejemplos para Testing

### Script completo de pruebas
```bash
#!/bin/bash
# test_api.sh - Script para probar toda la API

API_URL="http://localhost:8080"

echo "🧪 Iniciando pruebas de la API Kodetron..."
echo "=========================================="

# 1. Verificar servidor
echo "1️⃣ Verificando estado del servidor..."
curl -s $API_URL/health
echo

# 2. Crear usuario de prueba
echo "2️⃣ Creando usuario de prueba..."
USER_RESPONSE=$(curl -s -X POST $API_URL/api/users \
  -H "Content-Type: application/json" \
  -d '{"username":"test_user","email":"test@example.com"}')
echo $USER_RESPONSE
USER_ID=$(echo $USER_RESPONSE | grep -o '"id":[0-9]*' | cut -d':' -f2)
echo "Usuario creado con ID: $USER_ID"
echo

# 3. Crear tema de prueba
echo "3️⃣ Creando tema de prueba..."
THEME_RESPONSE=$(curl -s -X POST $API_URL/api/themes \
  -H "Content-Type: application/json" \
  -d "{\"user_id\":$USER_ID,\"name\":\"Tema de Prueba\",\"qss_content\":\"QWidget{background:#333;}\"}")
echo $THEME_RESPONSE
THEME_ID=$(echo $THEME_RESPONSE | grep -o '"id":[0-9]*' | cut -d':' -f2)
echo "Tema creado con ID: $THEME_ID"
echo

# 4. Obtener temas del usuario
echo "4️⃣ Obteniendo temas del usuario..."
curl -s "$API_URL/api/themes?user_id=$USER_ID"
echo

# 5. Actualizar tema
echo "5️⃣ Actualizando tema..."
curl -s -X PUT $API_URL/api/themes/$THEME_ID \
  -H "Content-Type: application/json" \
  -d '{"name":"Tema Actualizado","qss_content":"QWidget{background:#444;color:#fff;}"}'
echo

# 6. Limpiar datos de prueba
echo "6️⃣ Limpiando datos de prueba..."
curl -s -X DELETE $API_URL/api/themes/$THEME_ID
curl -s -X DELETE $API_URL/api/users/$USER_ID
echo

echo "✅ Pruebas completadas!"
```

### Hacer el script ejecutable y correrlo
```bash
chmod +x test_api.sh
./test_api.sh
```

---

## ❌ 7. Manejo de Errores

### Usuario no encontrado
```bash
curl http://localhost:8080/api/users/999
```
**Respuesta:**
```json
{
  "error": "User not found",
  "code": 404
}
```

### Datos inválidos
```bash
curl -X POST http://localhost:8080/api/users \
  -H "Content-Type: application/json" \
  -d '{"username":"","email":"invalid-email"}'
```

### Tema no encontrado
```bash
curl http://localhost:8080/api/themes/999
```

### Servidor no disponible
```bash
curl http://localhost:8081/health
# curl: (7) Failed to connect to localhost port 8081: Connection refused
```

---

## 🔍 8. Endpoints Pendientes (HTTP 501)

Los siguientes endpoints están estructurados pero aún no implementados:

### Snippets (Code Snippets)
```bash
# Todos retornan HTTP 501 - Not Implemented
curl http://localhost:8080/api/snippets
curl -X POST http://localhost:8080/api/snippets -d '{}'
curl http://localhost:8080/api/snippets/1
curl -X PUT http://localhost:8080/api/snippets/1 -d '{}'
curl -X DELETE http://localhost:8080/api/snippets/1
```

### Templates (Plantillas de código)
```bash
# Todos retornan HTTP 501 - Not Implemented  
curl http://localhost:8080/api/templates
curl -X POST http://localhost:8080/api/templates -d '{}'
```

### Contests & Codeforces
```bash
# También pendientes de implementación
curl http://localhost:8080/api/contests
curl http://localhost:8080/api/codeforces
```

---

¡Estos ejemplos te permitirán probar completamente la funcionalidad actual de la API Kodetron! 🚀

Para más información, consulta la [documentación completa](../BACKEND_GUIDE.md).

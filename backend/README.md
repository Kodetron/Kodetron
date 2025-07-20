# 🌐 Kodetron Backend API

**API RESTful para el editor de código Kodetron**

## 🚀 Inicio Rápido

### 1. Compilar y ejecutar
```bash
# Windows
.\build_backend.bat

# Linux/macOS  
mkdir build_backend && cd build_backend
cmake .. -DCMAKE_BUILD_TYPE=Release
make kodetron_server
./kodetron_server
```

### 2. Verificar funcionamiento
```bash
curl http://localhost:8080/health
# Respuesta: "Kodetron API Server is running"
```

## � Estado de Implementación

| Módulo | Usuarios | Temas | Snippets | Templates | Contests | Codeforces |
|--------|----------|-------|----------|-----------|----------|------------|
| **GET** | ✅ | ✅ | 🔄 | 🔄 | 🔄 | 🔄 |
| **POST** | ✅ | ✅ | 🔄 | 🔄 | 🔄 | 🔄 |
**Legend:** ✅ Complete | 🔄 In development | ❌ Pending

## � Main Endpoints

### 🏥 Server Health
```http
GET /health
```

### 👥 Users
```http
GET    /api/users          # List users
POST   /api/users          # Create user
GET    /api/users/{id}     # Specific user
PUT    /api/users/{id}     # Update user
DELETE /api/users/{id}     # Delete user
```

### 🎨 Themes
```http
GET    /api/themes?user_id={id}  # User themes
POST   /api/themes              # Create theme
GET    /api/themes/{id}         # Specific theme
PUT    /api/themes/{id}         # Update theme
DELETE /api/themes/{id}         # Delete theme
```

### Snippets (TODO)
- `GET /api/snippets?user_id={id}` - Get snippets for user
- `POST /api/snippets` - Create snippet
- `GET /api/snippets/{id}` - Get snippet by ID
- `PUT /api/snippets/{id}` - Update snippet
- `DELETE /api/snippets/{id}` - Delete snippet

### Templates (TODO)
- `GET /api/templates?user_id={id}` - Get templates for user
- `POST /api/templates` - Create template
- `GET /api/templates/{id}` - Get template by ID
- `PUT /api/templates/{id}` - Update template
- `DELETE /api/templates/{id}` - Delete template

### Contests (TODO)
- `GET /api/contests?user_id={id}` - Get contests for user
- `POST /api/contests` - Create contest

### Codeforces Accounts (TODO)
- `GET /api/codeforces?user_id={id}` - Get accounts for user
- `POST /api/codeforces` - Create account

## 📝 Request/Response Examples

### Create User
```bash
curl -X POST http://localhost:8080/api/users \
  -H "Content-Type: application/json" \
  -d '{"username":"john_doe","email":"john@example.com"}'
```

Response:
```json
{
  "id": 1,
  "username": "john_doe",
  "email": "john@example.com",
  "created_at": "2025-07-20T10:30:00"
}
```

### Create Theme
```bash
curl -X POST http://localhost:8080/api/themes \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": 1,
    "name": "Dark Theme",
    "qss_content": "QWidget { background-color: #2b2b2b; color: #ffffff; }"
  }'
```

### Get User Themes
```bash
curl http://localhost:8080/api/themes?user_id=1
```

## 🔐 Authentication

Currently using simplified authentication for demo purposes. 
For production, implement proper JWT tokens.

## 🗄️ Database

Uses SQLite database with the following tables:
- users
- settings  
- templates
- codeforces_accounts
- contests
- snippets
- themes

Database file: `kodetron_server.db` (created automatically)

## 🛠️ Development

### Project Structure
```
backend/
├── main_server.cpp           # Main server entry point
├── controllers/              # REST API controllers
├── middleware/               # Authentication middleware
├── utils/                    # JSON serialization utilities
└── CMakeLists.txt           # Build configuration
```

### Adding New Endpoints
1. Create controller class in `controllers/`
2. Add routes in `main_server.cpp`
3. Update CMakeLists.txt if needed

### CORS
CORS is enabled for all origins during development.
Configure appropriately for production.

## 🚧 TODO
- [ ] Implement remaining repositories (Snippets, Templates, etc.)
- [ ] Add proper JWT authentication
- [ ] Add input validation
- [ ] Add rate limiting
- [ ] Add logging
- [ ] Add tests for API endpoints
- [ ] Add OpenAPI/Swagger documentation

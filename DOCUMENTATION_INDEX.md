# 📚 Documentation Index - Kodetron

This is the complete documentation for the Kodetron project. Use this index to quickly find the information you need.

## 🏁 Quick Start

1. **[INSTALL_DEPENDENCIES.md](./INSTALL_DEPENDENCIES.md)** 📦
   - Installation of Qt6, CMake, Crow framework
   - Specific guides for Windows, Linux and macOS
   - Common troubleshooting

2. **[README.md](./README.md)** 🏠
   - Project overview
   - System architecture (Qt Client + HTTP Backend)
   - Basic compilation commands

## 🌐 Backend HTTP API

3. **[BACKEND_GUIDE.md](./BACKEND_GUIDE.md)** 📖
   - **Complete and detailed backend guide**
   - Installation, configuration and compilation
   - Server execution and verification
   - Qt Desktop integration
   - Advanced troubleshooting

4. **[API_EXAMPLES.md](./API_EXAMPLES.md)** 💡
   - **Practical examples of all queries**
   - curl, PowerShell, JavaScript
   - Automated testing scripts
   - Real use cases

5. **[backend/README.md](./backend/README.md)** 🔧
   - Technical backend summary
   - Implementation status
   - Available endpoints

## 🏗️ Build Scripts

6. **build_backend.bat** ⚙️
   - Main script to compile backend with CMake
   - Error handling and verifications

7. **build_backend_qmake.bat** 🔄
   - Alternative script to compile with qmake
   - Backup when CMake is not available

## 🗄️ Database

8. **Database Schema**
   - 7 tables: users, themes, snippets, templates, contests, codeforces_accounts, settings
   - Relationships and complete structure
   - Documented in [BACKEND_GUIDE.md](./BACKEND_GUIDE.md#database-schema)

## 📊 Implementation Status

### ✅ Fully Functional
- **Qt Desktop Client**: Complete editor with local database
- **Users API**: Complete CRUD (`/api/users`)
- **Themes API**: Complete CRUD (`/api/themes`)
- **Client-Server Integration**: Functional HttpApiClient
- **SQLite Database**: Complete schema implemented

### 🔄 In Development (Structure Created)
- **Snippets API**: Endpoints defined, return HTTP 501
- **Templates API**: Endpoints defined, return HTTP 501  
- **Contests API**: Endpoints defined, return HTTP 501
- **Codeforces API**: Endpoints defined, return HTTP 501

## 🚀 How to Use This Documentation

### For New Developers
1. Read [README.md](./README.md) to understand the project
2. Follow [INSTALL_DEPENDENCIES.md](./INSTALL_DEPENDENCIES.md) to set up the environment
3. Use [BACKEND_GUIDE.md](./BACKEND_GUIDE.md) to compile and run

### For Testing
1. Follow [BACKEND_GUIDE.md](./BACKEND_GUIDE.md) to run the server
2. Use examples from [API_EXAMPLES.md](./API_EXAMPLES.md) to test endpoints
3. Verify functionality with included curl scripts

### For Qt Integration
1. Review "Qt Desktop Integration" section in [BACKEND_GUIDE.md](./BACKEND_GUIDE.md)
2. Study HttpApiClient examples
3. Implement data synchronization following shown patterns

### For Contributing to the Project
1. Understand the architecture in [README.md](./README.md)
2. Review pending endpoints in [backend/README.md](./backend/README.md)
3. Follow the UsersController.cpp pattern for new endpoints

## 🔗 Quick Links

- **Backend API Running**: http://localhost:8080/api
- **Health Check**: http://localhost:8080/health
- **Main Repository**: [Kodetron GitHub](https://github.com/Kodetron/Kodetron)
- **Development Documentation**: [Documentation Repo](https://github.com/Kodetron/Documentation)

## ⚡ Essential Commands

```bash
# Compile Qt Desktop Client
mkdir build && cd build
cmake .. && cmake --build .

# Compile and run backend
.\build_backend.bat
cd build_backend\Release && .\kodetron_server.exe

# Verify server is running
curl http://localhost:8080/health

# Create test user
curl -X POST http://localhost:8080/api/users -H "Content-Type: application/json" -d '{"username":"test","email":"test@example.com"}'
```

## 📞 Support

- **Issues**: Create GitHub issue with logs and details
- **Main Documentation**: Always check [BACKEND_GUIDE.md](./BACKEND_GUIDE.md) first
- **Examples**: All use cases are in [API_EXAMPLES.md](./API_EXAMPLES.md)

---

**Your documentation is complete and ready to use!** 🎉

Start with [INSTALL_DEPENDENCIES.md](./INSTALL_DEPENDENCIES.md) if it's your first time, or go directly to [BACKEND_GUIDE.md](./BACKEND_GUIDE.md) if you already have the dependencies installed.

# Base de Datos Kodetron

Este directorio contiene la implementación de la base de datos SQLite para el proyecto Kodetron.

## Estructura

### Archivos principales

- **DatabaseManager.h/cpp**: Clase singleton para manejar la conexión y operaciones básicas de la base de datos
- **Models.h/cpp**: Estructuras de datos que representan las entidades de la base de datos
- **UserRepository.h/cpp**: Repositorio para operaciones CRUD en la tabla `users`
- **ThemeRepository.h/cpp**: Repositorio para operaciones CRUD en la tabla `themes`
- **DataBase**: Archivo SQL con la definición original de las tablas

### Tablas de la base de datos

1. **users**: Almacena información de usuarios
2. **settings**: Configuraciones por usuario
3. **templates**: Plantillas de código por usuario
4. **codeforces_accounts**: Cuentas de Codeforces vinculadas
5. **contests**: Información de concursos
6. **snippets**: Fragmentos de código reutilizables
7. **themes**: Temas personalizados (archivos QSS)

## Uso básico

### Inicialización

```cpp
#include "DataBase/DatabaseManager.h"

// En main.cpp o al inicio de la aplicación
if (!DatabaseManager::instance()->initialize()) {
    qCritical() << "Error al inicializar la base de datos";
    return -1;
}
```

### Trabajar con usuarios

```cpp
#include "DataBase/UserRepository.h"
#include "DataBase/Models.h"

UserRepository userRepo;

// Crear usuario
User newUser("john_doe", "john@example.com");
if (userRepo.create(newUser)) {
    qDebug() << "Usuario creado con ID:" << newUser.id;
}

// Buscar usuario
auto user = userRepo.findByUsername("john_doe");
if (user.has_value()) {
    qDebug() << "Usuario encontrado:" << user->username;
}

// Listar todos los usuarios
QList<User> users = userRepo.findAll();
for (const auto& user : users) {
    qDebug() << "Usuario:" << user.username;
}
```

### Trabajar con temas

```cpp
#include "DataBase/ThemeRepository.h"

ThemeRepository themeRepo;

// Crear tema
Theme theme(userId, "Dark Theme", "QWidget { background-color: #2b2b2b; }");
if (themeRepo.create(theme)) {
    qDebug() << "Tema creado con ID:" << theme.id;
}

// Buscar temas de un usuario
QList<Theme> userThemes = themeRepo.findByUserId(userId);
```

## Transacciones

Para operaciones que involucran múltiples consultas:

```cpp
DatabaseManager* db = DatabaseManager::instance();

if (db->beginTransaction()) {
    // Realizar operaciones...
    if (todoExitoso) {
        db->commitTransaction();
    } else {
        db->rollbackTransaction();
    }
}
```

## Extensión

Para agregar nuevas tablas o repositorios:

1. Agregar la definición de tabla en `DatabaseManager::createTables()`
2. Crear la estructura en `Models.h/cpp`
3. Crear el repositorio correspondiente siguiendo el patrón de `UserRepository`

## Dependencias

- Qt6 Core
- Qt6 Sql (SQLite driver incluido por defecto)

## Ubicación de la base de datos

Por defecto, la base de datos se crea en:
- Windows: `%APPDATA%/Kodetron/kodetron.db`
- Linux: `~/.local/share/Kodetron/kodetron.db`
- macOS: `~/Library/Application Support/Kodetron/kodetron.db`

Puedes especificar una ruta personalizada en `DatabaseManager::initialize(customPath)`.

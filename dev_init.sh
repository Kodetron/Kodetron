#!/usr/bin/env bash

set -e

echo "=== Kodetron Dev Initialization ==="

# 1. Check minimum dependencies
echo "[*] Checking dependencies..."

for dep in cmake g++ sqlite3; do
    if ! command -v $dep &>/dev/null; then
        echo "❌ Missing dependency: $dep"
        exit 1
    fi
done

if ! pkg-config --exists Qt6Core; then
    echo "❌ Missing Qt6 (make sure Qt6 and pkg-config are installed and configured)"
    exit 1
fi

echo "[✓] All required dependencies are present"

# 2. Install project dependencies if present
if [ -f requirements.txt ]; then
    echo "[*] Installing Python dependencies..."
    pip install -r requirements.txt
fi

if [ -f package.json ]; then
    echo "[*] Installing Node.js dependencies..."
    npm install
fi

if [ -f pom.xml ]; then
    echo "[*] Installing Java (Maven) dependencies..."
    mvn install
fi

# 3. Initialize/migrate the development database
echo "[*] Initializing development database..."
mkdir -p build
cp -f database/schema.sql build/schema.sql
sqlite3 build/dev_kodetron.db < database/schema.sql

# 4. Build and run the project
echo "[*] Building Kodetron..."
cd build
cmake ..
make

echo "[*] Running Kodetron in development mode..."
./Kodetron 
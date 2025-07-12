Write-Host "=== Kodetron Dev Initialization ==="

# 1. Check minimum dependencies
$deps = @("cmake", "g++", "sqlite3")
foreach ($dep in $deps) {
    if (-not (Get-Command $dep -ErrorAction SilentlyContinue)) {
        Write-Host "❌ Missing dependency: $dep"
        exit 1
    }
}

# Simple Qt6 check (customize path as needed)
if (-not (Test-Path "C:/Qt/6.*")) {
    Write-Host "❌ Missing Qt6 (install Qt6 and add to PATH)"
    exit 1
}

Write-Host "[✓] All required dependencies are present"

# 2. Install project dependencies if present
if (Test-Path "requirements.txt") {
    Write-Host "[*] Installing Python dependencies..."
    pip install -r requirements.txt
}
if (Test-Path "package.json") {
    Write-Host "[*] Installing Node.js dependencies..."
    npm install
}
if (Test-Path "pom.xml") {
    Write-Host "[*] Installing Java (Maven) dependencies..."
    mvn install
}

# 3. Initialize/migrate the development database
Write-Host "[*] Initializing development database..."
if (-not (Test-Path "build")) { mkdir build }
Copy-Item database/schema.sql build/schema.sql -Force
sqlite3 build/dev_kodetron.db < database/schema.sql

# 4. Build and run the project
Write-Host "[*] Building Kodetron..."
cd build
cmake ..
make

Write-Host "[*] Running Kodetron in development mode..."
./Kodetron.exe 
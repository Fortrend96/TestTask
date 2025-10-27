#!/bin/bash
set -e

# --------------------------------------
# Перейти в папку проекта
# --------------------------------------
PROJECT_DIR="$(dirname "$0")"
cd "$PROJECT_DIR"

# --------------------------------------
# Найти доступный терминал
# --------------------------------------
if command -v gnome-terminal &> /dev/null; then
    TERM_CMD="gnome-terminal -- bash -c"
elif command -v x-terminal-emulator &> /dev/null; then
    TERM_CMD="x-terminal-emulator -e"
elif command -v konsole &> /dev/null; then
    TERM_CMD="konsole -e"
elif command -v xfce4-terminal &> /dev/null; then
    TERM_CMD="xfce4-terminal -e"
else
    echo "Ошибка: Не найден поддерживаемый терминал. Установите gnome-terminal, konsole или xfce4-terminal."
    exit 1
fi

# --------------------------------------
# Удалить старую сборку, если есть
# --------------------------------------
if [ -d "build" ]; then
    echo "Папка build найдена. Удаляем..."
    rm -rf build
fi

mkdir build
cd build

# --------------------------------------
# Сборка проекта с тестами
# --------------------------------------
echo "Конфигурирование проекта..."
cmake .. -DBUILD_TESTS=ON

echo "Сборка проекта..."
cmake --build . -j$(nproc)

# --------------------------------------
# Проверка бинарников
# --------------------------------------
if [ ! -f "./server/server" ]; then
    echo "Ошибка: бинарник сервера не найден!"
    exit 1
fi

if [ ! -f "./client/client" ]; then
    echo "Ошибка: бинарник клиента не найден!"
    exit 1
fi

TEST_BINARY="./server/tests/tests"
if [ ! -f "$TEST_BINARY" ]; then
    echo "Ошибка: тестовый бинарник не найден!"
    exit 1
fi

# --------------------------------------
# Запуск сервера и клиента в отдельных терминалах
# --------------------------------------
echo "Запуск сервера..."
$TERM_CMD "./server/server; exec bash" &

echo "Запуск клиента..."
$TERM_CMD "./client/client; exec bash" &

sleep 2

# --------------------------------------
# Запуск тестов напрямую для детального вывода
# --------------------------------------
echo "Запуск тестов..."
"$TEST_BINARY"

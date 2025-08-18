# Переменные для удобства
CC = g++
CFLAGS = -Wall -Wextra -std=c++11 
CFLAGS1 = -lsfml-graphics -lsfml-window -lsfml-system
SRC_DIR = /home/dima/IMG1
BUILD_DIR = /home/dima/IMG1/BUILD_DIR
TARGET = a

# Список исходных файлов
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Список объектных файлов
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Цель по умолчанию (если ничего не указано)
all: $(TARGET)

# Создание каталога build, если он не существует
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Правило для компиляции .cpp файлов в .o файлы
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CFLAGS1)

# Правило для создания исполняемого файла
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(CFLAGS1)

# Правило для очистки
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)
	rm -d $(BUILD_DIR)

# Инструкция для пересоздания всего
rebuild: clean all

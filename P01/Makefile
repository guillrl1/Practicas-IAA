# Variables de compilación
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Archivos
SOURCES = $(wildcard $(SRC_DIR)/*.cc)
OBJECTS = $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET = $(BIN_DIR)/inferencia

# Regla por defecto
all: $(TARGET)

# Crear el ejecutable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilar archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Limpiar archivos temporales
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
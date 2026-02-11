# --- Configuración ---
CXX      = g++
CXXFLAGS = -std=c++17 -O3 -Iinclude

# --- Directorios ---
SRC_DIR  = src
INC_DIR  = include
TEST_DIR = test
BIN_DIR  = bin

# --- Archivos Fuente ---
# La lógica de la clase que comparten ambos
LIB_SRC   = $(SRC_DIR)/DistribucionConjunta.cc

# Los puntos de entrada de cada programa
MAIN_SRC  = $(SRC_DIR)/main.cc
BENCH_SRC = $(TEST_DIR)/benchmark.cc

# --- Ejecutables ---
TARGET_MAIN  = $(BIN_DIR)/programa_principal
TARGET_BENCH = $(BIN_DIR)/benchmark_exe

# --- Reglas ---

# Por defecto compila ambos
all: $(BIN_DIR) $(TARGET_MAIN) $(TARGET_BENCH)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Regla para el programa principal
main: $(BIN_DIR) $(TARGET_MAIN)

$(TARGET_MAIN): $(LIB_SRC) $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Regla para el benchmark
benchmark: $(BIN_DIR) $(TARGET_BENCH)

$(TARGET_BENCH): $(LIB_SRC) $(BENCH_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Ejecutar el main
run: main
	./$(TARGET_MAIN)

# Ejecutar el benchmark
run_bench: benchmark
	./$(TARGET_BENCH)

clean:
	rm -rf $(BIN_DIR) benchmark_resultados.csv

.PHONY: all clean run run_bench main benchmark
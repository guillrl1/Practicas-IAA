#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

bool suma_probabilidades(const std::map<std::string, double>& data) {
  const double precision = 1e-9;
  double suma = 0.0;
  for (const auto& pair : data) {
    suma += pair.second;
  }
  return std::abs(suma - 1.0) < precision;
}

std::map<std::string, double> normalizarProbabilidades(const std::map<std::string, double>& data) {
  double suma = 0.0;
  for (const auto& pair : data) 
    suma += pair.second;
  std::map<std::string, double> normalized_data;
  for (const auto& pair : data)
    normalized_data[pair.first] = pair.second / suma;
  return normalized_data;
}

std::map<std::string, double> generarAleatorios(int N) {
  std::map<std::string, double> data;
  double suma = 0.0;
  for (int i = 0; i < 10; ++i) {
    std::string binary_mask;
    for (int j = 0; j < N; ++j) {
      binary_mask += (rand() % 2) ? '1' : '0';
    }
    double probability = static_cast<double>(rand()) / RAND_MAX;
    data[binary_mask] = probability;
    suma += probability;
  }
  for (auto& pair : data) {
    pair.second /= suma;
  }
  return data;
}

int main (int argc, char *argv[]) {
  const int N = argv[1] ? std::stoi(argv[1]) : 3;
  std::map<std::string, double> data;
  if (argc < 3) {
    data = generarAleatorios(N);
  } else {
    std::string filename = argv[1];
    std::ifstream file (filename, std::ios::in | std::ios::binary);
    if (!file) {
      std::cerr << "Error opening file: " << filename << std::endl;
      return 1;
    }
    // La entrada será un CSV: mascara_binaria,probabilidad
    std::string line;
    std::regex pattern(R"(^([01]+),([0-9]*\.?[0-9]+)$)");

    while (std::getline(file, line)) {
      std::smatch matches;
      if (std::regex_match(line, matches, pattern)) {
        std::string binary_mask = matches[1];
        double probability = std::stod(matches[2]);
        std::cout << "Mascara binaria: " << binary_mask << ", Probabilidad: " << probability << std::endl;
        data[binary_mask] = probability;
      }
      else {
        std::cerr << "Linea no valida: " << line << std::endl;
      }
    }
    file.close();
  }

  data = normalizarProbabilidades(data);
  if (!suma_probabilidades(data)) {
    std::cerr << "La suma de las probabilidades no es 1.0" << std::endl;

  }
  double suma = 0.0;
  for (const auto& pair : data) {
    std::cout << "Mascara binaria: " << pair.first << ", Probabilidad normalizada: " << pair.second << std::endl;
    suma += pair.second;
  }
  std::cout << "Suma total de probabilidades: " << suma << std::endl;

  // Vector de variables condicionadas
  std::vector<int> condicionadas;
  std::cout << "Introduce las posiciones de las variables condicionadas (0-indexed), separadas por espacios." << std::endl;
  std::string input;
  std::getline(std::cin, input);
  std::istringstream iss(input);
  int pos;
  for(int i = 0; i < N; ++i)
    condicionadas.push_back(0);
  while (iss >> pos) {
    condicionadas[pos - 1] = 1;
  }

  // Variables de interés
  std::vector<int> interes;
  std::cout << "Introduce las posiciones de las variables de interes (0-indexed), separadas por espacios." << std::endl;
  std::getline(std::cin, input);
  std::istringstream iss2(input);
  for(int i = 0; i < N; ++i)
    interes.push_back(0);
  while (iss2 >> pos) {
    interes[pos - 1] = 1;
  }
}


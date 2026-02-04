#include "DistribucionConjunta.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>

DistribucionConjunta::DistribucionConjunta(int num_variables) : N(num_variables) {
    probabilidades.assign(pow(2, N), 0.0);
    for (double& p : probabilidades) {
        p = static_cast<double>(rand()) / RAND_MAX;
    }
    normalizar();
}

int DistribucionConjunta::mascaraAIndice(const std::string& mascara) const {
    // Según guion: Bit 0 (derecha) -> X1, Bit N-1 (izq) -> XN
    // Esto coincide con la interpretación estándar de binario
    return std::stoi(mascara, nullptr, 2);
}

bool DistribucionConjunta::cargarDesdeCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    probabilidades.assign(pow(2, N), 0.0);
    std::string line;
    std::regex pattern(R"(^([01]+),([0-9]*\.?[0-9]+)$)");

    while (std::getline(file, line)) {
        std::smatch matches;
        if (std::regex_match(line, matches, pattern)) {
            int idx = mascaraAIndice(matches[1]);
            if (idx < probabilidades.size()) {
                probabilidades[idx] = std::stod(matches[2]);
            }
        }
    }
    normalizar();
    return true;
}

void DistribucionConjunta::normalizar() {
    double suma = 0.0;
    for (double p : probabilidades) suma += p;
    for (double& p : probabilidades) p /= suma;
}

void DistribucionConjunta::mostrarDistribucion() const {
    std::cout << "\n--- Distribucion Conjunta P(X1...XN) ---\n";
    for (int i = 0; i < probabilidades.size(); ++i) {
        // Mostrar en formato binario para legibilidad
        std::cout << "Indice " << i << ": Prob = " << probabilidades[i] << std::endl;
    }
}
#include <iostream>
#include <iomanip>
#include "DistribucionConjunta.h"

int main(int argc, char* argv[]) {
    // 1. Manejo de argumentos y Carga (Apartado 1)
    int n_vars = 3; 
    if (argc > 1) n_vars = std::stoi(argv[1]);

    DistribucionConjunta dist(n_vars);

    if (argc > 2) {
        if (!dist.cargarDesdeCSV(argv[2])) {
            std::cerr << "Error al cargar CSV. Usando valores aleatorios.\n";
        }
    }

    // 2. Mostrar la distribución almacenada (Apartado 3)
    // Es vital mostrar el array p[k] para validar la carga 
    std::cout << "\n--- Distribucion Conjunta Almacenada (Array p[k]) ---" << std::endl;
    std::cout << std::left << std::setw(10) << "Indice k" << "Probabilidad p[k]" << std::endl;
    for (int k = 0; k < dist.get_size(); ++k) {
        std::cout << std::left << std::setw(10) << k << dist.get_prob(k) << std::endl;
    }

    // 3. Selección de variables (Apartado 2)
    int maskC = 0, valC = 0, maskI = 0;
    int pos, valor;

    std::cout << "\n--- Seleccion de Variables (Indices 1 a " << n_vars << ") ---" << std::endl;
    
    std::cout << "Variables Condicionadas (XC). Introduce 'posicion valor' (ej: 2 1), o -1 para terminar: ";
    while (std::cin >> pos && pos != -1) {
        if (pos < 1 || pos > n_vars) continue;
        std::cin >> valor;
        maskC |= (1 << (pos - 1)); // Bit 0 -> X1 
        if (valor == 1) valC |= (1 << (pos - 1));
    }

    std::cout << "Variables de Interes (XI). Introduce posiciones (ej: 1 3), o -1 para terminar: ";
    while (std::cin >> pos && pos != -1) {
        if (pos < 1 || pos > n_vars) continue;
        maskI |= (1 << (pos - 1));
    }

    // El cálculo de maskM (marginalizar) es implícito: variables que no están en maskC ni maskI
    int maskTotal = (1 << n_vars) - 1;
    int maskM = maskTotal & ~(maskC | maskI);

    std::cout << "\n--- Configuracion Final de Mascaras ---" << std::endl;
    std::cout << "maskC: " << maskC << " (Valores: " << valC << ")" << std::endl;
    std::cout << "maskI: " << maskI << std::endl;
    std::cout << "maskM: " << maskM << " (Variables a marginalizar)" << std::endl;

    return 0;
}
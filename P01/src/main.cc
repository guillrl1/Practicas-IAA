#include <iostream>
#include <iomanip>
#include <bitset>
#include <cmath>
#include <chrono>
#include "DistribucionConjunta.h"

int main(int argc, char* argv[]) {
    // 1. Manejo de argumentos y Carga
    int n_vars = 3; 
    if (argc > 1) n_vars = std::stoi(argv[1]);

    DistribucionConjunta dist(n_vars);

    if (argc > 2) {
        if (!dist.cargarDesdeCSV(argv[2])) {
            std::cerr << "Error al cargar CSV. Usando valores aleatorios.\n";
        }
    }

    // 2. Mostrar la distribución original para validar
    std::cout << "\n--- Distribucion Conjunta Almacenada (Array p[k]) ---" << std::endl;
    std::cout << std::left << std::setw(10) << "Indice k" << "Probabilidad p[k]" << std::endl;
    for (int k = 0; k < dist.get_size(); ++k) {
        std::cout << std::left << std::setw(10) << k << dist.get_prob(k) << std::endl;
    }

    // 3. Selección de variables
    uint32_t maskC = 0;
    uint32_t valC = 0;
    uint32_t maskI = 0;

    int pos, valor;

    std::cout << "\n--- Seleccion de Variables (Indices 1 a " << n_vars << ") ---" << std::endl;
    
    // XC - Si no se introduce nada, se calcula la Marginal
    std::cout << "Variables Condicionadas (XC). Introduce 'posicion valor' (ej: 2 1), o -1 para terminar. Puede poner -1 para saltar (Marginal): ";
    bool flag_marginal = true;
    while (std::cin >> pos && pos != -1) {
        if (pos < 1 || pos > n_vars) {
            std::cout << "Posicion invalida.\n";
            continue;
        }
        std::cin >> valor;
        if (valor != 0 && valor != 1) continue;
        
        maskC |= (1 << (pos - 1)); 
        if (valor == 1) valC |= (1 << (pos - 1));
        flag_marginal = false;
    }
    std::cout << "Variables de Interes (XI). Introduce posiciones (ej: 1 3), o -1 para terminar: ";
    while (std::cin >> pos && pos != -1) {
        if (pos < 1 || pos > n_vars) continue;

        if ((maskC >> (pos - 1)) & 1) {
            std::cout << "Variable " << pos << " ya esta en XC, no se añadira a XI.\n";
            continue;
        }
        maskI |= (1 << (pos - 1));
    }

    // 4. Cálculo de parámetros para la salida
    int num_interes = 0;
    for (int i = 0; i < n_vars; ++i) {
        if ((maskI >> i) & 1) num_interes++;
    }

    // 5. Llamada a la función de cálculo
    // Nota: Pasamos dist.get_prob_ptr() o similar si existe, 
    // si no, creamos el array temporal como tenías:
    int tam_total = dist.get_size();
    double* p_data = new double[tam_total];
    for(int k = 0; k < tam_total; ++k) p_data[k] = dist.get_prob(k);

    // iniciar chrono
    auto start = std::chrono::high_resolution_clock::now();
    double* resultado = dist.prob_cond_bin(p_data, n_vars, maskC, valC, maskI);
    // detener chrono
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // 6. Salida del resultado corregida
    flag_marginal ? std::cout << "\n--- Resultado de P(XI) (Marginal) ---" << std::endl
              :
    std::cout << "\n--- Resultado de P(XI | XC) ---" << std::endl;
    
    int tam_salida = 1 << num_interes;
    for (int i = 0; i < tam_salida; ++i) {
        // Mostramos el índice 'i' en binario, que representa la combinación de XI
        // Usamos substr para mostrar solo los bits relevantes
        std::string bits_XI = std::bitset<32>(i).to_string().substr(32 - num_interes);
        
        // Para XC, mostramos la configuración de valores que el usuario eligió
        // Filtramos valC con maskC para limpiar bits residuales
        if (!flag_marginal) {
            std::string bits_XC = std::bitset<32>(valC & maskC).to_string().substr(32 - n_vars);
            std::cout << "P(XI=" << bits_XI << " | XC=" << bits_XC << ") = " 
                      << std::fixed << std::setprecision(6) << resultado[i] << std::endl;
        } else {
            std::cout << "P(XI=" << bits_XI << ") = " 
                      << std::fixed << std::setprecision(6) << resultado[i] << std::endl;
        }
    }

    std::cout << "\nTiempo de calculo: " << elapsed.count() << " segundos.\n";

    // 7. Limpieza
    delete[] p_data;
    delete[] resultado;

    return 0;
}
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "DistribucionConjunta.h"

using namespace std;
using namespace std::chrono;

void ejecutar_test(ofstream& csv, DistribucionConjunta& dist, int n, uint32_t mC, int vC, uint32_t mI, string categoria) {
    const int REPETICIONES = 50;
    
    // Contar bits
    int nI = 0, nC = 0;
    for(int i=0; i<n; i++) {
        if((mI >> i) & 1) nI++;
        if((mC >> i) & 1) nC++;
    }

    // Preparar puntero de datos (ajusta si tu clase tiene acceso directo)
    int tam_total = 1 << n;
    double* p_data = new double[tam_total];
    for(int k = 0; k < tam_total; ++k) p_data[k] = dist.get_prob(k);

    // Medición
    auto inicio = high_resolution_clock::now();
    for (int r = 0; r < REPETICIONES; ++r) {
        double* res = dist.prob_cond_bin(p_data, n, mC, vC, mI);
        delete[] res;
    }
    auto fin = high_resolution_clock::now();
    
    double tiempo_total = duration_cast<microseconds>(fin - inicio).count();
    double tiempo_medio = tiempo_total / REPETICIONES;

    // CSV: Categoria, N, Num_I, Num_C, Tiempo
    csv << categoria << "," << n << "," << nI << "," << nC << "," << tiempo_medio << endl;
    
    delete[] p_data;
}

int main() {
    ofstream csv("benchmark_resultados.csv");
    csv << "Categoria,N_Vars,Num_Interes,Num_Cond,Tiempo_Microsegundos" << endl;

    cout << "Iniciando Benchmark Completo..." << endl;

    // --- TEST 1: ESCALABILIDAD (N Variable) ---
    // Objetivo: Ver el crecimiento exponencial
    cout << "Ejecutando Test de Escalabilidad..." << endl;
    for (int n = 10; n <= 20; ++n) {
        DistribucionConjunta d(n);
        // P(X1 | X2=1), variables fijas, N crece
        ejecutar_test(csv, d, n, 0b10, 1, 0b01, "Escalabilidad_N");
    }

    // --- TEST 2: IMPACTO DE VARIABLES DE INTERÉS (Marginalización) ---
    // Objetivo: Ver si el tamaño del array de salida afecta el tiempo
    // Fijamos N=16 para que sea un tiempo medible
    cout << "Ejecutando Test de Interes..." << endl;
    int N_FIX = 16;
    DistribucionConjunta d_fix(N_FIX);
    for (int i = 1; i <= 8; ++i) {
        uint32_t maskI = (1 << i) - 1; // X1, X1-X2, X1-X3...
        ejecutar_test(csv, d_fix, N_FIX, 0b10000000000, 0, maskI, "Carga_Interes");
    }

    // --- TEST 3: COMPLEJIDAD DE LA CONDICIÓN ---
    // Objetivo: Ver si filtrar por muchas condiciones ahorra o gasta tiempo
    cout << "Ejecutando Test de Condicion..." << endl;
    for (int c = 1; c <= 8; ++c) {
        uint32_t maskC = ((1 << c) - 1) << 8; // Condiciones en bits altos
        ejecutar_test(csv, d_fix, N_FIX, maskC, 0, 0b1, "Carga_Condicion");
    }

    csv.close();
    cout << "Finalizado. Resultados en benchmark_resultados.csv" << endl;
    return 0;
}
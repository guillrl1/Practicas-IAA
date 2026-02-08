
#ifndef DISTRIBUCION_CONJUNTA_H
#define DISTRIBUCION_CONJUNTA_H

#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>
#include <cstdint>

class DistribucionConjunta {
public:
    // Constructor para generación aleatoria
    DistribucionConjunta(int num_variables);
    
    // Carga desde CSV 
    bool cargarDesdeCSV(const std::string& filename);

    // Getters
    int get_N() const { return N; }
    int get_size() const { return probabilidades.size(); }
    double get_prob(int k) const { return probabilidades[k]; }
    void mostrarDistribucion() const;
    bool validarSuma() const;

    double* prob_cond_bin(double *p, int N, uint32_t maskC, int valC, uint32_t maskI);

private:
    int N; 
    std::vector<double> probabilidades; // Array p[k] solicitado 
    
    int mascaraAIndice(const std::string& mascara) const;
    void normalizar();
};

#endif // DISTRIBUCION_CONJUNTA_H
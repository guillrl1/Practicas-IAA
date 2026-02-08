#include "DistribucionConjunta.h"

/**
    * Constructor que inicializa la distribución conjunta con probabilidades aleatorias
    * @param num_variables: número de variables en la distribución conjunta (N)
 */
DistribucionConjunta::DistribucionConjunta(int num_variables) : N(num_variables) {
    probabilidades.assign(pow(2, N), 0.0);
    for (double& p : probabilidades) {
        p = static_cast<double>(rand()) / RAND_MAX;
    }
    normalizar();
}

/**
    * Convierte una máscara binaria (string) a un índice entero
    * @param mascara: string que representa la máscara binaria (ej. "101")
    * @return el índice entero correspondiente a la máscara
 */
int DistribucionConjunta::mascaraAIndice(const std::string& mascara) const {
    // Según guion: Bit 0 (derecha) -> X1, Bit N-1 (izq) -> XN
    // Esto coincide con la interpretación estándar de binario
    return std::stoi(mascara, nullptr, 2);
}

/**
    * Carga la distribución conjunta desde un archivo CSV
    * El formato esperado es: mascara_binaria,probabilidad
    * Ejemplo de línea: "101,0.25" -> P(X1=1, X2=0, X3=1) = 0.25
    * @param filename: nombre del archivo CSV a cargar
    * @return true si se cargó correctamente, false en caso contrario
 */
bool DistribucionConjunta::cargarDesdeCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    probabilidades.assign(pow(2, N), 0.0);
    std::string line;
    std::regex pattern(R"(^([01]+),([0-9]*\.?[0-9]+)$)");

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
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

/**
    * Normaliza las probabilidades para que sumen 1
 */
void DistribucionConjunta::normalizar() {
    double suma = 0.0;
    for (double p : probabilidades) suma += p;
    for (double& p : probabilidades) p /= suma;
}

/**
    * Muestra la distribución conjunta en la consola
 */
void DistribucionConjunta::mostrarDistribucion() const {
    std::cout << "\n--- Distribucion Conjunta P(X1...XN) ---\n";
    for (int i = 0; i < probabilidades.size(); ++i) {
        // Mostrar en formato binario para legibilidad
        std::cout << "Indice " << i << ": Prob = " << probabilidades[i] << std::endl;
    }
}

/**
    * Calcula la distribución condicional P(X_C | X_M = valM)
    * @param p: arreglo de probabilidades de la distribución conjunta
    * @param N: número de variables
    * @param maskC: máscara para las variables condicionadas (X_C)
    * @param valC: valor específico para las variables condicionadas (X_C)
    * @param maskM: máscara para las variables dadas (X_M)
    * @return un nuevo arreglo con las probabilidades condicionales normalizadas
*/
double* DistribucionConjunta::prob_cond_bin(double *p, int N, uint32_t maskC, int valC, uint32_t maskI) {
    // Determinamos el número de variables de interés contando los bits en maskI
    int num_vars_interes = 0;
    for (int i = 0; i < N; ++i) {
        if (maskI & (1 << i)) {
            num_vars_interes++;
        }
    }

    // El array tendrá 2^(num_vars_interes) elementos
    int tam_salida = 1 <<  num_vars_interes; // 2^num_vars_interes
    double* output = new double[tam_salida]();
    double suma = 0.0;

    // Recorremos todo el array de la distribución conjunta p[k]
    int tam_conjunta = 1 << N; // 2^N
    for (int k = 0; k < tam_conjunta; ++k) {
        // 1. Comprobar si el índice k cumple con las condiciones de maskC y valC
        if ((k & maskC) == valC) {
            suma += p[k]; // Acumulamos la probabilidad para normalizar después

            // 2. Marginalización: Mapear k a un nuevo índice
            //  para el array de salida basado en maskI
            uint32_t indice_interes = 0;
            int bit_pos_salida = 0; // Posición del bit en el índice de salida
            for (int i = 0; i < N; ++i) {
                if ((maskI >> i) & 1) { // Si la variable i es de interés
                    if (k & (1 << i)) { // Si el bit i en k es 1
                        indice_interes |= (1 << bit_pos_salida); // Setear el bit correspondiente en el índice de salida
                    }
                    bit_pos_salida++;
                }
            }
            output[indice_interes] += p[k]; // Acumular la probabilidad en el índice correspondiente
        }
    }
    
    // 3.Normalización: P(XI | XC) = P(XI, XC) / P(XC)
    if (suma > 0) {
        for (int i = 0; i < tam_salida; ++i) {
            output[i] /= suma;
        }
    }
    return output;
}

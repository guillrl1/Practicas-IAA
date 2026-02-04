# Inteligencia Artificial Avanzada

Este repositorio contiene la implementación de la práctica sobre **Inferencia Condicional en distribuciones discretas binarias**. El objetivo principal es el procesamiento de tablas de probabilidad para realizar selección de variables, marginalización y cálculo de probabilidades condicionales.

---

## 🎓 Información Académica
* **Institución:** Universidad de La Laguna (ULL)
* **Grado:** Ingeniería Informática
* **Asignatura:** Inteligencia Artificial Avanzada (IAA)

## 👥 Autores
| Nombre | Correo Electrónico |
| :--- | :--- |
| **Guillermo López Concepción** | [alu0101620459@ull.edu.es](mailto:alu0101620459@ull.edu.es) |
| **Marco Aguiar Álvarez** | [alu0101620961@ull.edu.es](mailto:alu0101620961@ull.edu.es) |

---

## 📂 Proyecto: Inferencia Condicional
El programa permite trabajar con distribuciones de probabilidad multivariables representadas mediante máscaras binarias.

### Funcionalidades implementadas:
1.  **Carga de datos:** Lectura de archivos CSV con formato `máscara,probabilidad`.
2.  **Normalización:** Asegura que la suma de todas las probabilidades sea $1.0$.
3.  **Selección de Variables:**
    * **Variables Condicionadas ($X_C$):** Fijación de valores observados (evidencia).
    * **Variables de Interés ($X_I$):** Definición de las variables sobre las que se desea calcular la distribución.
    * **Marginalización:** Eliminación automática de las variables no seleccionadas.

### Ejecución
Para compilar y ejecutar el programa:

```bash
g++ -o inferencia main.cc

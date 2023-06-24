#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <omp.h>

//INTEGRANTES: NICOLAS RUIZ, PAULA RUIZ, MATIAS ZAPATA (GRUPO H)

// Función para el cálculo de primos usando el algoritmo especificado e invetigado
std::vector<bool> calcularPrimos(int limite) {
    // Vector para guardar si un número es primo o no es primo, la funcion recibe el numero hasta cual buscar
    std::vector<bool> esPrimo(limite + 1, false);
    esPrimo[2] = esPrimo[3] = true; // 2 y 3 son números primos
    int raizLimite = std::sqrt(limite); // se calcula la raiz cuadrada entera del maximo

    // Comenzamos con los calculos en paralelo con OpenMP
    #pragma omp parallel for
    for (int x = 1; x <= raizLimite; ++x) {
        for (int y = 1; y <= raizLimite; ++y) {
            int n = (4 * x * x) + (y * y);
            if (n <= limite && (n % 12 == 1 || n % 12 == 5)) {
                #pragma omp critical
                esPrimo[n] = !esPrimo[n];
            }

            n = (3 * x * x) + (y * y);
            if (n <= limite && n % 12 == 7) {
                #pragma omp critical
                esPrimo[n] = !esPrimo[n];
            }

            n = (3 * x * x) - (y * y);
            if (x > y && n <= limite && n % 12 == 11) {
                #pragma omp critical
                esPrimo[n] = !esPrimo[n];
            }
        }
    }

    // Eliminar múltiplos de cuadrados de primos una forma de ir quitando numeros de forma mas rapida
    for (int n = 5; n <= raizLimite; ++n) {
        if (esPrimo[n]) {
            int s = n * n;
            for (int k = s; k <= limite; k += s) {
                esPrimo[k] = false;
            }
        }
    }

    return esPrimo;
}

int main() {
    int limite = 500000000;  // VAriable que se deberia editar si queremos aumentar o disminuir el numero maximo a buscar primos
    double tiempoInicioCalculo, tiempoFinalCalculo, tiempoInicioTotal, tiempoFinalTotal;
    std::vector<bool> esPrimo;
//aqui se activan los tiempos a mostrar al final del programa
    tiempoInicioTotal = omp_get_wtime();
    tiempoInicioCalculo = omp_get_wtime();
    esPrimo = calcularPrimos(limite);// se llama la funcion de calcular los primos
    tiempoFinalCalculo = omp_get_wtime();

    // Contador de primos
    int numPrimos = 0;
    for (int i = 2; i <= limite; ++i) {
        if (esPrimo[i]) {
            std::cout << i << '\n';
            ++numPrimos;
        }
    }
    tiempoFinalTotal = omp_get_wtime();

    // Imprimir el total de números primos y los tiempos
    std::cout << "Total de números primos entre 1 y " << limite << ": " << numPrimos << '\n';
    std::cout << "Tiempo de cálculo primos: " << tiempoFinalCalculo - tiempoInicioCalculo << " segundos.\n";
    std::cout << "Tiempo total (incluyendo la impresión): " << tiempoFinalTotal - tiempoInicioTotal << " segundos.\n";
    return 0;
}


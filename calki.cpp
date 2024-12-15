/**
 * @file main.cpp
 * @brief Program do obliczania przybliżonej wartości liczby PI za pomocą metody całkowania numerycznego.
 *
 * Program wykorzystuje wielowątkowość do równoległego przetwarzania danych w celu zwiększenia wydajności.
 * Obliczenia opierają się na metodzie prostokątów dla funkcji f(x) = 4 / (1 + x^2), której całka na przedziale
 * [0, 1] jest równa liczbie PI.
 *
 * @author Twoje Imię
 * @date 2024
 */

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cmath>
#include <cstdlib>

 /**
  * @struct ThreadData
  * @brief Struktura przechowująca dane potrzebne do pracy każdego wątku.
  *
  * Struktura ta zawiera parametry pracy wątku, takie jak identyfikator wątku, liczba wątków,
  * liczba przedziałów całkowania, rozmiar kroku oraz częściowa suma obliczona przez dany wątek.
  */
struct ThreadData {
    int thread_id = 0;        /**< Identyfikator wątku. */
    int num_threads = 0;      /**< Całkowita liczba wątków. */
    int num_intervals = 0;    /**< Liczba podziałów przedziału całkowania. */
    double step = 0.0;        /**< Rozmiar kroku, czyli szerokość jednego podprzedziału. */
    double partial_sum = 0.0; /**< Wynik częściowy obliczony przez wątek. */
};

/**
 * @brief Funkcja całkowana.
 *
 * Funkcja oblicza wartość f(x) = 4 / (1 + x^2), która jest podstawą obliczeń numerycznych.
 *
 * @param x Punkt, dla którego obliczana jest wartość funkcji.
 * @return Wartość funkcji f(x) = 4 / (1 + x^2).
 */
inline double f(double x) {
    return 4.0 / (1.0 + x * x);
}

/**
 * @brief Funkcja wykonywana przez każdy wątek.
 *
 * Oblicza częściową sumę wartości funkcji f(x) dla przypisanych fragmentów przedziału całkowania.
 * Fragmenty są wyznaczane na podstawie identyfikatora wątku oraz liczby wątków.
 *
 * @param data Wskaźnik do struktury ThreadData zawierającej parametry i wyniki obliczeń dla danego wątku.
 */
void calculate_pi(ThreadData* data) {
    double sum = 0.0;
    for (int i = data->thread_id; i < data->num_intervals; i += data->num_threads) {
        double x = (i + 0.5) * data->step; // Punkt środka podprzedziału
        sum += f(x);                      // Dodanie wartości funkcji
    }
    data->partial_sum = sum; // Zapisanie wyniku częściowego
}

/**
 * @brief Funkcja główna programu.
 *
 * Funkcja zarządza całym procesem obliczeń, w tym:
 * - Sprawdzaniem poprawności argumentów wejściowych.
 * - Tworzeniem wątków i przekazywaniem im odpowiednich parametrów.
 * - Sumowaniem wyników częściowych obliczonych przez wątki.
 * - Wyświetlaniem ostatecznego wyniku oraz czasu wykonania obliczeń.
 *
 * @param argc Liczba argumentów wiersza poleceń.
 * @param argv Tablica argumentów wiersza poleceń:
 * - argv[1]: Liczba przedziałów całkowania.
 * - argv[2]: Liczba wątków.
 * @return Kod zakończenia programu (0 - sukces, 1 - błąd).
 */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Użycie: " << argv[0] << " <liczba przedziałów> <liczba wątków>" << std::endl;
        return 1;
    }

    int num_intervals = std::atoi(argv[1]);
    int num_threads = std::atoi(argv[2]);

    if (num_intervals <= 0 || num_threads <= 0) {
        std::cerr << "Liczba przedziałów i liczba wątków musi być większa od zera." << std::endl;
        return 1;
    }

    std::vector<std::thread> threads;              /**< Wektor przechowujący wszystkie wątki. */
    std::vector<ThreadData> thread_data(num_threads); /**< Wektor przechowujący dane dla każdego wątku. */

    double step = 1.0 / num_intervals; /**< Obliczenie rozmiaru kroku. */
    auto start_time = std::chrono::high_resolution_clock::now(); /**< Zapis czasu rozpoczęcia obliczeń. */

    // Tworzenie wątków
    for (int i = 0; i < num_threads; ++i) {
        thread_data[i] = { i, num_threads, num_intervals, step, 0.0 };
        threads.emplace_back(calculate_pi, &thread_data[i]);
    }

    // Oczekiwanie na zakończenie wątków
    for (auto& thread : threads) {
        thread.join();
    }

    // Sumowanie wyników
    double pi = 0.0;
    for (const auto& data : thread_data) {
        pi += data.partial_sum;
    }
    pi *= step;

    auto end_time = std::chrono::high_resolution_clock::now(); /**< Zapis czasu zakończenia obliczeń. */
    std::chrono::duration<double> elapsed = end_time - start_time; /**< Obliczenie czasu wykonania obliczeń. */

    // Wyświetlenie wyniku i czasu obliczeń
    std::cout << "Przybliżona wartość liczby PI: " << pi << std::endl;
    std::cout << "Czas obliczeń: " << elapsed.count() << " sekund" << std::endl;

    return 0;
}

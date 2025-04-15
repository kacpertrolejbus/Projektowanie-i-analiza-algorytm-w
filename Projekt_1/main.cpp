#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <iomanip>
#include <cmath>
#include "heapSort.h"
#include "intSort.h"
#include "timSort.h"

using namespace std;


// Generator liczb pseudolosowych
random_device rd;
mt19937 gen(rd());

// Funkcja generuje losową tablicę o zadanym rozmiarze
vector<int> generateRanArr(int size)
{
    vector<int> arr(size);
    uniform_int_distribution<> distrib(0, 1000000);

    for (int i = 0; i < size; i++)
    {
        arr[i] = distrib(gen);  // Losowa liczba z zakresu 0–1,000,000
    }
    return arr;
}

// Generuje częściowo posortowaną tablicę — pewien procent jest posortowany
vector<int> generatePartSortArr(int size, double sortedPer)
{
    vector<int> arr = generateRanArr(size); // Losowa tablica
    int sortedCount = static_cast<int>(size * sortedPer / 100.0); // Ile elementów ma być posortowanych
    sort(arr.begin(), arr.begin() + sortedCount); // Sortujemy początkowy fragment
    return arr;
}

// Generuje tablicę posortowaną malejąco (odwrotnie)
vector<int> generateReverseSorArr(int size)
{
    vector<int> arr(size);
    for (int i = 0; i < size; i++)
    {
        arr[i] = size - i; // Wypełniamy wartości od size do 1
    }
    return arr;
}

// Sprawdza czy tablica jest posortowana rosnąco
bool isSor(const vector<int>& arr)
{
    for (size_t i = 1; i < arr.size(); i++)
    {
        if (arr[i] < arr[i - 1]) // Jeśli jakiś element jest mniejszy niż poprzedni
        {
            return false;
        }
    }
    return true;
}

// Funkcja testująca poprawność danego algorytmu sortowania
bool testSor(function<void(vector<int>&)> sortFunction, const string& testName)
{
    // Test 1: Pusta tablica
    vector<int> emptyArr;
    sortFunction(emptyArr);
    if (!isSor(emptyArr)) {
        cout << "Error with empty Array!" << std::endl;
        return false;
    }

    // Test 2: Tablica z jednym elementem
    vector<int> oneElArr = { 42 };
    sortFunction(oneElArr);
    if (!isSor(oneElArr))
    {
        cout << "Error with one element!" << endl;
        return false;
    }

    // Test 3: Już posortowana tablica
    vector<int> sortedArr = { 1, 2, 3, 4, 5 };
    sortFunction(sortedArr);
    if (!isSor(sortedArr)) {
        cout << "Error with sorted array!" << endl;
        return false;
    }

    // Test 4: Odwrotnie posortowana tablica
    vector<int> reverseArr = { 5, 4, 3, 2, 1 };
    sortFunction(reverseArr);
    if (!isSor(reverseArr))
    {
        cout << "Error with reverse!" << endl;
        return false;
    }

    // Test 5: Losowa tablica 1000 elementów (porównanie z std::sort)
    vector<int> randomArr = generateRanArr(1000);
    vector<int> sortedRanArr = randomArr;
    sort(sortedRanArr.begin(), sortedRanArr.end()); // Referencja
    sortFunction(randomArr);
    if (!isSor(randomArr) || randomArr != sortedRanArr)
    {
        cout << "Error with random array!" << endl;
        return false;
    }

    // Test 6: Tablica z powtórzeniami
    vector<int> rpeatArr = { 3,1,4,1,5,9,2,6,5,3,5 };
    sortFunction(rpeatArr);
    if (!isSor(rpeatArr))
    {
        cout << "Error with repeat!" << endl;
        return false;
    }

    cout << "GOOD" << endl;
    return true;
}

// Funkcja mierzy wydajność danego algorytmu sortowania w różnych przypadkach wejściowych
// Argumenty:
//  - sortFunction: wskaźnik do funkcji sortującej
//  - name: nazwa algorytmu (do wypisywania/zapisu wyników)
//  - sizes: rozmiary testowanych tablic
//  - repet: liczba powtórzeń każdego testu (dla średniej i odchylenia standardowego)
//  - resultsFile: plik CSV do którego zapisujemy wyniki
void benchSor(function<void(vector<int>&)> sortFunction, const string& name, const vector<int>& sizes, int repet, const string& resultsFile)
{
    // Otwieramy plik CSV do dopisywania wyników
    ofstream outFile(resultsFile, ios::app);
    if (!outFile.is_open()) {
        cerr << "Nie można otworzyć pliku : " << resultsFile << endl;
        return;
    }

    // Nagłówek testu w pliku
    outFile << "\nTESTY: " << name;
    outFile << "Rozmiar,Przypadek,Czas_sredni_ms,Odchylenie_standardowe_ms/n";

    // Procenty posortowania używane do testów częściowo uporządkowanych danych
    vector<double> sortedPer = { 0, 25, 50, 75, 95, 99, 99.7, 100 };

    // Główna pętla po rozmiarach tablicy
    for (int size : sizes) {
        std::cout << "Benchmarking " << name << " for array size " << size << std::endl;

        // === 1. Przypadek: tablica całkowicie losowa ===
        {
            vector<double> times;
            for (int rep = 0; rep < repet; rep++) {
                vector<int> arr = generateRanArr(size); // Tworzymy nową losową tablicę

                auto start = chrono::high_resolution_clock::now();
                sortFunction(arr); // Uruchamiamy sortowanie
                auto end = chrono::high_resolution_clock::now();

                chrono::duration<double, std::milli> elapsed = end - start; // Czas w ms
                times.push_back(elapsed.count());
            }

            // Obliczanie średniego czasu
            double sum = 0.0;
            for (double time : times) {
                sum += time;
            }
            double mean = sum / times.size();

            // Obliczanie odchylenia standardowego
            double variance = 0.0;
            for (double time : times) {
                variance += (time - mean) * (time - mean);
            }
            double stddev = std::sqrt(variance / times.size());

            // Zapis wyniku do pliku CSV
            outFile << size << ",Losowa," << mean << "," << stddev << "\n";
        }

        // === 2. Przypadki częściowo posortowane ===
        for (double percentage : sortedPer) {
            if (percentage == 0) continue;  // Pomijamy 0% (już przetestowane wyżej)
            if (percentage == 100) continue; // Pełne sortowanie będzie osobno

            vector<double> times;
            for (int rep = 0; rep < repet; rep++) {
                vector<int> arr = generatePartSortArr(size, percentage);

                auto start = chrono::high_resolution_clock::now();
                sortFunction(arr);
                auto end = chrono::high_resolution_clock::now();

                chrono::duration<double, std::milli> elapsed = end - start;
                times.push_back(elapsed.count());
            }

            double sum = 0.0;
            for (double time : times) {
                sum += time;
            }
            double mean = sum / times.size();

            double variance = 0.0;
            for (double time : times) {
                variance += (time - mean) * (time - mean);
            }
            double stddev = std::sqrt(variance / times.size());

            outFile << size << ",Posortowane_" << percentage << "%," << mean << "," << stddev << "\n";
        }

        // === 3. Przypadek: w 100% posortowana tablica ===
        {
            vector<double> times;
            for (int rep = 0; rep < repet; rep++) {
                vector<int> arr = generateRanArr(size);
                sort(arr.begin(), arr.end()); // Ręcznie sortujemy, żeby mieć 100%

                auto start = chrono::high_resolution_clock::now();
                sortFunction(arr);
                auto end = chrono::high_resolution_clock::now();

                chrono::duration<double, std::milli> elapsed = end - start;
                times.push_back(elapsed.count());
            }

            double sum = 0.0;
            for (double time : times) {
                sum += time;
            }
            double mean = sum / times.size();

            double variance = 0.0;
            for (double time : times) {
                variance += (time - mean) * (time - mean);
            }
            double stddev = std::sqrt(variance / times.size());

            outFile << size << ",Posortowane_100%," << mean << "," << stddev << "\n";
        }

        // === 4. Przypadek: odwrotnie posortowana tablica ===
        {
            vector<double> times;
            for (int rep = 0; rep < repet; rep++) {
                vector<int> arr = generateReverseSorArr(size);

                auto start = chrono::high_resolution_clock::now();
                sortFunction(arr);
                auto end = chrono::high_resolution_clock::now();

                chrono::duration<double, std::milli> elapsed = end - start;
                times.push_back(elapsed.count());
            }

            double sum = 0.0;
            for (double time : times) {
                sum += time;
            }
            double mean = sum / times.size();

            double variance = 0.0;
            for (double time : times) {
                variance += (time - mean) * (time - mean);
            }
            double stddev = std::sqrt(variance / times.size());

            outFile << size << ",Odwrotnie_posortowane," << mean << "," << stddev << "\n";
        }
    }

    // Zamykamy plik po zakończeniu wszystkich testów
    outFile.close();
}


int main()
{
    // Nazwa pliku CSV, do którego zostaną zapisane wyniki benchmarków
    string resultsFile = "wyniki_sortowania.csv";

    // Tworzenie i czyszczenie pliku wynikowego (nadpisuje zawartość, jeśli plik istnieje)
    ofstream initialFile(resultsFile);

    // Rozmiary tablic, dla których będziemy testować algorytmy
    vector<int> sizes = { 1000, 10000, 50000, 100000, 500000, 1000000 };

    // Liczba powtórzeń każdego testu (dla uśrednienia wyników i obliczenia odchylenia standardowego)
    int repetitions = 100;

    // Lista algorytmów sortowania w postaci par: (nazwa, funkcja sortująca)
    vector<pair<string, function<void(vector<int>&)>>> sortFunctions = {
        {"HeapSort", heapSort},
        {"TimSort", timsort},
        {"IntroSort", introSort}
    };

    // Sekcja testowania poprawności algorytmów
    std::cout << "\nTESTOWANIE POPRAWNOŚCI ALGORYTMÓW SORTOWANIA\n";
    std::cout << "=============================================\n";

    // Dla każdego algorytmu wykonujemy test poprawności
    for (const auto& algorithm : sortFunctions) {
        const string& name = algorithm.first;                  // Nazwa algorytmu
        const function<void(vector<int>&)>& function = algorithm.second;  // Funkcja sortująca

        // Wywołanie funkcji testującej poprawność sortowania
        if (!testSor(function, name)) {
            // Jeśli test się nie powiedzie, wypisujemy błąd i kończymy program
            cerr << "Test poprawności dla " << name << " nie powiódł się!\n";
            return 1;
        }
    }

    // Sekcja benchmarkowania algorytmów (pomiar czasu działania)
    std::cout << "\nBENCHMARKING ALGORYTMÓW SORTOWANIA\n";
    std::cout << "==================================\n";
    std::cout << "Dla każdego algorytmu i rozmiaru tablicy wykonywanych jest " << repetitions << " pomiarów.\n";
    std::cout << "Wyniki będą zapisywane do pliku: " << resultsFile << "\n\n";

    // Dla każdego algorytmu wykonujemy pomiary czasu działania
    for (const auto& algorithm : sortFunctions) {
        const string& name = algorithm.first;                   // Nazwa algorytmu
        const function<void(vector<int>&)>& function = algorithm.second;   // Funkcja sortująca

        // Informacja w konsoli, który algorytm aktualnie testujemy
        cout << "Testowanie algorytmu: " << name << "\n";
        cout << "-----------------------\n";

        // Wywołanie funkcji benchmarkującej
        benchSor(function, name, sizes, repetitions, resultsFile);

        // Informacja o zakończeniu testów dla danego algorytmu
        cout << "Zakończono testy dla: " << name << "\n\n";
    }

    // Informacja końcowa — wszystkie testy zakończone
    std::cout << "\nWszystkie testy zakończone. Wyniki zapisane do pliku: " << resultsFile << std::endl;

    return 0;
}




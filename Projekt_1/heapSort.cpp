#include <iostream>
#include "heapSort.h"

using namespace std;

/*
 * Funkcja pomocnicza heapify: zapewnia własność kopca dla poddrzewa z korzeniem w indeksie i.
 * arr - wektor liczb całkowitych (do posortowania)
 * n - rozmiar aktualnego zakresu kopca
 * i - indeks korzenia poddrzewa, które poddajemy kopcowaniu
 */

void heapify(vector<int>& arr, int n, int i)
{
	int larg = i;                // Załóżmy na początku, że największy element to korzeń
	int left = i * 2 + 1;        // Indeks lewego dziecka w tablicy
	int right = i * 2 + 2;       // Indeks prawego dziecka

	// Sprawdzamy, czy lewe dziecko jest większe od dotychczas największego elementu
	if (left < n && arr[left] > arr[larg])
	{
		larg = left;
	}
	// Sprawdzamy, czy prawe dziecko jest większe od dotychczas największego
	if (right < n && arr[right] > arr[larg])
	{
		larg = right;
	}

	// Jeśli największy element nie był korzeniem — zamień i rekurencyjnie przekształć poddrzewo
	if (larg != i)
	{
		swap(arr[i], arr[larg]);        // Zamiana miejscami
		heapify(arr, n, larg);          // Rekurencyjne wywołanie dla przekształconego poddrzewa
	}
}

/*
 * Główna funkcja sortująca za pomocą algorytmu heapsort.
 */
void heapSort(vector<int>& arr)
{
	int n = arr.size();           // Pobierz rozmiar tablicy

	// Budowanie kopca (maks-heap) - zaczynając od ostatniego rodzica
	for (int i = n / 2 - 1; i >= 0; i--)
	{
		heapify(arr, n, i);       // Upewniamy się, że poddrzewo o korzeniu i spełnia warunek kopca
	}

	// Właściwe sortowanie — iteracyjnie przenosimy największy element na koniec
	for (int i = n - 1; i > 0; i--)
	{
		swap(arr[0], arr[i]);     // Największy element (na korzeniu) idzie na koniec
		heapify(arr, i, 0);       // Przywróć własność kopca dla pozostałej części
	}
}
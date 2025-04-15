#include <iostream>
#include <algorithm>
#include "intSort.h"


// Zwraca indeks elementu środkowego (mediany) spośród trzech podanych indeksów.
// Używana do lepszego wyboru pivota w quicksort, co zmniejsza szansę na zły przypadek.
int medianOfThree(vector<int>& arr, int a, int b, int c)
{
	if (arr[a] < arr[b])
	{
		if (arr[b] < arr[c])
		{
			return b;
		}
		else if (arr[a] < arr[c])
		{
			return c;
		}
		else
		{
			return a;
		}
	}
	else
	{
		if (arr[a] < arr[c])
		{
			return a;
		}
		else if (arr[b] < arr[c])
		{
			return c;
		}
		else
		{
			return b;
		}
	}
}

// Funkcja dzieląca tablicę wg pivota — klasyczny podział z quicksorta.
int partition(vector<int>& arr, int low, int high)
{
	int mid = low + (high - low) / 2;                         // Środek zakresu
	int pivotIndex = medianOfThree(arr, low, mid, high);      // Używamy mediany z 3 jako pivota

	swap(arr[pivotIndex], arr[high]);                         // Przenosimy pivota na koniec
	int pivot = arr[high];
	int i = low - 1;

	for (int j = low; j <= high - 1; j++) {
		if (arr[j] <= pivot) {
			i++;
			swap(arr[i], arr[j]);                             // Przesuwamy elementy mniejsze od pivota w lewo
		}
	}
	swap(arr[i + 1], arr[high]);                              // Przenosimy pivot na właściwe miejsce
	return (i + 1);                                           // Zwracamy nową pozycję pivota
}

// Dla małych podtablic stosujemy insertion sort (lepszy w praktyce przy n < 16)
void insertionSortForIntroSort(vector<int>& arr, int begin, int end)
{
	for (int i = begin + 1; i <= end; i++) {
		int key = arr[i];
		int j = i - 1;

		while (j >= begin && arr[j] > key) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

// Przekształca poddrzewo w kopiec maksymalny — klasyczna funkcja heapify z uwzględnieniem przesunięcia "begin"
void heapify(vector<int>& arr, int begin, int end, int i)
{
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < end && arr[begin + left] > arr[begin + largest])
		largest = left;

	if (right < end && arr[begin + right] > arr[begin + largest])
		largest = right;

	if (largest != i) {
		swap(arr[begin + i], arr[begin + largest]);
		heapify(arr, begin, end, largest);      // Rekurencyjne przywrócenie kopca
	}
}


// Sortuje zakres tablicy za pomocą heapsorta (stosowany, gdy rekurencja jest zbyt głęboka)
void heapSortForIntroSort(std::vector<int>& arr, int begin, int end) {
	int size = end - begin;

	for (int i = size / 2 - 1; i >= 0; i--)
		heapify(arr, begin, size, i);        // Budowanie kopca

	for (int i = size - 1; i > 0; i--) {
		swap(arr[begin], arr[begin + i]);    // Przenoszenie maksymalnego elementu na koniec
		heapify(arr, begin, i, 0);           // Odtworzenie kopca
	}
}


// Wewnętrzna wersja introsort (rekurencyjna), z limitem głębokości rekurencji
void introSortU(vector<int>& arr, int begin, int end, int depthLimit) {
	int size = end - begin;

	if (size < 16) {
		insertionSortForIntroSort(arr, begin, end - 1);   // Dla małych tablic insertion sort
		return;
	}

	if (depthLimit == 0) {
		heapSortForIntroSort(arr, begin, end);           // Gdy rekurencja zbyt głęboka — heapsort
		return;
	}

	int pivot = partition(arr, begin, end - 1);          // Podział tablicy

	introSortU(arr, begin, pivot, depthLimit - 1);       // Rekurencja lewa
	introSortU(arr, pivot + 1, end, depthLimit - 1);     // Rekurencja prawa
}


// Główna funkcja sortująca — użytkownik używa tylko tego
void introSort(vector<int>& arr) {
	int n = arr.size();

	int depthLimit = 2 * std::log2(n);      // Maksymalna głębokość rekurencji
	// Po jej przekroczeniu przechodzimy na heapsort

	introSortU(arr, 0, n, depthLimit);      // Wywołanie funkcji rekurencyjnej
}

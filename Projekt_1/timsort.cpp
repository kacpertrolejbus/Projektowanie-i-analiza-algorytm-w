#include <iostream>
#include <vector>
#include "timsort.h"

using namespace std;

// Sortuje fragment tablicy od indeksu `left` do `right` włącznie przy użyciu sortowania przez wstawianie
void insertSort(vector<int>& arr, int left, int right)
{
	for (int i = left + 1; i <= right; i++)
	{
		int tem = arr[i];                  // Element do wstawienia
		int j = i - 1;

		// Przesuwamy większe elementy w prawo
		while (j >= left && arr[j] > tem)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = tem;                  // Wstawiamy element w odpowiednie miejsce
	}
}


// Scalanie dwóch posortowanych podtablic: [left..mid] i [mid+1..right]
void merge(vector<int>& arr, int left, int mid, int right)
{
	int len1 = mid - left + 1;              // Długość lewej tablicy
	int len2 = right - mid;                 // Długość prawej tablicy

	vector<int> lArr(len1);                 // Tymczasowa tablica lewa
	vector<int> rArr(len2);                 // Tymczasowa tablica prawa

	// Kopiujemy dane do tablic tymczasowych
	for (int i = 0; i < len1; i++)
		lArr[i] = arr[left + i];
	for (int i = 0; i < len2; i++)
		rArr[i] = arr[mid + 1 + i];

	int i = 0, j = 0, k = left;

	// Scalanie właściwe
	while (i < len1 && j < len2) {
		if (lArr[i] <= rArr[j])
			arr[k++] = lArr[i++];
		else
			arr[k++] = rArr[j++];
	}
	// Kopiowanie pozostałych elementów
	while (i < len1)
		arr[k++] = lArr[i++];
	while (j < len2)
		arr[k++] = rArr[j++];
}


// Implementacja algorytmu Timsort
void timsort(vector<int>& arr)
{
	int n = arr.size();

	// Pierwszy krok: dzielenie tablicy na m-elementowe fragmenty i sortowanie insertion sortem
	for (int i = 0; i < n; i += m) {
		insertSort(arr, i, min((i + m - 1), (n - 1)));  // sortujemy każdy blok o rozmiarze `m`
	}

	// Łączenie posortowanych bloków
	for (int size = m; size < n; size = 2 * size) {     // rozmiar bloków się podwaja

		for (int left = 0; left < n; left += 2 * size) {
			int mid = left + size - 1;
			int right = min((left + 2 * size - 1), (n - 1));

			// Scal tylko wtedy, gdy są dwa bloki
			if (mid < right) {
				merge(arr, left, mid, right);
			}
		}
	}
}


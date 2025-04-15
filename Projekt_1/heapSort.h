#ifndef HEAPSORT_H
#define HEAPSORT_H
#include <vector>
using namespace std;

void heapSort(vector<int>& arr);
void heapify(vector<int>& arr, int n, int i);

#endif
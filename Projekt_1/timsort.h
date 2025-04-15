#ifndef TIMSORT_H
#define TIMSORT_H
#include <vector>
#include <algorithm>

using namespace std;
const int m = 32;

void timsort(vector<int>& arr);
void insertSort(vector<int>& arr, int left, int right);
void merge(vector<int>& arr, int left, int right, int mid);

#endif

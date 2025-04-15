#ifndef INTSORT_H
#define INTSORT_H
#include<vector>
#include<cmath>
using namespace std;

void introSort(vector<int>& arr);
void introSortU(vector<int>& arr, int begin, int end, int depLimit);
int partition(vector<int>& arr, int low, int high);
void insertionSortForIntroSort(vector<int>& arr, int begin, int end);
void heapify(vector<int>& arr, int begin, int end, int i);
void heapSortForIntroSort(vector<int>& arr, int begin, int end);
int medianOfThree(vector<int>& arr, int a, int b, int c);

#endif
// 计数排序（Counting sort）

// 导入可视化库 {
#include "algorithm-visualizer.h"
// }
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 定义输入数组变量
#define N 15
#define MIN 1
#define MAX 20

void CountingSort(int ini_arr[], int sorted_arr[], int count_arr[], int n,
                  int max_val);

// 定义跟踪变量 {
Array1DTracer arrayTracer("Array");
Array1DTracer countsTracer("Counts");
Array1DTracer sortedArrayTracer("Sorted Array");
// }

int main() {
  // 定义跟踪变量 {
  int array[N];
  int counts[MAX + 1] = {0};
  int sortedArray[N] = {0};
  Randomize::Array1D<int>(N, *(new Randomize::Integer(MIN, MAX)))
      .fill(&array[0]);
  arrayTracer.set(array);
  countsTracer.set(counts);
  sortedArrayTracer.set(sortedArray);
  Layout::setRoot(
      VerticalLayout({arrayTracer, countsTracer, sortedArrayTracer}));
  Tracer::delay();
  // }

  CountingSort(array, sortedArray, counts, N, MAX);
  return 0;
}

void CountingSort(int ini_arr[], int sorted_arr[], int count_arr[], int n,
                  int max_val) {
  for (int i = 0; i < n; i++) {
    int number = ini_arr[i];
    count_arr[number]++;
    // visualize {
    arrayTracer.select(i);
    countsTracer.patch(number, count_arr[number]);
    Tracer::delay();
    countsTracer.depatch(number);
    arrayTracer.deselect(i);
    // }
  }
  for (int i = 1; i <= max_val; i++) {
    count_arr[i] += count_arr[i - 1];
    // visualize {
    countsTracer.select(i - 1);
    countsTracer.patch(i, count_arr[i]);
    Tracer::delay();
    countsTracer.depatch(i);
    countsTracer.deselect(i - 1);
    // }
  }

  for (int i = n - 1; i >= 0; i--) {
    int number = ini_arr[i];
    int count = count_arr[number];
    sorted_arr[count - 1] = number;
    count_arr[number]--;
    // visualize {
    arrayTracer.select(i);
    countsTracer.select(number);
    sortedArrayTracer.patch(count - 1, sorted_arr[count - 1]);
    countsTracer.patch(number, count_arr[number]);
    Tracer::delay();
    sortedArrayTracer.depatch(count - 1);
    countsTracer.depatch(number);
    countsTracer.deselect(number);
    arrayTracer.deselect(i);
    // }
  }
}
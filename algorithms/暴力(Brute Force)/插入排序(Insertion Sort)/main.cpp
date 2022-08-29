// 插入排序（Insertion Sort）

// 导入可视化库 {
#include "algorithm-visualizer.h"
// }
#include <sstream>

// 定义输入数组变量
#define N 15
#define MIN 1
#define MAX 20

void InsertionSort(int len, int array[]);

// 定义跟踪变量 {
void PrintlnArray(const char *msg, int count, int array[]);
ChartTracer chartTracer("ChartTracer");
Array1DTracer array1DTracer("Array1DTracer");
LogTracer logger("LogTracer");
// }

int main() {
  // 定义跟踪变量 {
  int array[N];
  Randomize::Array1D<int>(N, *(new Randomize::Integer(MIN, MAX)))
      .fill(&array[0]);
  array1DTracer.set(array);
  array1DTracer.chart(chartTracer);
  Layout::setRoot(VerticalLayout({chartTracer, array1DTracer, logger}));
  Tracer::delay();
  // }

  // logger {
  PrintlnArray("original", N - 1, array);
  // }

  InsertionSort(N - 1, array);

  // logger {
  PrintlnArray("sorted", N - 1, array);
  // }

  return 0;
}

// logger {
void PrintlnArray(const char *msg, int count, int array[]) {
  std::ostringstream os;
  os << msg << " array = [";
  for (int i = 0; i < count; i++) {
    os << array[i] << ", ";
  }
  os << "]";
  logger.println(os.str());
}
// }
void InsertionSort(int k, int array[]) {
  int i, j;
  for (i = 1; i < k; i++)
  {
    int temp = array[i];
    // visualize {
    std::ostringstream os;
    os << "insert " << temp;
    logger.println(os.str());
    array1DTracer.select(i);
    Tracer::delay();
    // }
    for (j = i - 1; j >= 0 && array[j] > temp; j--) {
      array[j + 1] = array[j];
      // visualize {
      array1DTracer.patch(j + 1, array[j + 1]);
      Tracer::delay();
      array1DTracer.depatch(j + 1);
      // }
    }
    array[j + 1] = temp;
    // visualize {
    array1DTracer.patch(j + 1, array[j + 1]);
    Tracer::delay();
    array1DTracer.depatch(j + 1);
    array1DTracer.deselect(i);
    // }
  }
}

// 梳排序（Comb sort）

// 导入可视化库 {
#include "algorithm-visualizer.h"
// }
#include <sstream>

// 定义输入数组变量
#define N   15
#define MIN 1
#define MAX 20

void CombSort(int arr[], int len);

// 定义跟踪变量 {
void PrintlnArray(const char* msg, int count,int array[]);
ChartTracer chartTracer("ChartTracer");
Array1DTracer array1DTracer("Array1DTracer");
LogTracer logger("LogTracer");
// }

int main() {
    // 定义跟踪变量 {
    int array[N];
    Randomize::Array1D<int>(N, *(new Randomize::Integer(MIN, MAX))).fill(&array[0]);
    array1DTracer.set(array);
    array1DTracer.chart(chartTracer);
    Layout::setRoot(VerticalLayout({ chartTracer, array1DTracer, logger }));
    Tracer::delay();
    // }

    // logger {
    PrintlnArray("original", N - 1, array);
    // }

    CombSort(array, N - 1);

    // logger {
    PrintlnArray("sorted", N - 1, array);
    // }

    return 0;
}

// logger {
void PrintlnArray(const char* msg, int count,int array[]) 
{
    std::ostringstream os;
    os << msg << " array = [";
    for (int i = 0; i < count; i++) {
        os << array[i] << ", ";
    }
    os << "]";
    logger.println(os.str());
}
// }
void CombSort(int arr[], int len) {

	double shrink_factor = 0.8;
	int gap = len, swapped = 1, i;
	int temp;
	while (gap > 1 || swapped) {
        
		if (gap > 1) 
			gap *= shrink_factor;
		swapped = 0;
		for (i = 0; gap + i < len; i++) {
            // visualize {
            array1DTracer.select(i);
            array1DTracer.select(i + gap);
            Tracer::delay();
            // }
			if (arr[i] > arr[i + gap]) {
                // logger {
                std::ostringstream os;
                os << "swap " << arr[i] << " and " << arr[i + gap];
                logger.println(os.str());
                // }
				temp = arr[i];
				arr[i] = arr[i + gap];
				arr[i + gap] = temp;
                // visualize {
                array1DTracer.patch(i, arr[i]);
                array1DTracer.patch(i + gap, arr[i + gap]);
                Tracer::delay();
                array1DTracer.depatch(i);
                array1DTracer.depatch(i + gap);
                // }

				swapped = 1;
			}
            // visualize {
            array1DTracer.deselect(i);
            array1DTracer.deselect(i + gap);
            // }
        }
            
	}
}

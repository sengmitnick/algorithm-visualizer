/**
 * 梳排序（Comb sort）是一种由Wlodzimierz Dobosiewicz于1980年所发明的不稳定排序算法，并由Stephen Lacey和Richard Box于1991年四月号的Byte杂志中推广。
 * 梳排序是改良自冒泡排序和快速排序，其要旨在于消除乌龟，亦即在数组尾部的小数值，这些数值是造成冒泡排序缓慢的主因。
 * 相对地，兔子，亦即在数组前端的大数值，不影响冒泡排序的性能。
 * 在冒泡排序中，只比较数组中相邻的二项，即比较的二项的间距（Gap）是1，梳排序提出此间距其实可大于1，改自插入排序的希尔排序同样提出相同观点。
 * 梳排序中，开始时的间距设置为数组长度，并在循环中以固定比率递减，通常递减率设置为1.3。
 * 在一次循环中，梳排序如同冒泡排序一样把数组从首到尾扫描一次，比较及交换两项，不同的是两项的间距不固定于1。
 * 如果间距递减至1，梳排序假定输入数组大致排序好，并以冒泡排序作最后检查及修正。
 *
 * https://baike.baidu.com/item/%E6%A2%B3%E6%8E%92%E5%BA%8F
**/

// 导入可视化库 {
#include "algorithm-visualizer.h"
// }

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

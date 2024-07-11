#include <future>
#include <chrono>
#include <iostream>
#include <random>

bool make_thread = true;

void merge(int* arr, int l, int m, int r) {
    int nl = m - l + 1;
    int nr = r - m;

    // создаем временные массивы
    int* left = new int[nl];
    int* right = new int[nr];

    // копируем данные во временные массивы
    for (size_t i = 0; i < nl; i++)
        left[i] = arr[l + i];
    for (size_t j = 0; j < nr; j++)
        right[j] = arr[m + 1 + j];

    int i = 0, j = 0;
    int k = l;  // начало левой части

    while (i < nl && j < nr) {
        // записываем минимальные элементы обратно во входной массив
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    // записываем оставшиеся элементы левой части
    while (i < nl) {
        arr[k] = left[i];
        i++;
        k++;
    }
    // записываем оставшиеся элементы правой части
    while (j < nr) {
        arr[k] = right[j];
        j++;
        k++;
    }

    delete[] left;
    delete[] right;
}

void mergeSort(int* arr, int l, int r) {
    if (l >= r)
        return;

    int m = (l + r - 1) / 2;

    if (make_thread && (r - l > 100000)) {
        auto f = async(std::launch::async, [&]() {
            mergeSort(arr, l, m);
        });
        mergeSort(arr, m + 1, r);
    }
    else {
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
    }

    merge(arr, l, m, r);
}

int main() {
    srand(0);
    int arr_size = 100000000;
    int* array = new int[arr_size];
    //int arr_size = 7;
    //int* array = new int[arr_size]{ 3, 5, 1, 2, 1, 0, 5, 4 };
    time_t start, end;

    // многопоточный запуск
    for (size_t i = 0; i < arr_size; i++) {
        array[i] = rand() % 500000;
    }

    time(&start);
    mergeSort(array, 0, arr_size);
    time(&end);

    double seconds = difftime(end, start);
    std::cout << "The time: " << seconds << " seconds" << std::endl;

    for (size_t i = 0; i < arr_size - 1; i++) {
        if (array[i] > array[i + 1]) {
            std::cout << "Unsorted" << std::endl;
            break;
        }
    }

    // однопоточный запуск
    for (size_t i = 0; i < arr_size; i++) {
        array[i] = rand() % 500000;
    }

    make_thread = false;

    time(&start);
    mergeSort(array, 0, arr_size);
    time(&end);

    seconds = difftime(end, start);
    std::cout << "The time: " << seconds << " seconds" << std::endl;

    for (size_t i = 0; i < arr_size - 1; i++) {
        if (array[i] > array[i + 1]) {
            std::cout << "Unsorted" << std::endl;
            break;
        }
    }

    delete[] array;

    return 0;
}
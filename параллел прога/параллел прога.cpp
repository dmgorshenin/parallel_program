#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <time.h>

using namespace std;
using namespace std::chrono;


// Функция для генерации случайной матрицы и записи ее в файл
void generateRandomMatrixToFile(int size, const string& filename) {
    int** matrix = new int* [size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new int[size];
    }

    srand(time(NULL));
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            matrix[i][j] = rand() % 100;
        }
    }

    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        exit(1);
    }

    file << size << endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            file << matrix[i][j] << " ";
        }
        file << endl;
    }

    file.close();

    for (int i = 0; i < size; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Функция для чтения матрицы из файла
void readMatrixFromFile(int* size, const string& filename, int*** matrix) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        exit(1);
    }

    file >> *size;

    *matrix = new int* [*size];
    for (int i = 0; i < *size; ++i) {
        (*matrix)[i] = new int[*size];
        for (int j = 0; j < *size; ++j) {
            file >> (*matrix)[i][j];
        }
    }

    file.close();
}

// Функция для записи матрицы в файл
void writeMatrixToFile(int size, int** matrix, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        exit(1);
    }

    file << size << endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            file << matrix[i][j] << " ";
        }
        file << endl;
    }

    file.close();
}

// Функция для умножения двух матриц
void multiplyMatrices(int size, int** matrix1, int** matrix2, int*** result) {
    *result = new int* [size];
    for (int i = 0; i < size; ++i) {
        (*result)[i] = new int[size];
        for (int j = 0; j < size; ++j) {
            (*result)[i][j] = 0;
            for (int k = 0; k < size; ++k) {
                (*result)[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

// Функция для освобождения памяти, выделенной для матрицы
void freeMatrix(int size, int** matrix) {
    for (int i = 0; i < size; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

//Функция записи результатов
void writeResultsToFile(int size, long long generation_time, long long computation_time, long long total_time, long long task_size, const string& filename) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Unable to open file: "<< filename << endl;
        exit(1);
    }

    file << size << endl << generation_time << endl << computation_time << endl << total_time << endl << task_size << endl;

    file.close();
}

int main() {
    setlocale(LC_ALL, "Russian");
    int size;
    int** matrix1;
    int** matrix2;
    int** resultMatrix;
    int N = 500;

    //while (N <= 3000) {
        generateRandomMatrixToFile(N, "matrix1.txt");
        generateRandomMatrixToFile(N, "matrix2.txt");

        cout << "Файлы с данными для матриц сгенерированы" << endl;

        auto start = high_resolution_clock::now();

        readMatrixFromFile(&size, "matrix1.txt", &matrix1);
        readMatrixFromFile(&size, "matrix2.txt", &matrix2);

        auto start_compute = high_resolution_clock::now();
        multiplyMatrices(size, matrix1, matrix2, &resultMatrix);
        auto stop_compute = high_resolution_clock::now();

        cout << "Матрицы перемножены." << endl;

        writeMatrixToFile(size, resultMatrix, "result_matrix.txt");

        auto stop = high_resolution_clock::now();

        freeMatrix(size, matrix1);
        freeMatrix(size, matrix2);
        freeMatrix(size, resultMatrix);

        /*auto duration_generation = duration_cast<milliseconds>(start_compute - start);
        auto duration_computation = duration_cast<milliseconds>(stop_compute - start_compute);
        auto duration_total = duration_cast<milliseconds>(stop - start);
        long long task_size = static_cast<long long>(size) * static_cast<long long>(size) * static_cast<long long>(size);

        cout << "Время чтения матриц: " << duration_generation.count() << " мс" << endl;
        cout << "Время умножения матриц: " << duration_computation.count() << " мс" << endl;
        cout << "Общее время выполнения: " << duration_total.count() << " мс" << endl;
        cout << "Объем задачи: " << task_size << endl;

        writeResultsToFile(N, duration_generation.count(), duration_computation.count(), duration_total.count(), task_size, "results.txt");
        N += 100;*/
    /*}*/
    return 0;
}
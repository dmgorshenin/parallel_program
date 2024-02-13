#include <iostream>
#include <fstream>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;


// Функция для генерации случайной матрицы и записи ее в файл
void generateRandomMatrixToFile(int rows, int cols, const string& filename) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100); 

    int** matrix = new int* [rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dis(gen);
        }
    }

    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        exit(1);
    }

    file << rows << " " << cols << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file << matrix[i][j] << " ";
        }
        file << endl;
    }

    file.close();

    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Функция для чтения матрицы из файла
void readMatrixFromFile(int* rows, int* cols, const string& filename, int*** matrix) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        exit(1);
    }

    file >> *rows >> *cols;

    *matrix = new int* [*rows];
    for (int i = 0; i < *rows; ++i) {
        (*matrix)[i] = new int[*cols];
        for (int j = 0; j < *cols; ++j) {
            file >> (*matrix)[i][j];
        }
    }

    file.close();
}

// Функция для записи матрицы в файл
void writeMatrixToFile(int rows, int cols, int** matrix, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        exit(1);
    }

    file << rows << " " << cols << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file << matrix[i][j] << " ";
        }
        file << endl;
    }

    file.close();
}

// Функция для умножения двух матриц
void multiplyMatrices(int rows1, int cols, int** matrix1, int** matrix2, int*** result) {
    *result = new int* [rows1];
    for (int i = 0; i < rows1; ++i) {
        (*result)[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            (*result)[i][j] = 0;
            for (int k = 0; k < cols; ++k) {
                (*result)[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

// Функция для освобождения памяти, выделенной для матрицы
void freeMatrix(int rows, int** matrix) {
    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int rows, cols;
    int** matrix1;
    int** matrix2;
    int** resultMatrix;
    int N = 500;

    generateRandomMatrixToFile(N, N, "matrix1.txt"); 
    generateRandomMatrixToFile(N, N, "matrix2.txt"); 


    auto start = high_resolution_clock::now();

    readMatrixFromFile(&rows, &cols, "matrix1.txt", &matrix1);
    readMatrixFromFile(&rows, &cols, "matrix2.txt", &matrix2);

    auto start_compute = high_resolution_clock::now();
    multiplyMatrices(rows, cols, matrix1, matrix2, &resultMatrix);
    auto stop_compute = high_resolution_clock::now();

    writeMatrixToFile(rows, cols, resultMatrix, "result_matrix.txt");

    auto stop = high_resolution_clock::now();

    cout << "Файлы с данными для матриц сгенерированы и перемножены." << endl;

    freeMatrix(rows, matrix1);
    freeMatrix(cols, matrix2);
    freeMatrix(rows, resultMatrix);

    auto duration_generation = duration_cast<milliseconds>(start_compute - start);
    auto duration_computation = duration_cast<milliseconds>(stop_compute - start_compute);
    auto duration_total = duration_cast<milliseconds>(stop - start);
    long long task_size = static_cast<long long>(rows) * static_cast<long long>(cols) * static_cast<long long>(cols);

    cout << "Время чтения матриц: " << duration_generation.count() << " мс" << endl;
    cout << "Время умножения матриц: " << duration_computation.count() << " мс" << endl;
    cout << "Общее время выполнения: " << duration_total.count() << " мс" << endl;
    cout << "Объем задачи: " << task_size;
    return 0;
}
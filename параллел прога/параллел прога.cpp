#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <time.h>
#include <omp.h>

using namespace std;
using namespace std::chrono;

class MultiplyMatrix {
private:
    int size = 500;
    int** matrix1;
    int** matrix2;
    int** result_matrix;
    string filename_matrix1 = "matrix1.txt";
    string filename_matrix2 = "matrix2.txt";
    string filename_matrix_res = "result_matrix.txt";

    int** create_matrix() {
        int** matrix = new int* [size];
        for (int i = 0; i < size; ++i)
            matrix[i] = new int[size];
        return matrix;
    }

    void generateRandomMatrixToFile(int*** matrix, const string& filename) {
        srand(time(NULL));
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                (*matrix)[i][j] = rand() % 100;
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
                file << (*matrix)[i][j] << " ";
            }
            file << endl;
        }

        file.close();
    }

    void writeMatrixToFile(int** matrix, const string& filename) {
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

public:
    MultiplyMatrix(int N) {
        this->size = N;
        matrix1 = create_matrix();
        matrix2 = create_matrix();
        result_matrix = create_matrix();
    }

    ~MultiplyMatrix() {
        clean(&matrix1);
        clean(&matrix2);
        clean(&result_matrix);
    }

    void generate_and_save_matrices() {
        generateRandomMatrixToFile(&matrix1, filename_matrix1);
        generateRandomMatrixToFile(&matrix2, filename_matrix2);
    }

    void multiplyMatrices() {
        #pragma omp parallel for 
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                result_matrix[i][j] = 0;
                for (int k = 0; k < size; ++k) {
                    result_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }

        writeMatrixToFile(result_matrix, filename_matrix_res);
    }

    void clean(int*** matrix) {
        for (int i = 0; i < size; ++i) {
            delete[] (*matrix)[i];
        }
        delete[] (*matrix);
    }
};

void writeTimeToFile(long long computation_time, const string& filename) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Unable to open file: "<< filename << endl;
        exit(1);
    }

    file << computation_time << endl;

    file.close();
}

void writeTaskSizeToFile(int size, long long task_size, const string& filename) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        exit(1);
    }

    file << size << endl << task_size << endl;

    file.close();
}

//void readMatrixFromFile(int* size, const string& filename, int*** matrix) {
//    ifstream file(filename);
//    if (!file.is_open()) {
//        cerr << "Unable to open file: " << filename << endl;
//        exit(1);
//    }
//
//    file >> *size;
//
//    *matrix = new int* [*size];
//    for (int i = 0; i < *size; ++i) {
//        (*matrix)[i] = new int[*size];
//        for (int j = 0; j < *size; ++j) {
//            file >> (*matrix)[i][j];
//        }
//    }
//
//    file.close();
//}

int main() {
    setlocale(LC_ALL, "Russian");
    int N = 500;
    int num_threads = 16;
    string file_stat = "stats8.txt";


    while (N <= 2200) {
        
        long long task_size = static_cast<long long>(N) * static_cast<long long>(N) * static_cast<long long>(N);
        writeTaskSizeToFile(N, task_size, file_stat);
        cout << "Размер матриц " << N << "x" << N << endl << "Объем задачи: " << task_size << endl;

        for (size_t i = 0; i < 10; ++i)
        {
            MultiplyMatrix matrix(N);

            matrix.generate_and_save_matrices();

            omp_set_num_threads(num_threads);
            cout << "Количество ядер: " << omp_get_num_threads()<< endl;

            cout << "Файлы с данными для матриц сгенерированы" << endl;

            auto start_compute = high_resolution_clock::now();
            matrix.multiplyMatrices();
            auto stop_compute = high_resolution_clock::now();

            cout << "Матрицы перемножены." << endl;

            auto duration_computation = duration_cast<milliseconds>(stop_compute - start_compute);

            cout << "Время умножения матриц: " << duration_computation.count() << " мс" << endl;
            writeTimeToFile(duration_computation.count(), file_stat);
        }
        N += 100;
    }

    return 0;
}


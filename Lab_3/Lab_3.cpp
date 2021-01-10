// Lab_3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <omp.h>
#include <Windows.h>
#include <time.h>
#include <ctime>

// Генерация рандомных чисел
int get_random_number(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// Равномерно распределяем рандомное число в нашем диапазоне
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

// Функция создания двумерной матрицы
int** array_generator(unsigned int dim) 
{
	int** ptrary = new int* [dim];
	for (int i = 0; i < dim; i++) {
		ptrary[i] = new int[dim];
	}
	return ptrary;
}

// Функция удаления двумерной матрицы
void array_destroyer(int** ary, unsigned int dim1) 
{
	for (int i = 0; i < dim1; i++) {
		delete[] ary[i];
	}
	delete[] ary;
}


// Функция заполнения матрицы нулями
int** matrix_generate_empty(int** matrix, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrix[i][j] = 0;
		}
	}
	return matrix;
}

// Заполнение матрицы вручную
int** fill_matrix(int** matrix, int n) 
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << "Введите элемент с индексами " << i << " " << j << ": ";
			std::cin >> matrix[i][j];
		}
	}
	std::cout << "Матрица сгенерирована\n";
	return matrix;
}


// Функция вывода матрицы на экран
int** matrix_output(int** matrix, int n) 
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << matrix[i][j] << "\t";
		}
		std::cout << std::endl;
	}

	return matrix;
}

// Функция заполнения матрицы с помощью генератора случайных чисел
int** matrix_generate(int** matrix, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrix[i][j] = get_random_number(0, 200);
		}
	}
	std::cout << "Матрица сгенерирована\n";
	return matrix;
}

// Процедура последовательного умножения двух матриц
void sequential_matrix_multiplication(int** matrix_one, int** matrix_two, int n)
{
	int** matrix_result = array_generator(n);
	matrix_generate_empty(matrix_result, n);
	int start = clock();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				matrix_result[i][j] += matrix_one[i][k] * matrix_two[k][j];
			}
		}
	}
	int stop = clock();
	std::cout << "Матрица, полученная в результате последовательного умножения: \n";
	matrix_output(matrix_result, n);
	std::cout << "Время работы последовательного алгоритма: " << ((double)stop - (double)start) / CLOCKS_PER_SEC << " секунд" << std::endl;
	array_destroyer(matrix_result, n);
}

// Процедура параллельного умножения двух матриц
void parallel_matrix_multiplication(int** matrix_one, int** matrix_two, int n, int thread)
{
	int** matrix_result = array_generator(n);
	matrix_generate_empty(matrix_result, n);
	int start = clock();
	omp_set_num_threads(thread);
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				matrix_result[i][j] += matrix_one[i][k] * matrix_two[k][j];
			}
		}
	}
	int stop = clock();
	std::cout << "Матрица, полученная в результате параллельного умножения: \n";
	matrix_output(matrix_result, n);
	std::cout << "Время работы параллельного алгоритма: " << ((double)stop - (double)start) / CLOCKS_PER_SEC << " секунд" << std::endl;
	array_destroyer(matrix_result, n);
}

void menu()
{
	std::cout << "Меню программы:\n";
	std::cout << "1 - Ручное заполнение\n";
	std::cout << "2 - Заполнение случайными числами\n";
	std::cout << "0 - Завершение программы\n";
}

int main()
{
	// Настройка кодировки в консоли
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int command; // Выбор раздела меню
	int number_matrix; // Выбор размера матрицы
	int threads; // Выбор количества поток

	int** matrix_one = 0;
	int** matrix_two = 0;

	srand(time(NULL));

	menu();
	do 
	{
		std::cout << "\nВведите команду: ";
		std::cin >> command;
		switch (command)
		{
		case 1:
			
			do 
			{
				std::cout << "Введите размерность матрицы: ";
				std::cin >> number_matrix;
				
				if (number_matrix > 0)
				{
					break;
				}
				else 
				{
					std::cout << "Проверьте входные данные! Размерность должна быть положительна!\n";
				}
			} while (true);
			
			matrix_one = array_generator(number_matrix);
			matrix_two = array_generator(number_matrix);

			std::cout << "Заполнение первой матрицы" << std::endl;
			fill_matrix(matrix_one, number_matrix);
			std::cout << "Заполнение второй матрицы" << std::endl;
			fill_matrix(matrix_two, number_matrix);

			std::cout << "Первая матрица:\n";
			matrix_output(matrix_one, number_matrix);
			std::cout << "Вторая матрица:\n";
			matrix_output(matrix_two, number_matrix);

			do
			{
				std::cout << "Введите количество потоков:";
				std::cin >> threads;

				if (threads > 0) 
				{
					break;
				}
				else 
				{
					std::cout << "Проверьте входные данные! Размерность должна быть положительна!\n";
				}
			} while (true);

			// Последовательное умножение
			sequential_matrix_multiplication(matrix_one, matrix_two, number_matrix);

			// Распараллеливание умножения матриц
			parallel_matrix_multiplication(matrix_one, matrix_two, number_matrix, threads);

			array_destroyer(matrix_one, number_matrix);
			array_destroyer(matrix_two, number_matrix);

			break;

		case 2:
			do
			{
				std::cout << "Введите размерность матрицы: ";
				std::cin >> number_matrix;

				if (number_matrix > 0)
				{
					break;
				}
				else
				{
					std::cout << "Проверьте входные данные! Размерность должна быть положительна!\n";
				}
			} while (true);

			matrix_one = array_generator(number_matrix);
			matrix_two = array_generator(number_matrix);

			matrix_generate(matrix_one, number_matrix);
			matrix_generate(matrix_two, number_matrix);

			do
			{
				std::cout << "Введите количество потоков:";
				std::cin >> threads;

				if (threads > 0)
				{
					break;
				}
				else
				{
					std::cout << "Проверьте входные данные! Размерность должна быть положительна!\n";
				}
			} while (true);

			// Последовательное умножение
			sequential_matrix_multiplication(matrix_one, matrix_two, number_matrix);

			// Распараллеливание умножения матриц
			parallel_matrix_multiplication(matrix_one, matrix_two, number_matrix, threads);

			array_destroyer(matrix_one, number_matrix);
			array_destroyer(matrix_two, number_matrix);
			break;

		case 0:
			// Завершение программы
			std::cout << "Программа завершена";
			break;
		default:
			std::cout << "Неверно введена комманда\n";
			break;
		}
	} while (command);
}
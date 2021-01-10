#include <iostream>
#include <math.h>
#include <omp.h>
#include <Windows.h>

using namespace std;

double function(double x);

int main() {
    // Настройка кодировки в консоли
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    double intervalBegin;
    double intervalEnd;
    int count;
    double integral = 0;
    double step;

    cout << "Введите начало интервала: ";

    cin >> intervalBegin;

    cout << "Введите конец интервала: ";

    cin >> intervalEnd;

    cout << "Введите количество итераций: ";

    cin >> count;

    step = (intervalEnd - intervalBegin) / count;

#pragma omp parallel for

    for (int i = 1; i <= count; ++i) {
        integral += step * function(intervalBegin + (i - 1) * step);
    }

    cout << "Интеграл равен: " << integral << endl;

    int a;

    cin >> a;

    return 0;
}
double function(double x) {
    return 1 / (1 + exp(x));
}

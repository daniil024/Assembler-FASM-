#include <iostream>
#include <omp.h>


int N = omp_get_max_threads();

double f(double x) {
    return x * x + 1;
}

double get_A() {
    std::string a1;
    double a;
    std::cout << "To calculate integral of function - f(x) = (x^2 + 1) please input A - ";
    std::cin >> a1;
    try {
        a = std::stod(a1);
    } catch (std::exception) {
        std::cout << "Incorrect input, try again!\n";
        a = get_A();
    }
    return a;
}


double get_B(double a) {
    std::string b1;
    double b;
    std::cout << "Please input B - ";
    std::cin >> b1;
    try {
        b = std::stod(b1);
    } catch (std::exception) {
        std::cout << "Incorrect input, try again!\n";
        b = get_B(a);
    }
    if (b < a) {
        std::cout << "Incorrect input, B must be more than A. Try again!\n";
        b = get_B(a);
    }
    return b;
}

// number_thread == i
// n - кол-во прямоугольников
double meth_for_one_thread(double a, int number_thread, int n, double h) {
    double thread_sum = 0;
    for (int i = number_thread; i <= n; i += N) {
        thread_sum += f(a + i * h);
    }

    return thread_sum * h;
}

double get_result(double a, double b, double h) {
    double n = (b - a) / h;  // кол-во разбиений на прямоугольники
    double *thread_res = new double[N]{0.0};

#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        thread_res[i] = meth_for_one_thread(a, i, n, h);
    }

    double result = 0;
    for (int j = 0; j < N; ++j) {
        result += thread_res[j];
    }

    delete [] thread_res;
    return result;
}

int main() {
    double a = get_A();       // Нижний предел.
    double b = get_B(a);      // Верхний предел.
    double h = 0.0001;        // Переменная необходимая для разбиения графика на прямоугольники.

    std::cout << get_result(a, b, h);

    return 0;
}

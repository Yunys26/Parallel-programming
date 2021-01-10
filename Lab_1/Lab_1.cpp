#include <iostream>
#include <omp.h>
#include <windows.h>

int main()
{
    #pragma omp parallel    
    {
        std::cout << omp_get_max_threads() << std::endl;
    }
}
    
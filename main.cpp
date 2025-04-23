#include <iostream>
#include <chrono>
#include <optional>
#include <vector>
#include <thread>

#include "poly.h"

std::optional<double> poly_test(polynomial& p1,
                                polynomial& p2,
                                std::vector<std::pair<power, coeff>> solution)

{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    polynomial p3 = p1 * p2;

    auto p3_can_form = p3.canonical_form();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

   p3.print();

    if (p3_can_form != solution)
    {
        return std::nullopt;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

std::optional<double> poly_test_mult(polynomial& p1,
                                int value,
                                std::vector<std::pair<power, coeff>> solution)

{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    polynomial p3 =  value * p1;

    auto p3_can_form = p3.canonical_form();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

   p3.print();

    if (p3_can_form != solution)
    {
        return std::nullopt;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}


int main()
{
    /** We're doing (x+1)^2, so solution is x^2 + 2x + 1*/
    std::vector<std::pair<power, coeff>> solution = {{0,0}};

    /** This holds (x+1), which we'll pass to each polynomial */
    std::vector<std::pair<power, coeff>> poly_input = {{6,27},{5,27}, {4, 9}, {3,1}};
    std::vector<std::pair<power, coeff>> poly_input2 = {{2, 1}, {1,1}};

    polynomial p1(poly_input.begin(), poly_input.end());
    polynomial p2(poly_input2.begin(), poly_input2.end());
    int value = 1;
    
    std::vector<std::pair<power, coeff>> solution2 = {{1,1}};
    std::optional<double> result = poly_test(p1, p2, solution);
    // std::optional<double> result2 = poly_test_mult(p1, value, solution2);

    if (result.has_value())
    {
        std::cout << "Passed test, took " << result.value()/1000 << " seconds" << std::endl;
    } 
    else 
    {
        std::cout << "Failed test" << std::endl;
    }
}
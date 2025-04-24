// test_poly.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <chrono>
#include "poly.h"

using namespace std;
using namespace std::chrono;

// Read a polynomial from a file
vector<pair<size_t, int>> parsePolyFile(const string& filename) {
    ifstream file(filename);
    string line;
    vector<pair<size_t, int>> terms;

    while (getline(file, line)) {
        int coeff = 0;
        size_t power = 0;
        if (sscanf(line.c_str(), "%dx^%zu", &coeff, &power) == 2) {
            terms.emplace_back(power, coeff);
        }
    }

    return terms;
}

void writePolynomialToFile(const polynomial& poly, const string& filename) {
    ofstream out(filename);
    auto terms = poly.to_vector();

    // Sort by power ascending
    sort(terms.begin(), terms.end());

    for (const auto& [power, coeff] : terms) {
        if (coeff != 0) {
            out << coeff << "x^" << power << endl;
        }
    }
}

int main() {
        
    ifstream file("simple_poly.txt");
    string line;
    vector<pair<size_t, int>> terms1, terms2;
    bool readingFirst = true;

    // Parse the file into two term vectors
    while (getline(file, line)) {
        if (line == ";") {
            readingFirst = false;
            continue;
        }

        int coeff = 0;
        size_t power = 0;
        if (sscanf(line.c_str(), "%dx^%zu", &coeff, &power) == 2) {
            if (readingFirst) {
                terms1.emplace_back(power, coeff);
            } else {
                terms2.emplace_back(power, coeff);
            }
        }
    }

    // Construct the two polynomials
    polynomial p1(terms1.begin(), terms1.end());
    polynomial p2(terms2.begin(), terms2.end());

    // Multiply them

    auto start = high_resolution_clock::now();

    polynomial result = p1 * p2;

    auto end = high_resolution_clock::now();

    // Write result to file
    writePolynomialToFile(result, "output.txt");

    auto duration = duration_cast<milliseconds>(end - start).count();
    cout << "Polynomial multiplication took " << duration << " ms" << endl;
    return 0;
}

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

    auto result_con = result.canonical_form();

    auto end = high_resolution_clock::now();

    // Write result to file
    writePolynomialToFile(result, "output.txt");

    auto duration = duration_cast<milliseconds>(end - start).count();
    cout << "Polynomial multiplication took " << duration << " ms" << endl;
    return 0;
}
  // parallel implementation 2
    // polynomial result;

    // std::vector<std::unordered_map<int, int>> local_results;
    // std::vector<std::thread> threads;

    // unsigned int num_threads = std::thread::hardware_concurrency() / 16;
    // // printf("1/16 Number of threads: %u\n", num_threads);
    // // int num_threads = 8;
    // local_results.resize(num_threads);

    // size_t total_terms = this->terms.size();
    // size_t chunk_size = (total_terms + num_threads - 1) / num_threads;

    // auto it = this->terms.begin();

    // for (unsigned int t = 0; t < num_threads; ++t) {
    //     auto begin = it;

    //     size_t remaining = static_cast<size_t>(std::distance(it, this->terms.end()));
    //     size_t count = std::min(chunk_size, remaining);
        
    //     std::advance(it, count);
    //     auto end = it;

    //     threads.push_back(std::thread([=, &other, &local_results]() {
    //         for (auto i = begin; i != end; ++i) {
    //             int exp1 = i->first;
    //             int coeff1 = i->second;
    //             for (const auto& [exp2, coeff2] : other.terms) {
    //                 local_results[t][exp1 + exp2] += coeff1 * coeff2;
    //             }
    //         }
    //     }));
    // }
    
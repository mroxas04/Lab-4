#include <vector>
#include <utility>
#include <cstddef>
#include <map>
#include <iostream>
// #include <unordered_map>
#include <thread>
#include <mutex>
#include <optional>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <utility>
#include <cstddef>

#include "poly.h"

polynomial::polynomial() { //zero constructor
    terms[0] = 0;
}


polynomial polynomial::operator+(const polynomial &other) const{
    polynomial result = *this;
    for (auto i = other.terms.begin(); i != other.terms.end(); i++) {
        result.terms[i->first] += i->second;
    }
    result.removeZeroes();
    return result;

}

polynomial polynomial::operator+(int value) const {
    polynomial result = *this; //might need to fix, not sure how copy stuff works
    result.terms[0] += value;
    result.removeZeroes();
    return result;
}

polynomial operator+(int value, const polynomial &other) {
    return other + value;
}

polynomial polynomial::operator*(const polynomial &other)const {
    // polynomial result;

    //Sequential implementation
    // for (auto i = this->terms.begin(); i != this->terms.end(); i++) {
    //     for (auto j = other.terms.begin(); j != other.terms.end(); j++) {
    //         result.terms[i->first + j->first] += i->second * j->second;
    //     }
    // }
    //Parallel implementation 1

    // polynomial result;
    // std::mutex result_mutex;
    // int prod_val;

    //  int num_threads = 8;
    // // unsigned int num_threads = std::thread::hardware_concurrency();
    // //  printf("Number of threads: %u\n", num_threads);
    // std::vector<std::thread> threads;
    // std::vector<std::map<int, int>> local_results(num_threads);


    // size_t thread_size = (this->terms.size() + num_threads - 1) / num_threads;
    // std::vector<std::pair<int, int>> term_vector(this->terms.begin(), this->terms.end());

    // for (int t = 0; t < num_threads; ++t) {
    // //    threads.push_back(std::thread([&, t]() {
    //          threads.emplace_back([&, t]() {
    //         for (size_t i = t * thread_size; i < std::min((t + 1) * thread_size, term_vector.size()); ++i) {
    //             const auto& [exp1, coeff1] = term_vector[i];
    //             for (auto j = other.terms.begin(); j != other.terms.end(); j++) {
    //                 // prod_val = coeff1 * j->second;
    //                 // if (prod_val != 0) {
    //                 //     // std::lock_guard<std::mutex> lock(result_mutex);
    //                 //     // result.terms[exp1 + j->first] += prod_val;
    //                 //     local_results[t][exp1 + j->first] += prod_val;
    //                 // }
    //                 // std::lock_guard<std::mutex> lock(result_mutex);
    //                 local_results[t].try_emplace(exp1 + j->first, 0);
    //                 local_results[t][exp1 + j->first] += coeff1 * j->second;
    //                 // local_results[t][exp1 + j->first] += coeff1 * j->second;             //change
    //             }
    //         }
    //     // }));  
    //  }); 
    // }
  // parallel implementation 2
    polynomial result;

    std::vector<std::unordered_map<int, int>> local_results;
    std::vector<std::thread> threads;

    unsigned int num_threads = std::thread::hardware_concurrency();
    //  printf("1/16 Number of threads: %u\n", num_threads);
    // int num_threads = 6;
    local_results.resize(num_threads);

    size_t total_terms = this->terms.size();
    size_t chunk_size = (total_terms + num_threads - 1) / num_threads;

    auto iter = this->terms.begin();

    for (unsigned int t = 0; t < num_threads; ++t) {
        auto begin = iter;

        size_t remaining = static_cast<size_t>(std::distance(iter, this->terms.end()));
        size_t count = std::min(chunk_size, remaining);
        
        std::advance(iter, count);
        auto end = iter;

        threads.push_back(std::thread([=, &other, &local_results]() {
            for (auto i = begin; i != end; ++i) {
                int exp1 = i->first;
                int coeff1 = i->second;
                for (const auto& [exp2, coeff2] : other.terms) {
                    local_results[t][exp1 + exp2] += coeff1 * coeff2;
                }
            }
        }
        )
        );
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    // Combine results from all threads
    for (const auto& local : local_results) {
        for (const auto& [exp, coeff] : local) {
            result.terms[exp] += coeff;
        }
    }

    result.removeZeroes();
    // result.canonical_form();
    return result;

}

polynomial polynomial::operator*(int value) const {
    polynomial result;
    for (auto i = this->terms.begin(); i != this->terms.end(); i++) {
        result.terms[i->first] = i->second * value;
    }
    result.removeZeroes();
    return result;
}

polynomial operator*(int value, const polynomial &other) {
    return other * value;
}

polynomial polynomial::operator%(const polynomial &other) const {
    polynomial result(*this);
    while (result.find_degree_of() >= other.find_degree_of()) {
  //  for (int i = 0; i < 3; i++) {
        auto iter = result.terms.rbegin();
        auto iter2 = other.terms.rbegin();
        // std::cout << "iter Power: " << iter->first << " Coeff: " << iter->second << std::endl;
        // std::cout << "iter2 Power: " << iter2->first << " Coeff: " << iter2->second << std::endl;
        int power = iter->first - iter2->first;
        int coeff = iter->second / iter2->second;
       //   std::cout << "div Power: " << power << " Coeff: " << coeff << std::endl;
        polynomial temp;
        temp.terms[power] = coeff;
        temp = temp * other;
     //   std::cout << "PRE RESULT PRINT"<< std::endl;
     //   result.print();
    //    std::cout << "TEMP PRINT"<< std::endl;
  //      temp.print();
        result = result + (-1 * temp);
    //    std::cout << "RESULT PRINT"<< std::endl;
   //     result.print();
        
    }
    result.removeZeroes();
    return result;
}

void polynomial::removeZeroes() {
    auto i = terms.end();
    while (i != terms.begin()) {
         i--;
        // std::cout << "Power: " << i->first << " Coeff: " << i->second << std::endl;
        if (i->second == 0) {
            // std::cout << "Deleted Power: " << i->first << " Deleted Coeff: " << i->second << std::endl;
            i = terms.erase(i); 
        }
    }

    if (terms.empty()) {
        terms[0] = 0;
    }
}

polynomial::polynomial(const polynomial &other) {  
    this->terms = other.terms;
}

std::vector<std::pair<power, coeff>> polynomial::canonical_form() const {
    std::vector<std::pair<power, coeff>> form;
    for (auto i = terms.rbegin(); i != terms.rend(); i++) {
        int power = i->first;
        int coeff = i->second;
        std::pair<size_t, int> term = std::make_pair(power, coeff);
        // std::cout << "canon Power:" << term.first << " Coeff: " << term.second << std::endl;
        form.push_back(term);
    }

    // std::cout << "Vector elements: ";
    // for (const auto& element : form) {
    //     std::cout << "("<< element.first << ","<< element.second << "), ";
    // }
    // std::cout << std::endl;
    return form;
}

void polynomial::print () const {
    for (auto &[pow, coeff] : terms) {
        std::cout << coeff << "x^" << pow << std::endl;
    }
}

size_t polynomial::find_degree_of() const{
    return terms.rbegin()->first; //last entry in map
}

polynomial& polynomial::operator=(const polynomial &other) {
    if (this != &other) {
        this->terms = other.terms;
    }
    return *this; 
}


std::vector<std::pair<size_t, int>> polynomial::to_vector() const {
    std::vector<std::pair<size_t, int>> result;
    for (const auto& [power, coeff] : terms) {
        if (coeff != 0)
            result.emplace_back(power, coeff);
    }
    return result;
}
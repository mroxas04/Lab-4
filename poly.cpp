#include <vector>
#include <utility>
#include <cstddef>
#include <map>
#include <iostream>

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
    polynomial result;
    for (auto i = this->terms.begin(); i != this->terms.end(); i++) {
        for (auto j = other.terms.begin(); j != other.terms.end(); j++) {
            result.terms[i->first + j->first] += i->second * j->second;
        }
    }

    result.removeZeroes();
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

polynomial polynomial::operator%(const polynomial &other) {
    polynomial result = *this;
    while (result.find_degree_of() >= other.find_degree_of()) {
  //  for (int i = 0; i < 3; i++) {
        auto it = result.terms.rbegin();
        auto it2 = other.terms.rbegin();
        // std::cout << "it Power: " << it->first << " Coeff: " << it->second << std::endl;
        // std::cout << "it2 Power: " << it2->first << " Coeff: " << it2->second << std::endl;
        int power = it->first - it2->first;
        int coeff = it->second / it2->second;
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
#include <vector>
#include <utility>
#include <cstddef>

#include "poly.h"

polynomial::polynomial() { //zero constructor
    terms[0] = 0;
}

template <typename Iter>
polynomial::polynomial(Iter begin, Iter end) { //first - power, second - coefficient
    for (auto i = begin; i != end; i++) {
        if (i->second != 0) {
            terms[i->first] += i->second
        }
    }
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
    
}

void polynomial::removeZeroes() {
    auto i = terms.begin();
    while (i != terms.end()) {
        if(i->second == 0) { //if coefficient is 0
            i = terms.erase(i);
        } else {
            i++;
        }

    }
}
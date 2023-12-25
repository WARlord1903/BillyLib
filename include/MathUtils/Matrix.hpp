#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "main.h"
#include <cstddef>
#include <ostream>
#include <vector>

class Matrix;

Matrix operator*(Matrix, double);
Matrix operator*(Matrix, Matrix);
std::ostream& operator<<(std::ostream&, const Matrix&);

class Matrix{
public:
    Matrix(std::initializer_list<std::initializer_list<double>> ielems): elems{new std::vector<std::vector<double>>} { for(auto v : ielems) elems->emplace_back(v.begin(), v.end()); 
                                                                                                                          //ensure each row is of equal size
                                                                                                                          int largest = 0;
                                                                                                                          for(auto v : *elems) if(v.size() > largest) largest = v.size();
                                                                                                                          for(auto v : *elems) while(v.size() < largest) v.push_back(0);
                                                                                                                        }
    Matrix(const Matrix& m): elems{new std::vector<std::vector<double>>(m.elems->begin(), m.elems->end())} {}
    void operator=(const Matrix& m) { elems.reset(new std::vector<std::vector<double>>(m.elems->begin(), m.elems->end())); }
    std::vector<double>& operator[](unsigned x) { return (*elems)[x]; }
    const std::vector<double>& operator[](unsigned x) const { return (*elems)[x]; }
    std::size_t size() const { return elems->size(); }
    Matrix& operator*=(double);
    Matrix& operator*=(Matrix&);
private: 
    std::shared_ptr<std::vector<std::vector<double>>> elems;
};


#endif
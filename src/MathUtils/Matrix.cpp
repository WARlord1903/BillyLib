#include "MathUtils/Matrix.hpp"

Matrix operator*(Matrix m, double d) { return m *= d; }
Matrix operator*(Matrix lhs, Matrix rhs) { return lhs *= rhs; }

std::ostream& operator<<(std::ostream& os, const Matrix& m){
    os << "{\n";
    for(int i = 0; i < m.size(); i++){
        os << "\t{";
        for(int j = 0; j < m[i].size() - 1; j++){
            os << m[i][j] << ", ";
        }
        os << m[i].back() << "}\n";
    } 
    os << "}";
    return os;
}

Matrix& Matrix::operator*=(double d){
    for(auto v : *elems)
        for(auto e : v)
            e *= d;
    return *this;
}

Matrix& Matrix::operator*=(Matrix& m){
    Matrix x{};
    if(m.elems->size() != (*elems)[0].size()){
        std::cerr << "ERROR: Number of left-hand columns does not equal number of right-hand rows.\n";
        return *this;
    }
    std::vector<std::vector<double>> new_elems;
    for(int i = 0; i < elems->size(); i++){
        x.elems->push_back({});
        for(int j = 0; j < (*(m.elems))[0].size(); j++){
            double sum = 0;
            for(int k = 0; k < m.elems->size(); k++){
                sum += (*elems)[i][k] * (*(m.elems))[k][j];
            }
            (*(x.elems))[i].push_back(sum);
        }
    }
    *this = x;
    return *this;
}
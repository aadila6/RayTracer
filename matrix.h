/* Mediocre Matrix Math - matrix.h
 *
 * Copyright 2019, Zander Adams
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef MMMAT_H
#define MMMAT_H

#include <cmath>
#include <initializer_list>
#include "vector.h"

namespace mmm
{

/* class Matrix -- defines a 2D matrix with M rows and N columns of T objects
 *
 * Columns are defined in row-major order
 */
template <int M, int N, typename T>
class matrix
{
private:
    T components[M*N];
public:
    matrix();
    matrix(std::initializer_list<T> entries);
    
    // Accessors
    T& index(const int i, const int j);
    const T& index(const int i, const int j) const;

    // Basic arithmetic operations
    matrix<M,N,T> operator*(const T scalar) const;
    matrix<M,N,T>& operator*=(const T scalar);
    matrix<M,N,T> operator+(const matrix<M,N,T> m) const;

    // Matrix operators
    matrix<M+1,N+1,T> homogeneous() const;
    matrix<M,N,T> transpose() const;
    matrix<M-1,N-1,T> getCofactor(const int i, const int j) const;

    // Static constructor for an MxN
    static constexpr matrix<M,N,T> identity();
};

/* ####### CLASS MATRIX ####### */
template <int M, int N, typename T>
matrix<M,N,T>::matrix()
{
    for (int i = 0; i < M*N; i++)
    {
        components[i] = T();
    }
}

template <int M, int N, typename T>
matrix<M,N,T>::matrix(std::initializer_list<T> entries)
{
    auto itr = entries.begin();
    for (int i = 0; i < M*N; i++)
    {
        components[i] = *itr;
        itr++;
    }
}

template <int M, int N, typename T>
T& matrix<M,N,T>::index(const int i, const int j)
{
    return components[i*N + j];
}

template <int M, int N, typename T>
const T& matrix<M,N,T>::index(const int i, const int j) const
{
    return components[i*N + j];
}

template <int M, int N, typename T>
matrix<M,N,T> matrix<M,N,T>::operator*(const T scalar) const
{
    matrix<M,N,T> mat;
    for (int i = 0; i < M*N; i++)
    {
        mat.components[i] = (*this).components[i] * scalar;
    }
    return mat;
}

template <int M, int N, typename T>
matrix<M,N,T>& matrix<M,N,T>::operator*=(const T scalar)
{
    for (int i = 0; i < M*N; i++)
    {
        (*this).components[i] *= scalar;
    }
    return (*this);
}

template <int M, int N, typename T>
matrix<M,N,T> matrix<M,N,T>::operator+(const matrix<M,N,T> m) const
{
    matrix<M,N,T> mat;
    for (int i = 0; i < M*N; i++)
    {
        mat.components[i] = this->components[i] + m.components[i];
    }
    return mat;
}

template <int M, int N, typename T>
matrix<M+1,N+1,T> matrix<M,N,T>::homogeneous() const
{
    matrix<M+1,N+1,T> mat;
    for (int i = 0; i < M + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            if (i < M && j < N)
                mat.index(i, j) = this->index(i, j);
            else if (i == M && j == N)
                mat.index(i, j) = (T)1;
            else
                mat.index(i, j) = T();
        }
    }
    return mat;
}

template <int M, int N, typename T>
matrix<M,N,T> matrix<M,N,T>::transpose() const
{
    matrix<M,N,T> mat;
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            mat.index(i, j) = this->index(j, i);
        }
    }
    return mat;
}

template <int M, int N, typename T>
matrix<M-1,N-1,T> matrix<M,N,T>::getCofactor(const int I, const int J) const
{
    static_assert(M > 1 && N > 1);

    matrix<M-1,N-1,T> mat;
    int v = 0;
    for (int i = 0; i < M; i++)
    {
        if (i == I)
            continue;
        int u = 0;
        for (int j = 0; j < N; j++)
        {
            if (j == J)
                continue;
            mat.index(v, u) = this->index(i, j);
            u++;
        }
        v++;
    }
    return mat;
}

template <int M, int N, typename T>
constexpr matrix<M,N,T> matrix<M,N,T>::identity()
{
    static_assert(M == N, "Identity matrices must be square");
    matrix<M,N,T> mat;
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            mat.index(i, j) = (T)(i == j);
        }
    }
    return mat;
}

/* END CLASS MATRIX */

template <int M, int N, typename T>
T determinant(const matrix<M,N,T> &m)
{
    static_assert(N==M, "The determinant is only defined for square matrices");
    T det = T();
    if (M > 2 && N > 2)
    {
        for (int j = 0; j < N; j++)
        {
            auto C = m.getCofactor(0, j);
            int sgn = (j%2) ? -1 : 1;
            det += m.index(0, j) * sgn * determinant(C);
        }
    }
    return det;
}

template <typename T>
matrix<2,2,T> inverse(const matrix<2,2,T> &m)
{
    matrix<2,2,T> inv{
        m.index(1,1), -m.index(0,1),
        -m.index(1,0), m.index(0,0)};
    return inv * (1/determinant(m));
}

template <int M, int N, typename T>
matrix<M,N,T> inverse(const matrix<M,N,T> &m)
{
    static_assert(M==N, "Inverse is only defined for square matrices");
    matrix<M,N,T> inv;
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int sgn = ((i+j)%2) ? -1 : 1;
            inv.index(i,j) = sgn * determinant(m.getCofactor(i, j));
        }
    }
    return inv.transpose() * (1/determinant(m));
}

template <typename T>
float determinant(const matrix<2,2,T> &m)
{
    return m.index(0,0)*m.index(1,1) - m.index(0,1)*m.index(1,0);
}

template <int M, int N, int P, typename T>
matrix<N,P,T> operator*(const matrix<N,M,T> &m1, const matrix<M,P,T> &m2)
{
    matrix<N,P,T> mat;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < P; j++)
        {
            T dot = T();
            for (int k = 0; k < M; k++)
            {
                dot += m1.index(i, k) * m2.index(k, j);
            }
            mat.index(i, j) = dot;
        }
    }
    return mat;
}

template <int M, int N, typename T>
vector<M,T> operator*(const matrix<M,N,T> &m, const vector<N,T> &u)
{
    vector<M,T> v;
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            v[i] += m.index(i, j) * u[j];
        }
    }
    return v;
}

} // end namespace mmm

#endif

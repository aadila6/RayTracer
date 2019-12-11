/* Mediocre Matrix Math - vector.h
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

#ifndef ZVEC_H
#define ZVEC_H

#include <cmath>
#include <iostream>

namespace mmm
{

template <int N, typename T>
class vector
{
private:
    T components[N];
public:
    vector();
    vector(T x, T y);
    vector(T x, T y, T z);

    T& x();
    T& y();
    T& z();
    T& u() { return x(); }
    T& v() { return y(); }
    T& w() { return w(); }
    const T& x() const;
    const T& y() const;
    const T& z() const;
    const T& u() const { return x(); }
    const T& v() const { return y(); }
    const T& w() const { return z(); }
    T& operator[](int i);
    const T& operator[](int i) const;
    T* dataPtr() { return components; }
    vector<N,T> operator+(const vector<N, T> &v) const;
    vector<N,T> operator-(const vector<N, T> &v) const;
    vector<N,T>& operator+=(const vector<N, T> &v);
    vector<N,T>& operator-=(const vector<N, T> &v);
    vector<N,T> operator*(const T scalar) const;
    vector<N,T> operator/(const T scalar) const;
    vector<N,T>& operator*=(const T scalar);
    vector<N,T>& operator/=(const T scalar);
    T dot(const vector<N, T> &v) const;
    vector<N,T> cross(const vector<N, T> &v) const;
    vector<N+1,T> homogeneous() const;
    float norm() const;
    vector<N,T> normalized() const;

    friend std::ostream& operator<<(std::ostream &os, const vector &v)
    {
        os << "{";
        for (int i = 0; i < N; i++)
        {
            os << v[i];
            if (i < N-1)
                os << ", ";
        }
        os << "}";
        return os;
    }
};

template <int N, typename T>
vector<N,T>::vector()
{
    for (int i = 0; i < N; i++)
    {
        components[i] = T();
    }
}

template <int N, typename T>
vector<N,T>::vector(T x, T y)
{
    static_assert(N >= 2, "Constructor requires N >= 2 components");
    components[0] = x;
    components[1] = y;
}

template <int N, typename T>
vector<N,T>::vector(T x, T y, T z)
{
    static_assert(N >= 3, "Constructor requires N >= 3 components");
    components[0] = x;
    components[1] = y;
    components[2] = z;
}

template <int N, typename T>
T& vector<N,T>::x()
{
    return components[0];
}

template <int N, typename T>
T& vector<N,T>::y()
{
    return components[1];
}

template <int N, typename T>
T& vector<N,T>::z()
{
    return components[2];
}

template <int N, typename T>
const T& vector<N,T>::x() const
{
    return components[0];
}

template <int N, typename T>
const T& vector<N,T>::y() const
{
    return components[1];
}

template <int N, typename T>
const T& vector<N,T>::z() const
{
    return components[2];
}

template <int N, typename T>
T& vector<N,T>::operator[](const int i)
{
    if (i >= N)
        throw 1;
    return components[i];
}

template <int N, typename T>
const T& vector<N,T>::operator[](const int i) const
{
    if (i >= N)
        throw 1;
    return components[i];
}

template <int N, typename T>
vector<N, T> vector<N,T>::operator+(const vector<N, T> &v) const
{
    vector<N,T> vec;
    for (int i = 0; i < N; i++)
    {
        vec[i] = (*this)[i] + v[i];
    }
    return vec;
}

template <int N, typename T>
vector<N, T> vector<N,T>::operator-(const vector<N, T> &v) const
{
    vector<N,T> vec;
    for (int i = 0; i < N; i++)
    {
        vec[i] = (*this)[i] - v[i];
    }
    return vec;
}

template <int N, typename T>
vector<N, T>& vector<N,T>::operator+=(const vector<N, T> &v)
{
    for (int i = 0; i < N; i++)
    {
        components[i] += v[i];
    }
    return (*this);
}

template <int N, typename T>
vector<N, T>& vector<N,T>::operator-=(const vector<N, T> &v)
{
    for (int i = 0; i < N; i++)
    {
        components[i] -= v[i];
    }
    return (*this);
}

template <int N, typename T>
vector<N, T> vector<N,T>::operator*(const T scalar) const
{
    vector<N,T> vec;
    for (int i = 0; i < N; i++)
    {
        vec[i] = (*this)[i] * scalar;
    }
    return vec;
}

template <int N, typename T>
vector<N,T> operator*(const T scalar, const vector<N,T> &v)
{
    return v * scalar;
}

template <int N, typename T>
vector<N, T> vector<N,T>::operator/(const T scalar) const
{
    vector<N,T> vec;
    for (int i = 0; i < N; i++)
    {
        vec[i] = (*this)[i] / scalar;
    }
    return vec;
}

template <int N, typename T>
vector<N, T>& vector<N,T>::operator*=(const T scalar)
{
    for (int i = 0; i < N; i++)
    {
        components[i] *= scalar;
    }
    return (*this);
}

template <int N, typename T>
vector<N, T>& vector<N,T>::operator/=(const T scalar)
{
    for (int i = 0; i < N; i++)
    {
        components[i] /= scalar;
    }
    return (*this);
}

template <int N, typename T>
T vector<N,T>::dot(const vector<N,T> &v) const
{
    T dot = T();
    for (int i = 0; i < N; i++)
    {
        dot += (*this)[i] * v[i];
    }
    return dot;
}

template <int N, typename T>
vector<N,T> vector<N,T>::cross(const vector<N,T> &v) const
{
    static_assert(N == 3, "The cross product is only defined for 3-vectors");
    const vector<N,T> &u = (*this);
    vector<N,T> vec;
    vec.x() = u.y() * v.z() - u.z() * v.y();
    vec.y() = -(u.x() * v.z() - u.z() * v.x());
    vec.z() = u.x() * v.y() - u.y() * v.x();
    return vec;
}
template <int N, typename T>
vector<N+1,T> vector<N,T>::homogeneous() const
{
    vector<N+1,T> vec;
    int i;
    for (i = 0; i < N; i++)
    {
        vec[i] = (*this)[i];
    }
    vec[i] = (T)1;
    return vec;
}

template <int N, typename T>
float vector<N,T>::norm() const
{
    T componentsSquared = T();
    for (int i = 0; i < N; i++)
    {
        componentsSquared += (*this)[i] * (*this)[i];
    }
    return sqrt(componentsSquared);
}

template <int N, typename T>
vector<N,T> vector<N,T>::normalized() const
{
    return (*this) / norm();
}

} // end namespace mmm
#endif


#include <array>
#include <initializer_list>

#pragma once

using Dimension_t = unsigned char;

template <typename Scalar_t, Dimension_t Dim>
class Vector
{
public:
    using ScalarType = Scalar_t;
    using IndexType = Dimension_t;
    static constexpr Dimension_t Dimension = Dim;

    Vector() {}
    Vector(const Vector& iOther) = default;
    Vector(Vector&& iOther) = default;
    Vector(const std::array<Scalar_t, Dim>& iStorage) : m_Coords(iStorage) {}

    Vector& operator-=(const Vector& iOther) {
        for(Dimension_t index=0; index<Dim; index++)
            (*this)[index] -= iOther[index];
        return *this;
    }
    
    Vector& operator+=(const Vector& iOther) {
        for(Dimension_t index=0; index<Dim; index++)
            (*this)[index] += iOther[index];
        return *this;
    }

    Vector& operator*=(Scalar_t iScale)
    {
        for(Dimension_t index=0; index<Dim; index++)
            (*this)[index] *= iScale;
        return *this;
    }

    Vector operator-(const Vector& iOther) const {
        Vector result(*this);
        return result -= iOther;
    }
    
    Vector operator+(const Vector& iOther) const {
        Vector result(*this);
        return result += iOther;
    }

    Vector operator*(Scalar_t iScale) const
    {
        Vector result(*this);
        return result *= iScale;
    }
    
    Scalar_t operator*(const Vector& iOther) const
    {
        Scalar_t dotProduct{0.};
        for(Dimension_t index=0; index<Dim; index++)
            dotProduct += (*this)[index] * iOther[index];
        return dotProduct;
    }

    Scalar_t& operator[](Dimension_t iIndex) {
        return m_Coords[iIndex];
    }

    Scalar_t operator[](Dimension_t iIndex) const {
        return m_Coords[iIndex];
    }

protected:
    std::array<Scalar_t, Dim> m_Coords;
};


template <typename Scalar_t, Dimension_t Dim>
class Point : private Vector<Scalar_t, Dim>
{
private:
    using Base = Vector<Scalar_t, Dim>;
    Point(const Base& iOther) : Base(iOther) {}
    Point(Base&& iOther) : Base(iOther) {}

public:
    using ScalarType = Base::ScalarType;
    using IndexType = Base::IndexType;
    static constexpr Dimension_t Dimension = Base::Dimension;

    Point() {}
    Point(const Point& iOther) : Base(iOther) {}
    Point(Point&& iOther) : Base(std::move(iOther)) {}
    Point(const std::array<Scalar_t, Dim>& iStorage) : Base(iStorage) {}

    Point& operator-=(const Base& iOther) {
        return Base::operator-=(iOther);
    }

    Point& operator+=(const Base& iOther) {
        return Base::operator+=(iOther);
    }

    Point& operator*=(Scalar_t iScale) {
        return Base::operator*=(iScale);
    }

    Point operator-(const Base& iOther) const {
        return Base::operator-(iOther);
    }
    
    Base operator-(const Point& iOther) const {
        return Base::operator-(iOther);
    }
    
    Point operator+(const Base& iOther) const {
        return Base::operator+(iOther);
    }

    Point operator*(Scalar_t iScale) const {
        return Base::operator*(iScale);
    }

    Scalar_t& operator[](Dimension_t iIndex) {
        return Base::operator[](iIndex);
    }

    Scalar_t operator[](Dimension_t iIndex) const {
        return Base::operator[](iIndex);
    }
};

using Vector3D = Vector<double, 3>;
using Point3D = Point<double, 3>;
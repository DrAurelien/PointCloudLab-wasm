#include <array>
#include <initializer_list>

#pragma once

using Dimension_t = unsigned char;

template <typename Scalar, Dimension_t Dim>
class Vector : public std::array<Scalar, Dim>
{
private:
    using Base = std::array<Scalar, Dim>;

public:
    Vector() {}
    Vector(const Vector& iOther) : Base(iOther) {}
    Vector(Vector&& iOther) : Base(std::move(iOther)) {}
    Vector(const std::array<Scalar, Dim>& iStorage) : Base(iStorage) {}

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

    Vector& operator*=(Scalar iScale)
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

    Vector operator*(Scalar iScale) const
    {
        Vector result(*this);
        return result *= iScale;
    }
    
    Scalar operator*(const Vector& iOther) const
    {
        Scalar dotProduct{0.};
        for(Dimension_t index=0; index<Dim; index++)
            dotProduct += (*this)[index] * iOther[index];
        return dotProduct;
    }
};


template <typename Scalar, Dimension_t Dim>
class Point : private Vector<Scalar, Dim>
{
private:
    using Base = Vector<Scalar, Dim>;
    Point(const Base& iOther) : Base(iOther) {}
    Point(Base&& iOther) : Base(iOther) {}

public:
    Point() {}
    Point(const Point& iOther) : Base(iOther) {}
    Point(Point&& iOther) : Base(std::move(iOther)) {}
    Point(const std::array<Scalar, Dim>& iStorage) : Base(iStorage) {}

    Point& operator-=(const Base& iOther) {
        return Base::opertor-=(iOther);
    }

    Point& operator+=(const Base& iOther) {
        return Base::opertor+=(iOther);
    }

    Point& operator*=(Scalar iScale) {
        return Base::opertor*=(iScale);
    }

    Point operator-(const Base& iOther) const {
        return Base::opertor-(iOther);
    }
    
    Base operator-(const Point& iOther) const {
        return Base::opertor-(iOther);
    }
    
    Point operator+(const Base& iOther) const {
        return Base::opertor+(iOther);
    }

    Point operator*(Scalar iScale) const {
        return Base::opertor*(iScale);
    }
};

using Vector3D = Vector<double, 3>;
using Point3D = Point<double, 3>;
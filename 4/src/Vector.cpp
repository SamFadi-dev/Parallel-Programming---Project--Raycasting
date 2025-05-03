#include <Vector.h>
#include <math.h>

template <typename T>
Vector<T>::Vector(T x, T y) : _x(x), _y(y)
{
}

template <typename T>
Vector<T>::Vector(const Vector<T> &v) : _x(v.x()), _y(v.y())
{
}

template <typename T>
T Vector<T>::x() const
{
    return _x;
}

template <typename T>
T Vector<T>::y() const
{
    return _y;
}

template <typename T>
Vector<T> &Vector<T>::operator+=(const Vector<T> &v)
{
    _x += v.x();
    _y += v.y();
    return *this;
}

template <typename T>
Vector<T> &Vector<T>::operator=(const Vector<T> &v)
{
    _x = v.x();
    _y = v.y();
    return *this;
}

template <typename T>
void Vector<T>::rotate(double angle)
{
    double oldX = _x;
    _x = _x * cos(angle) - _y * sin(angle);
    _y = oldX * sin(angle) + _y * cos(angle);
}

template class Vector<int>;
template class Vector<double>;
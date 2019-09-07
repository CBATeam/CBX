/**
 * @author    Dedmen
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Vector of length 3 operations. Originally developed for the ACE3 and ACRE2 projects.
 */

#pragma once

#include "types.hpp"

#include <iostream>
#include <vector>
#include <cmath>

#ifndef _WIN32
#define sinf(x) sin(x)
#define cosf(x) cos(x)
#define acosf(x) acos(x)
#endif

namespace cbx {
    template <typename T> T acos(T n) { return -1; }
    template <typename T> T cos(T n) { return -1; }
    template <typename T> T sin(T n) { return -1; }

    template<typename T>
    class Vector3 {
    public:
        constexpr Vector3() :
            _x(0),
            _y(0),
            _z(0) {
        }
        constexpr Vector3(const T x_, const T y_, const T z_) noexcept :
            _x(x_),
            _y(y_),
            _z(z_) {
        }
        Vector3(std::istream & read_) {
            // Constructor to read from a stream
            read_.read((char *)&_x, sizeof(T));
            read_.read((char *)&_y, sizeof(T));
            read_.read((char *)&_z, sizeof(T));
        }
        Vector3(const float32_t *buffer) {
            _x = buffer[0];
            _y = buffer[1];
            _z = buffer[2];
        }

        constexpr Vector3& operator= (const Vector3& other) noexcept { _x = other.x(); _y = other.y(); _z = other.z(); return *this; }
        constexpr Vector3 operator * (const T& val) const { return Vector3(_x * val, _y * val, _z * val); }
        constexpr Vector3 operator / (const T& val) const { T invVal = T(1) / val; return Vector3(_x * invVal, _y * invVal, _z * invVal); }
        constexpr Vector3 operator + (const Vector3& v) const { return Vector3(_x + v.x(), _y + v.y(), _z + v.z()); }
        constexpr Vector3 operator / (const Vector3& v) const { return Vector3(_x / v.x(), _y / v.y(), _z / v.z()); }
        constexpr Vector3 operator * (const Vector3& v) const { return Vector3(_x * v.x(), _y * v.y(), _z * v.z()); }
        constexpr Vector3 operator - (const Vector3& v) const { return Vector3(_x - v.x(), _y - v.y(), _z - v.z()); }
        constexpr Vector3 operator - () const { return Vector3(-_x, -_y, -_z); }

        constexpr Vector3& operator *=(const Vector3& v) noexcept { _x *= v._x; _y *= v._y; _z *= v._z; return *this; }
        constexpr Vector3& operator *=(T mag) noexcept { _x *= mag; _y *= mag; _z *= mag; return *this; }
        constexpr Vector3& operator /=(const Vector3& v) noexcept { _x /= v._x; _y /= v._y; _z /= v._z; return *this; }
        constexpr Vector3& operator /=(T mag) noexcept { _x /= mag; _y /= mag; _y /= mag; return *this; }
        constexpr Vector3& operator +=(const Vector3& v) noexcept { _x += v._x; _y += v._y; _z += v._z; return *this; }
        constexpr Vector3& operator -=(const Vector3& v) noexcept { _x -= v._x; _y -= v._y; _z -= v._z; return *this; }


        bool operator == (const Vector3 &r) const noexcept { return (_x == r.x() && _y == r.y() && _z == r.z()); }

        constexpr T magnitude() const noexcept { return sqrt(_x * _x + _y * _y + _z * _z); }
        constexpr T magnitude_squared() const noexcept { return _x * _x + _y * _y + _z * _z; }
        constexpr T dot(const Vector3& v) const noexcept { return (_x * v.x() + _y * v.y() + _z * v.z()); }
        constexpr T distance(const Vector3& v) const noexcept { Vector3 dist = (*this - v); dist = dist * dist; return sqrt(dist.x() + dist.y() + dist.z()); }
        constexpr Vector3 cross(const Vector3& v) const noexcept { return Vector3(_y * v.z() - _z * v.y(), _z * v.x() - _x * v.z(), _x * v.y() - _y * v.x()); }
        constexpr Vector3 normalize() const noexcept { return (*this / abs(magnitude())); };
        constexpr bool zero_distance() const noexcept { return ((_x == 0.0f && _y == 0.0f && _z == 0.0f) ? true : false ); }

        static T clamp(T x, T a, T b) { return x < a ? a : (x > b ? b : x); }

        static Vector3 lerp(const Vector3& A, const Vector3& B, const T t) noexcept { return A*t + B*(1.f - t); }
        constexpr Vector3 lerp(const Vector3& B, const T t) const noexcept { return Vector3::lerp(*this, B, t);  }

        static Vector3 slerp(Vector3 start, Vector3 end, T percent) noexcept {
            T dot = start.dot(end);
            dot = Vector3::clamp(dot, -1.0f, 1.0f);

            T theta = acos(dot) * percent;
            Vector3 relative = end - start*dot;
            relative.normalize();
            return ((start * cos(theta)) + (relative*sin(theta)));
        }
        constexpr Vector3 slerp(const Vector3& B, const T p) const noexcept {
            return Vector3::slerp(*this, B, p);
        }

        const T& x() const noexcept { return _x; }
        const T& y() const noexcept { return _y; }
        const T& z() const noexcept { return _z; }

        void x(const T val) { _x = val; }
        void y(const T val) { _y = val; }
        void z(const T val) { _z = val; }
    protected:
        T _x;
        T _y;
        T _z;
    };

    template<typename T, uint32_t N = 3>
    class vector {
    public:
        vector() :
            _values(std::vector<T>(N)) {
        }
        std::vector<T> _values;
    };


    template<typename T>
    class spatial {
    public:
        spatial() :
            position(Vector3<T>()),
            orientation(Vector3<T>())
        {
        }
        explicit spatial(const Vector3<T> & position_, const Vector3<T> & orientation_) :
            position(position_),
            orientation(orientation_)
        {}
        spatial<T> & operator= (const spatial<T> & other) { position = other.position; orientation = other.orientation; return *this; }
        Vector3<T> position;
        Vector3<T> orientation;
    };


    template<typename T>
    class pair {
    public:
        pair() :
            _x(0),
            _y(0) {
        }
        pair(const T x_, const T y_, const T z_) :
            _x(x_),
            _y(y_) {
        }
        pair(const float32_t *buffer) {
            _x = buffer[0];
            _y = buffer[1];
        }
        pair(std::istream & read_) {
            // Constructor to read from a stream
            read_.read((char *)&_x, sizeof(T));
            read_.read((char *)&_y, sizeof(T));
        }

        pair<T> & operator= (const pair<T>& other) { _x = other.x(); _y = other.y();  return *this; }

        const T & x() const { return _x; }
        const T & y() const { return _y; }

        void x(const T val) { _x = val; }
        void y(const T val) { _y = val; }
    protected:
        T _x;
        T _y;
    };

    using vec3_fp32_t = Vector3<float32_t>;
    using vec3_fp64_t = Vector3<float64_t>;
}

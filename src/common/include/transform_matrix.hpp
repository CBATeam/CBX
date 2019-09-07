/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Transformation matrix. Originally developed for the ACE3 and ACRE2 projects.
 */

#pragma once

#include "types.hpp"
#include "vector.hpp"

namespace cbx {
    template <typename T>
    class transform_matrix_base {
    public:
        transform_matrix_base & operator= (const transform_matrix_base& other) { _x = other.x(); _y = other.y(); _z = other.z(); _n = other.n();  return *this; }

        transform_matrix_base() {}
        transform_matrix_base(std::istream &stream_, uint32_t version_ = 73) : _x(stream_), _y(stream_), _z(stream_), _n(stream_) { (void)version_;}
        transform_matrix_base(cbx::Vector3<T> x_, cbx::Vector3<T> y_, cbx::Vector3<T> z_, cbx::Vector3<T> n_) : _x(x_), _y(y_), _z(z_), _n(n_) { }

        const cbx::Vector3<T> & x() const { return _x; }
        const cbx::Vector3<T> & y() const { return _y; }
        const cbx::Vector3<T> & z() const { return _z; }
        const cbx::Vector3<T> & n() const { return _n; }

        void x(const cbx::Vector3<T> val) { _x = val; }
        void y(const cbx::Vector3<T> val) { _y = val; }
        void z(const cbx::Vector3<T> val) { _z = val; }
        void n(const cbx::Vector3<T> val) { _n = val; }

    protected:
        cbx::Vector3<T> _x;
        cbx::Vector3<T> _y;
        cbx::Vector3<T> _z;
        cbx::Vector3<T> _n;
    };

    using transform_matrix_fp32_t = transform_matrix_base<float32_t>;
}

// MIT License
//
// Copyright (c) 2019 Jelle Spijker
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef LIBFLUIDS_FUNCTOR_H
#define LIBFLUIDS_FUNCTOR_H

#include <memory>

#include <Eigen/Eigen>
#include <unsupported/Eigen/NumericalDiff>

#include "../include/fluids/System.h"

namespace Fluids {
/// Functor used in the optimization
/// \tparam _Scalar Scalar type
/// \tparam NX Inputs vector
/// \tparam NY Outputs vector
template<typename _Scalar, int NX = Eigen::Dynamic, int NY = Eigen::Dynamic>
struct Functor {
  typedef _Scalar Scalar; //! Scalar type
  enum {
    InputsAtCompileTime = NX,
    ValuesAtCompileTime = NY
  };
  typedef Eigen::Matrix<Scalar, InputsAtCompileTime, 1> InputType;
  typedef Eigen::Matrix<Scalar, ValuesAtCompileTime, 1> ValueType;
  typedef Eigen::Matrix<Scalar, ValuesAtCompileTime, InputsAtCompileTime> JacobianType;

  const int m_inputs, m_values;

  Functor() : m_inputs(InputsAtCompileTime), m_values(ValuesAtCompileTime) {}
  Functor(int inputs, int values) : m_inputs(inputs), m_values(values) {}

  int inputs() const { return m_inputs; }
  int values() const { return m_values; }
};

struct System_Functor_Base : Functor<double> {
public:
  System_Functor_Base() : m_system(std::make_shared<System>()) {};
  explicit System_Functor_Base(const std::shared_ptr<System> &m_system) : m_system(m_system) {}

  const std::shared_ptr<System> &Get_System() const {
    return m_system;
  }
  void Set_System(const std::shared_ptr<System> &system) {
    System_Functor_Base::m_system = system;
  }

  int operator()(const Eigen::VectorXd &x, Eigen::VectorXd &dvec) const {

    // Set unknown values from x-vector
    auto speed = m_system->Get_Unknown_speeds();
    auto pressure = m_system->Get_Unknown_static_pressures();
    size_t n_speed = speed.size();
    size_t n_pressure = pressure.size();
    for (size_t i = 0; i < n_speed; ++i) {
      *speed[i] = x(i) * si::meter_per_second; // TODO write more generic get unit from type
    }
    for (size_t j = n_speed; j < n_speed + n_pressure; ++j) {
      *pressure[n_speed - j] = x(j) * si::pascals; // TODO write more generic get unit from type
    }

    // TODO return vector
    return 0;
  }

private:
  std::shared_ptr<System> m_system;
};

typedef Eigen::NumericalDiff<System_Functor_Base> System_Functor;
}

#endif //LIBFLUIDS_FUNCTOR_H

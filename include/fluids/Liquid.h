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

#ifndef FLUIDS_LIQUID_H
#define FLUIDS_LIQUID_H

#include <memory>

#include "Units.h"

namespace Fluids {
class Liquid {
 public:
  Liquid();
  Liquid(const Liquid &other);

  Liquid &operator=(const Liquid &other);

  ~Liquid() = default;

  const std::shared_ptr<quantity<si::mass_density>> &Get_Density() const;
  void Set_Density(const std::shared_ptr<quantity<si::mass_density>> &density);

  const std::shared_ptr<quantity<si::dynamic_viscosity>> &Get_Dynamic_viscosity() const;
  void Set_Dynamic_viscosity(const std::shared_ptr<quantity<si::dynamic_viscosity>> &dynamic_viscosity);

  const std::shared_ptr<quantity<si::length>> &Get_Height() const;
  void Set_Height(const std::shared_ptr<quantity<si::length>> &height);

  const std::shared_ptr<quantity<si::velocity>> &Get_Speed() const;
  void Set_Speed(const std::shared_ptr<quantity<si::velocity>> &speed);

  const std::shared_ptr<quantity<si::pressure>> &Get_Static_pressure() const;
  void Set_Static_pressure(const std::shared_ptr<quantity<si::pressure>> &static_pressure);

  const std::shared_ptr<quantity<si::pressure>> &Get_Dynamic_pressure() const;
  const std::shared_ptr<quantity<si::pressure>> &Get_Potential_pressure() const;
  const std::shared_ptr<quantity<si::pressure>> &Get_Bernoulli() const;

 private:
  std::shared_ptr<quantity<si::pressure>> m_static_pressure;
  std::shared_ptr<quantity<si::velocity>> m_speed;
  std::shared_ptr<quantity<si::length>> m_height;
  std::shared_ptr<quantity<si::mass_density>> m_density;
  std::shared_ptr<quantity<si::dynamic_viscosity>> m_dynamic_viscosity;
  std::shared_ptr<quantity<si::pressure>> m_dynamic_pressure;
  std::shared_ptr<quantity<si::pressure>> m_potential_pressure;
  std::shared_ptr<quantity<si::pressure>> m_bernoulli;

};
}

#endif //FLUIDS_LIQUID_H

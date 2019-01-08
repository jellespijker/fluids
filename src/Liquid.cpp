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

#include "../include/fluids/Liquid.h"

#include <boost/units/cmath.hpp>
#include <fluids/Liquid.h>

namespace Fluids {

Liquid::Liquid() :
    State(),
    m_height(new quantity<si::length>(0. * si::meter)),
    m_density(new quantity<si::mass_density>(1000. * si::kilogram_per_cubic_meter)),
    m_dynamic_viscosity(new quantity<si::dynamic_viscosity>(1.15e-3 * si::pascals * si::seconds)),
    m_dynamic_pressure(new quantity<si::pressure>(0. * si::pascal)),
    m_potential_pressure(new quantity<si::pressure>(0. * si::pascal)),
    m_bernoulli(new quantity<si::pressure>(0. * si::pascal)) {

}

Liquid::Liquid(const Liquid &other) :
    State(other),
    m_height(new quantity<si::length>(*other.m_height)),
    m_density(new quantity<si::mass_density>(*other.m_density)),
    m_dynamic_viscosity(new quantity<si::dynamic_viscosity>(*other.m_dynamic_viscosity)),
    m_dynamic_pressure(new quantity<si::pressure>(*other.m_dynamic_pressure)),
    m_potential_pressure(new quantity<si::pressure>(*other.m_potential_pressure)),
    m_bernoulli(new quantity<si::pressure>(*other.m_bernoulli)) {

}

Liquid &Liquid::operator=(const Liquid &other) {
  if (this == &other)
    return *this;
  State::operator=(other);
  m_height = std::make_shared<quantity<si::length>>(*other.m_height);
  m_density = std::make_shared<quantity<si::mass_density>>(*other.m_density);
  m_dynamic_viscosity = std::make_shared<quantity<si::dynamic_viscosity>>(*other.m_dynamic_viscosity);
  m_dynamic_pressure = std::make_shared<quantity<si::pressure>>(*other.m_dynamic_pressure);
  m_potential_pressure = std::make_shared<quantity<si::pressure>>(*other.m_potential_pressure);
  m_bernoulli = std::make_shared<quantity<si::pressure>>(*other.m_bernoulli);
  return *this;
}

const std::shared_ptr<quantity<si::mass_density>> &Liquid::Get_Density() const {
  return m_density;
}

void Liquid::Set_Density(const std::shared_ptr<quantity<si::mass_density>> &density) {
  Liquid::m_density = density;
}

const std::shared_ptr<quantity<si::length>> &Liquid::Get_Height() const {
  return m_height;
}

void Liquid::Set_Height(const std::shared_ptr<quantity<si::length>> &height) {
  Liquid::m_height = height;
}

const std::shared_ptr<quantity<si::pressure>> &Liquid::Get_Dynamic_pressure() const {
  *m_dynamic_pressure = 0.5 * *this->Get_Density() * abs(*this->Get_Speed()) * *this->Get_Speed();
  return m_dynamic_pressure;
}

const std::shared_ptr<quantity<si::pressure>> &Liquid::Get_Potential_pressure() const {
  *m_potential_pressure = si::constants::g * *this->Get_Height() * *this->Get_Density();
  return m_potential_pressure;
}

const std::shared_ptr<quantity<si::pressure>> &Liquid::Get_Bernoulli() const {
  *m_bernoulli = *this->Get_Static_pressure() + *this->Get_Dynamic_pressure() + *this->Get_Potential_pressure();
  return m_bernoulli;
}

const std::shared_ptr<quantity<si::dynamic_viscosity>> &Liquid::Get_Dynamic_viscosity() const {
  return m_dynamic_viscosity;
}

void Liquid::Set_Dynamic_viscosity(const std::shared_ptr<quantity<si::dynamic_viscosity>> &dynamic_viscosity) {
  Liquid::m_dynamic_viscosity = dynamic_viscosity;
}
}

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

#include "../include/fluids/Pipes.h"

#include <math.h>

#include <boost/units/cmath.hpp>
#include <fluids/Pipes.h>

namespace Fluids {
Pipes::Pipes() :
    FluidComponents(std::make_shared<Liquid>(), std::make_shared<Liquid>()),
    m_diameter(new quantity<si::length>(0. * si::meter)),
    m_length(new quantity<si::length>(0. * si::meter)),
    m_roughness(new quantity<si::length>(0. * si::meter)),
    m_relative_roughness(new quantity<si::dimensionless>(0.)) {

}

Pipes::Pipes(const std::shared_ptr<Liquid> &liquid_u, const std::shared_ptr<Liquid> &liquid_v) :
    FluidComponents(liquid_u, liquid_v),
    m_diameter(new quantity<si::length>(0. * si::meter)),
    m_length(new quantity<si::length>(0. * si::meter)),
    m_roughness(new quantity<si::length>(0. * si::meter)),
    m_relative_roughness(new quantity<si::dimensionless>(0.)) {

}

Pipes::Pipes(const Fluids::Pipes &other) :
    FluidComponents(other),
    m_diameter(new quantity<si::length>(*other.m_diameter)),
    m_length(new quantity<si::length>(*other.m_length)),
    m_roughness(new quantity<si::length>(*other.m_roughness)),
    m_relative_roughness(new quantity<si::dimensionless>(*other.m_relative_roughness)) {

}

Pipes::Pipes(quantity<si::length> diameter, quantity<si::length> length, quantity<si::length> roughness) :
    FluidComponents(std::make_shared<Liquid>(), std::make_shared<Liquid>()),
    m_diameter(new quantity<si::length>(diameter)),
    m_length(new quantity<si::length>(length)),
    m_roughness(new quantity<si::length>(roughness)),
    m_relative_roughness(new quantity<si::dimensionless>(0.)) {

}

Pipes &Pipes::operator=(const Fluids::Pipes &other) {
  if (this == &other)
    return *this;
  FluidComponents::operator=(other);
  m_diameter = std::make_shared<quantity<si::length>>(*other.m_diameter);
  m_length = std::make_shared<quantity<si::length>>(*other.m_length);
  m_roughness = std::make_shared<quantity<si::length>>(*other.m_roughness);
  m_relative_roughness = std::make_shared<quantity<si::dimensionless>>(*other.m_relative_roughness);
  return *this;
}

std::shared_ptr<quantity<si::pressure>> Pipes::Get_DeltaPressure() {
  quantity<si::dimensionless> re = Pipes::Reynolds(*this->Get_Liquid(Vertex::u)->Get_Speed(),
                                                   *this->Get_Diameter(),
                                                   *this->Get_Liquid(Vertex::u)->Get_Density(),
                                                   *this->Get_Liquid(Vertex::u)->Get_Dynamic_viscosity());
  quantity<si::dimensionless> f = Pipes::Haaland(re, *this->Get_Relative_roughness());
  *Pipes::m_deltapressure =
      0.81056946914 * f * *this->Get_Length() * pow<2>(*this->Get_Volumetricflow())
          * *this->Get_Liquid(Vertex::u)->Get_Density().get()
          / pow<5>(*this->Get_Diameter());
  return m_deltapressure;
}

const std::shared_ptr<quantity<si::area>> &Pipes::Get_CrossSection() const {
  *m_crosssection = M_PI_4 * pow<2>(*this->Get_Diameter());
  return FluidComponents::Get_CrossSection();
}

void Pipes::Set_CrossSection(const std::shared_ptr<quantity<si::area>> &crosssection) {
  *m_diameter = sqrt(*crosssection / M_PI_4);
  FluidComponents::Set_CrossSection(crosssection);
}

const std::shared_ptr<quantity<si::length>> &Pipes::Get_Diameter() const {
  return m_diameter;
}

void Pipes::Set_Diameter(const std::shared_ptr<quantity<si::length>> &diameter) {
  Pipes::m_diameter = diameter;
}

const std::shared_ptr<quantity<si::length>> &Pipes::Get_Length() const {
  return m_length;
}

void Pipes::Set_Length(const std::shared_ptr<quantity<si::length>> &length) {
  Pipes::m_length = length;
}

const std::shared_ptr<quantity<si::length>> &Pipes::Get_Roughness() const {
  return m_roughness;
}

void Pipes::Set_Roughness(const std::shared_ptr<quantity<si::length>> &roughness) {
  Pipes::m_roughness = roughness;
}

const std::shared_ptr<quantity<si::dimensionless>> &Pipes::Get_Relative_roughness() const {
  *m_relative_roughness = *this->Get_Roughness() / *this->Get_Diameter();
  return m_relative_roughness;
}

quantity<si::dimensionless> Pipes::Reynolds(const quantity<si::velocity> &speed,
                                            const quantity<si::length> &diameter,
                                            const quantity<si::mass_density> &density,
                                            const quantity<si::dynamic_viscosity> &dynamic_viscosity) {
  return speed * diameter * density / dynamic_viscosity;
}

quantity<si::dimensionless> Pipes::Haaland(const quantity<si::dimensionless> &reynolds,
                                           const quantity<si::dimensionless> &relative_roughness) {
  quantity<si::dimensionless> A{pow(relative_roughness / 3.7, 1.11)};
  quantity<si::dimensionless> B{6.9 / reynolds};
  quantity<si::dimensionless> haaland{1.};
  haaland /= -1.8 * log10(A + B);
  return pow(haaland, 2);
}
}

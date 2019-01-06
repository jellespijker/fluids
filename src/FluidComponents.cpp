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

#include "../include/fluids/FluidComponents.h"

namespace Fluids {
FluidComponents::FluidComponents() :
    m_crosssection(new quantity<si::area>{0. * si::square_meter}),
    m_massflow(new quantity<si::mass_flow>(0. * si::kilogram_per_second)),
    m_volumetricflow(new quantity<si::volumetric_flow>(0. * si::cubic_meter_per_second)),
    m_deltapressure(new quantity<si::pressure>(0. * si::pascals)),
    m_liquid(new Liquid) {

}

FluidComponents::FluidComponents(std::shared_ptr<Liquid> liquid) :
    m_crosssection(new quantity<si::area>(0. * si::square_meter)),
    m_massflow(new quantity<si::mass_flow>(0. * si::kilogram_per_second)),
    m_volumetricflow(new quantity<si::volumetric_flow>(0. * si::cubic_meter_per_second)),
    m_deltapressure(new quantity<si::pressure>(0. * si::pascals)),
    m_liquid(liquid) {

}

FluidComponents::FluidComponents(const FluidComponents &other) :
    m_crosssection(new quantity<si::area>(*other.m_crosssection)),
    m_massflow(new quantity<si::mass_flow>(*other.m_massflow)),
    m_volumetricflow(new quantity<si::volumetric_flow>(*other.m_volumetricflow)),
    m_deltapressure(new quantity<si::pressure>(*other.m_deltapressure)),
    m_liquid(new Liquid(*other.m_liquid)) {

}

FluidComponents &FluidComponents::operator=(const Fluids::FluidComponents &other) {
  if (this == &other)
    return *this;
  m_crosssection = std::make_shared<quantity<si::area>>(*other.m_crosssection);
  m_massflow = std::make_shared<quantity<si::mass_flow>>(*other.m_massflow);
  m_volumetricflow = std::make_shared<quantity<si::volumetric_flow>>(*other.m_volumetricflow);
  m_deltapressure = std::make_shared<quantity<si::pressure>>(*other.m_deltapressure);
  m_liquid = std::make_shared<Liquid>(*other.m_liquid);
  return *this;
}

std::shared_ptr<quantity<si::mass_flow>> FluidComponents::Get_Massflow() {
  *m_massflow = *Get_Volumetricflow() * *Get_Liquid()->Get_Density();
  return m_massflow;
}

const std::shared_ptr<quantity<si::area>> &FluidComponents::Get_CrossSection() const {
  return m_crosssection;
}

void FluidComponents::Set_CrossSection(const std::shared_ptr<quantity<si::area>> &crosssection) {
  m_crosssection = crosssection;
}

const std::shared_ptr<Liquid> &FluidComponents::Get_Liquid() const {
  return m_liquid;
}

void FluidComponents::Set_Liquid(const std::shared_ptr<Liquid> &liquid) {
  m_liquid = liquid;
}

std::shared_ptr<quantity<si::pressure>> FluidComponents::Get_DeltaPressure() {
  return m_deltapressure;
}

std::shared_ptr<quantity<si::volumetric_flow>> FluidComponents::Get_Volumetricflow() {
  *m_volumetricflow = *Get_CrossSection() * *Get_Liquid()->Get_Speed();
  return m_volumetricflow;
}
}

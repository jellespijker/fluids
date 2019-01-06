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

#ifndef FLUIDS_FLUIDCOMPONENTS_H
#define FLUIDS_FLUIDCOMPONENTS_H

#include <memory>

#include "Units.h"
#include "Liquid.h"

namespace Fluids {
class FluidComponents {
 public:
  FluidComponents();
  explicit FluidComponents(std::shared_ptr<Liquid> liquid);
  FluidComponents(const FluidComponents &other);

  virtual ~FluidComponents() = default;

  virtual FluidComponents &operator=(const FluidComponents &other);

  virtual std::shared_ptr<quantity<si::pressure>> Get_DeltaPressure() = 0;
  virtual std::shared_ptr<quantity<si::mass_flow>> Get_Massflow();
  virtual std::shared_ptr<quantity<si::volumetric_flow>> Get_Volumetricflow();

  virtual const std::shared_ptr<quantity<si::area>> &Get_CrossSection() const;
  virtual void Set_CrossSection(const std::shared_ptr<quantity<si::area>> &crosssection);

  const std::shared_ptr<Liquid> &Get_Liquid() const;
  void Set_Liquid(const std::shared_ptr<Liquid> &liquid);

 protected:
  std::shared_ptr<quantity<si::area>> m_crosssection;
  std::shared_ptr<quantity<si::mass_flow>> m_massflow;
  std::shared_ptr<quantity<si::volumetric_flow>> m_volumetricflow;
  std::shared_ptr<quantity<si::pressure>> m_deltapressure;
  std::shared_ptr<Liquid> m_liquid;
};
}

#endif //FLUIDS_FLUIDCOMPONENTS_H

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

#ifndef FLUIDS_PIPES_H
#define FLUIDS_PIPES_H

#include "FluidComponents.h"

namespace Fluids {
class Pipes : public FluidComponents {
 public:
  Pipes();
  explicit Pipes(std::shared_ptr<Liquid> liquid);
  Pipes(const Pipes &other);
  Pipes(quantity<si::length> diameter, quantity<si::length> length, quantity<si::length> roughness);

  ~Pipes() = default;

  Pipes &operator=(const Pipes &other);

  std::shared_ptr<quantity<si::pressure>> Get_DeltaPressure() override;

  const std::shared_ptr<quantity<si::area>> &Get_CrossSection() const override;
  void Set_CrossSection(const std::shared_ptr<quantity<si::area>> &crosssection) override;

  const std::shared_ptr<quantity<si::length>> &Get_Diameter() const;
  void Set_Diameter(const std::shared_ptr<quantity<si::length>> &diameter);

  const std::shared_ptr<quantity<si::length>> &Get_Length() const;
  void Set_Length(const std::shared_ptr<quantity<si::length>> &length);

  const std::shared_ptr<quantity<si::length>> &Get_Roughness() const;
  void Set_Roughness(const std::shared_ptr<quantity<si::length>> &roughness);

  const std::shared_ptr<quantity<si::dimensionless>> &Get_Relative_roughness() const;

  static quantity<si::dimensionless> Reynolds(const quantity<si::velocity> &speed,
                                              const quantity<si::length> &diameter,
                                              const quantity<si::mass_density> &density,
                                              const quantity<si::dynamic_viscosity> &dynamic_viscosity);

  static quantity<si::dimensionless> Haaland(const quantity<si::dimensionless> &reynolds,
                                             const quantity<si::dimensionless> &relative_roughness);

 private:
  std::shared_ptr<quantity<si::length>> m_diameter;
  std::shared_ptr<quantity<si::length>> m_length;
  std::shared_ptr<quantity<si::length>> m_roughness;
  std::shared_ptr<quantity<si::dimensionless>> m_relative_roughness;
};
}

#endif //FLUIDS_PIPES_H

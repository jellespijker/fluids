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

#include <memory>
#include <iostream>

#include <gtest/gtest.h>

#include <fluids/Liquid.h>
#include <fluids/Pipes.h>
#include <fluids/System.h>
#include <fluids/Solver.h>

TEST(LiquidTest, StandardWater) {
  Fluids::Liquid water;
  ASSERT_EQ(*water.Get_Density(), 1000. * si::kilogram_per_cubic_meter);
  ASSERT_EQ(*water.Get_Static_pressure(), 1.e5 * si::pascals);
  ASSERT_EQ(*water.Get_Dynamic_viscosity(), 1.15e-3 * si::pascals * si::seconds);
  ASSERT_EQ(*water.Get_Height(), 0. * si::meters);
  ASSERT_EQ(*water.Get_Speed(), 0. * si::meters_per_second);
  ASSERT_EQ(*water.Get_Potential_pressure(), 0. * si::pascals);
  ASSERT_EQ(*water.Get_Dynamic_pressure(), 0. * si::pascals);
  ASSERT_EQ(*water.Get_Bernoulli(), 1.e5 * si::pascals);
  *water.Get_Speed() = 1. * si::meters_per_second;
  ASSERT_EQ(*water.Get_Dynamic_pressure(), 500. * si::pascals);
  ASSERT_EQ(*water.Get_Bernoulli(), 1.005e5 * si::pascals);
  *water.Get_Height() = 10. * si::meters;
  ASSERT_NEAR(water.Get_Potential_pressure()->value(), 98066.5, 0.1);
  ASSERT_NEAR(water.Get_Bernoulli()->value(), 198566.5, 0.1);
}

TEST(LiquidTest, CopyConstructor) {
  Fluids::Liquid water;
  *water.Get_Density() = 900. * si::kilogram_per_cubic_meter;
  Fluids::Liquid water2(water);
  ASSERT_EQ(*water.Get_Density(), *water2.Get_Density());
  ASSERT_NE(water.Get_Density(), water2.Get_Density());
}

TEST(LiquidTest, AssignOperator) {
  Fluids::Liquid water;
  *water.Get_Density() = 900. * si::kilogram_per_cubic_meter;
  Fluids::Liquid water2 = water;
  ASSERT_EQ(*water.Get_Density(), *water2.Get_Density());
  ASSERT_NE(water.Get_Density(), water2.Get_Density());
}

TEST(PipeTest, Pipes) {
  auto water = std::make_shared<Fluids::Liquid>();
  auto water2 = std::make_shared<Fluids::Liquid>();
  Fluids::Pipes pipe(water, water);
  *pipe.Get_Diameter() = 0.2 * si::meters;
  ASSERT_NEAR(pipe.Get_CrossSection()->value(), 0.0314159, 1e-6);
  *pipe.Get_Length() = 20. * si::meters;
  *pipe.Get_Roughness() = 4.6e-5 * si::meters;
  ASSERT_NEAR(*pipe.Get_Relative_roughness(), 0.00023, 1e-6);
  *water->Get_Speed() = 10. * si::meters_per_second;
  ASSERT_NEAR(pipe.Get_Volumetricflow()->value(), 0.314159, 1e-6);
  ASSERT_NEAR(pipe.Get_Massflow()->value(), 314.1589999, 1e-3);
  ASSERT_NEAR(pipe.Get_DeltaPressure()->value(), 73077.09055185, 1e6);
}

TEST(PipeTest, Haaland) {
  ASSERT_NEAR(Fluids::Pipes::Haaland(1.739130434783e6, 0.00023), 0.01461541811037, 1e-6);
}

TEST(PipeTest, Reynolds) {
  ASSERT_NEAR(Fluids::Pipes::Reynolds(10. * si::meters_per_second,
                                      0.2 * si::meters,
                                      1000. * si::kilogram_per_cubic_meter,
                                      1.15e-3 * si::pascals * si::seconds),
              1.739130434783e6, 1e-6);
}

TEST(PipeTest, CopyConstructor) {
  Fluids::Pipes pipe;
  *pipe.Get_Length() = 20. * si::meter;
  Fluids::Pipes pipe2(pipe);
  ASSERT_EQ(*pipe.Get_Length(), *pipe2.Get_Length());
  ASSERT_NE(pipe.Get_Length(), pipe2.Get_Length());
}

TEST(PipeTest, AssignOperator) {
  Fluids::Pipes pipe;
  *pipe.Get_Length() = 20. * si::meter;
  Fluids::Pipes pipe2 = pipe;
  ASSERT_EQ(*pipe.Get_Length(), *pipe2.Get_Length());
  ASSERT_NE(pipe.Get_Length(), pipe2.Get_Length());
}

TEST(PipeTest, SimpleInitialization) {
  Fluids::Pipes pipe(0.2 * si::meter, 10. * si::meter, 4.6e-5 * si::meters);
  ASSERT_EQ(*pipe.Get_Length(), 10. * si::meter);
  ASSERT_EQ(*pipe.Get_Diameter(), 0.2 * si::meter);
  ASSERT_EQ(*pipe.Get_Roughness(), 4.6e-5 * si::meter);
}

TEST(SystemTest, MakeGraph) {
  Fluids::Liquid water;
  Fluids::System system(water, 6);
  auto p0 = std::make_shared<Fluids::Pipes>(0.2 * si::meter, 10. * si::meter, 4.6e-5 * si::meters);
  system.add_FluidComponent(p0, 0, 1);
  auto p1(p0);
  system.add_FluidComponent(p1, 1, 2);
  auto p2(p0);
  system.add_FluidComponent(p2, 2, 3);
  auto p3(p0);
  system.add_FluidComponent(p3, 3, 4);
  auto p4(p0);
  system.add_FluidComponent(p4, 4, 5);
  auto p5(p0);
  system.add_FluidComponent(p5, 1, 4);
  system.Set_Known_Speed(0, 2 * si::meters_per_second);
  system.Set_Known_Static_Pressure(0, static_cast<quantity<si::pressure>>(2. * si::bar));
  system.Set_Known_Static_Pressure(5, static_cast<quantity<si::pressure>>(1. * si::bar));
  ASSERT_TRUE(true); // TODO write test criteria
}

TEST(SystemTest, SimpleSystem) {
  Fluids::Liquid water;
  Fluids::System sys(water, 2);
  auto p0 = std::make_shared<Fluids::Pipes>(0.2 * si::meter,
                                            10. * si::meter,
                                            static_cast<quantity<si::length>>(46. * si::micrometers));
  sys.add_FluidComponent(p0, 0, 1);
  sys.Set_Known_Speed(0, 2. * si::meters_per_second);
  sys.Set_Known_Speed(1, 2. * si::meters_per_second);
  sys.Set_Known_Static_Pressure(0, static_cast<quantity<si::pressure>>(2. * si::bar));
  sys.Set_Known_Static_Pressure(1, static_cast<quantity<si::pressure>>(1.983 * si::bar));
  sys.Initialize();
}

TEST(SolverTest, SimpleSystem) {
  Fluids::Liquid water;
  auto sys = std::make_shared<Fluids::System>(water, 2);
  auto p0 = std::make_shared<Fluids::Pipes>(0.2 * si::meter,
                                            10. * si::meter,
                                            static_cast<quantity<si::length>>(46. * si::micrometers));
  sys->add_FluidComponent(p0, 0, 1);
  sys->Set_Known_Speed(0, 2. * si::meters_per_second);
  sys->Set_Known_Static_Pressure(0, static_cast<quantity<si::pressure>>(2. * si::bar));
  sys->Initialize();

  Fluids::Solver solver(sys);

  solver.Solve();
  std::cout << *sys->Get_Liquid(1)->Get_Static_pressure() << std::endl;
  std::cout << *sys->Get_Liquid(1)->Get_Speed() << std::endl;
}
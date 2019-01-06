# Fluids

Solve a steady-state fluid problem. This project makes use of [Boost::units](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_units.html)
Zero-overhead dimensional analysis and unit/quantity manipulation and conversion.

# Use in your own projects

Link to library using cmake, see example `CMakeLists.txt` below:
```cmake
cmake_minimum_required(VERSION 3.8)
project(example_program)

find_package(Fluids)

add_executable(example_program example_program.cpp)
target_link_libraries(example_program Fluids::fluids)
``` 

Source code:
```c++
#include <memory>
#include <iostream>

#include <fluids/Liquid.h>
#include <fluids/Pipes.h>
#include <fluids/System.h>
#include <fluids/Solver.h>

int main (int argc, char *argv[]) {
      Fluids::Liquid water;
      auto system = std::make_shared<Fluids::System>(water, 6);
      auto p0 = std::make_shared<Fluids::Pipes>(0.2 * si::meter, 10. * si::meter, 46e-5 * si::meters);
      system->add_FluidComponent(p0, 0, 1);
      auto p1(p0);
      *p1->Get_Diameter() = 0.3 * si::meter;
      system->add_FluidComponent(p1, 1, 2);
      auto p2(p0);
      *p2->Get_Length() = 20. * si::meter;
      system->add_FluidComponent(p2, 2, 3);
      auto p3(p0);
      system->add_FluidComponent(p3, 3, 4);
      auto p4(p0);
      system->add_FluidComponent(p4, 4, 5);
      auto p5(p0);
      system->add_FluidComponent(p5, 1, 4);
      system->Set_Known_Speed(0, 2 * si::meters_per_second);
      system->Set_Known_Static_Pressure(0, static_cast<quantity<si::pressure>>(2. * si::bar));
      system->Set_Known_Static_Pressure(5, static_cast<quantity<si::pressure>>(1. * si::bar));
      Fluids::Solver solver(system);
      solver.solve();
      std::cout << "The fluid speed at point 1 is: " << *system->Get_Liquid(1)->Get_speed();
}

```


## Dependencies

- [Boost](https://www.boost.org/) (Units)
- [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page) (non-linear-optimization)

for development:

- [GTest](https://github.com/google/googletest/)

## Release
The latest main release can be downloaded from:

- [libfluids-Linux.tar.gz](https://gitlab.com/HAN-thesis/fluids/-/jobs/artifacts/master/raw/libfluids-Linux.tar.gz?job=release)
- Windows to come (but maybe you should think about running a real OS)

The latest develop release can be downloaded from:

- [libfluids-Linux.tar.gz](https://gitlab.com/HAN-thesis/fluids/-/jobs/artifacts/develop/raw/libfluids-Linux.tar.gz?job=release)
- Windows to come (but maybe you should think about running a real OS)

### Build from source

make sure you have [Boost](https://www.boost.org/), [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page) and [GTest](https://github.com/google/googletest/) installed.

```bash
$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
$ make -j 4
$ sudo make install
```



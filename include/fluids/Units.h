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

#ifndef FLUIDS_UNITS_H
#define FLUIDS_UNITS_H

#include <boost/units/io.hpp>
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/units/systems/si/codata/universal_constants.hpp>
#include <boost/units/base_units/metric/bar.hpp>
#include <boost/units/base_units/si/meter.hpp>

using namespace boost::units;

namespace boost {
namespace units {
typedef derived_dimension<length_base_dimension, 3,
                          time_base_dimension, -1>::type volumetric_flow_dimension;

typedef derived_dimension<mass_base_dimension, 1,
                          time_base_dimension, -1>::type mass_flow_dimension;
} // namespace units
} // namespace boost

namespace boost {
namespace units {

namespace si {
// Length types, constants and quantities.

typedef scaled_base_unit<meter_base_unit, scale<10, static_rational<-6>>>::unit_type micrometer_unit;
BOOST_UNITS_STATIC_CONSTANT(micrometer, micrometer_unit);
BOOST_UNITS_STATIC_CONSTANT(micrometers, micrometer_unit);
BOOST_UNITS_STATIC_CONSTANT(micrometre, micrometer_unit);
BOOST_UNITS_STATIC_CONSTANT(micrometres, micrometer_unit);
typedef scaled_base_unit<meter_base_unit, scale<10, static_rational<-3>>>::unit_type millimeter_unit;
BOOST_UNITS_STATIC_CONSTANT(millimeter, millimeter_unit);
BOOST_UNITS_STATIC_CONSTANT(millimeters, millimeter_unit);
BOOST_UNITS_STATIC_CONSTANT(millimetre, millimeter_unit);
BOOST_UNITS_STATIC_CONSTANT(millimetres, millimeter_unit);
typedef scaled_base_unit<meter_base_unit, scale<10, static_rational<-2>>>::unit_type centimeter_unit;
BOOST_UNITS_STATIC_CONSTANT(centimeter, centimeter_unit);
BOOST_UNITS_STATIC_CONSTANT(centimeters, centimeter_unit);
BOOST_UNITS_STATIC_CONSTANT(centimetre, centimeter_unit);
BOOST_UNITS_STATIC_CONSTANT(centimetres, centimeter_unit);

typedef scaled_base_unit<metric::bar_base_unit, scale<10, static_rational<0> > >::unit_type bar_unit;
BOOST_UNITS_STATIC_CONSTANT(bar, bar_unit);
BOOST_UNITS_STATIC_CONSTANT(bars, bar_unit);
typedef quantity<bar_unit> bar_quantity;

typedef unit<volumetric_flow_dimension, si::system> volumetric_flow;
BOOST_UNITS_STATIC_CONSTANT(cubic_meter_per_second, volumetric_flow);
BOOST_UNITS_STATIC_CONSTANT(cubic_meters_per_second, volumetric_flow);
BOOST_UNITS_STATIC_CONSTANT(cubic_metre_per_second, volumetric_flow);
BOOST_UNITS_STATIC_CONSTANT(cubic_metres_per_second, volumetric_flow);

typedef unit<mass_flow_dimension, si::system> mass_flow;
BOOST_UNITS_STATIC_CONSTANT(kilogram_per_second, mass_flow);
BOOST_UNITS_STATIC_CONSTANT(kilogramme_meters_per_second, mass_flow);
BOOST_UNITS_STATIC_CONSTANT(kilograms_metre_per_second, mass_flow);
BOOST_UNITS_STATIC_CONSTANT(kilogrammes_metres_per_second, mass_flow);

namespace constants {
BOOST_UNITS_PHYSICAL_CONSTANT(g,
                              quantity<acceleration>,
                              9.80665 * meter_per_second_squared,
                              0.0 * meter_per_second_squared);
}
} // namespace si
} // namespace units
} //namespace boost

#endif //FLUIDS_UNITS_H

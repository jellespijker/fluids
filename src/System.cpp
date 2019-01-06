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
#include <algorithm>

#include <fluids/System.h>
#include "../include/fluids/System.h"

namespace Fluids {

System::System() {

}

System::System(Liquid liquid, size_t num_vertices) {
  for (size_t i = 0; i < num_vertices; ++i) {
    vertex_t u = boost::add_vertex(m_graph);
    m_graph[u] = std::make_shared<Liquid>(liquid);
    m_unknown_speeds.push_back(m_graph[u]->Get_Speed());
    m_unknown_static_pressures.push_back(m_graph[u]->Get_Static_pressure());
  }
}

void System::add_FluidComponent(const std::shared_ptr<FluidComponents> &component,
                                const size_t &vertex_u,
                                const size_t &vertex_v) {
  edge_t e;
  bool b;
  vertex_t u = boost::vertex(vertex_u, m_graph);
  vertex_t v = boost::vertex(vertex_v, m_graph);
  boost::tie(e, b) = boost::add_edge(u, v, m_graph);
  component->Set_Liquid(m_graph[u]);
  m_graph[e] = component;
}

std::shared_ptr<Liquid> &System::Get_Liquid(const size_t &vertex_u) {
  vertex_t u = boost::vertex(vertex_u, m_graph);
  return m_graph[u];
}

std::shared_ptr<FluidComponents> &System::Get_Component(const size_t &vertex_u, const size_t &vertex_v) {
  edge_t e;
  bool b;
  vertex_t u = boost::vertex(vertex_u, m_graph);
  vertex_t v = boost::vertex(vertex_v, m_graph);
  boost::tie(e, b) = boost::edge(u, v, m_graph);
  return m_graph[e];
}

void System::Set_Known_Speed(const size_t &vertex_u, const quantity<si::velocity> &speed) {
  Set_Known<quantity<si::velocity>>(Get_Liquid(vertex_u)->Get_Speed(),
                                    speed,
                                    m_known_speeds,
                                    m_unknown_speeds);
}

void System::Set_Known_Static_Pressure(const size_t &vertex_u, const quantity<si::pressure> &pressure) {
  Set_Known<quantity<si::pressure>>(Get_Liquid(vertex_u)->Get_Static_pressure(),
                                    pressure,
                                    m_known_static_pressures,
                                    m_unknown_static_pressures);
}

size_t System::n_unknowns() {
  return m_unknown_speeds.size() + m_unknown_static_pressures.size() - 2;
}

const shared_velocity_vector &System::Get_Known_speeds() const {
  return m_known_speeds;
}

const shared_velocity_vector &System::Get_Unknown_speeds() const {
  return m_unknown_speeds;
}

const shared_pressure_vector &System::Get_Known_static_pressures() const {
  return m_known_static_pressures;
}

const shared_pressure_vector &System::Get_Unknown_static_pressures() const {
  return m_unknown_static_pressures;
}

}

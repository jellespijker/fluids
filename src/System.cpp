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
#include <numeric>
#include <random>

#include <Eigen/Eigen>
#include <fluids/System.h>

#include "../include/fluids/System.h"
#include "TransportEdge.h"

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
  component->Set_Liquid(Vertex::u, m_graph[u]);
  component->Set_Liquid(Vertex::v, m_graph[v]);
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
  return m_unknown_speeds.size() + m_unknown_static_pressures.size() + m_unknown_volumetric_flows.size();
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

const Graph &System::Get_Graph() const {
  return m_graph;
}

const Eigen::VectorXd System::Get_Return_vec() const {
  Eigen::VectorXd ber_vec = Get_Bernoulli_vec();
  Eigen::VectorXd mass_vec = Get_massflow_vec();
  Eigen::VectorXd ret_vec(ber_vec.size() + mass_vec.size());
  ret_vec << ber_vec, mass_vec;
  return ret_vec;
}

const Eigen::VectorXd System::Get_Bernoulli_vec() const {
  std::vector<double> values;
  auto es = boost::edges(m_graph);
  for (auto eit = es.first; eit != es.second; ++eit) {
    if (m_graph[*eit]->isTransportEdge())
      continue;
    values.push_back(m_graph[*eit]->Get_Bernoulli_balance()->value());
  }
  return Eigen::Map<Eigen::VectorXd>(values.data(), values.size());
}

const Eigen::VectorXd System::Get_massflow_vec() const {
  std::vector<double> values;
  values.push_back(0.); // System massflow
  auto vs = boost::vertices(m_graph);
  edge_t e;
  bool b;
  for (auto vit = vs.first; vit != vs.second; ++vit) {
    // Get system mass flows
    if (boost::in_degree(*vit, m_graph) == 0) { // incoming system mass flow
      typename boost::graph_traits<Graph>::out_edge_iterator eo, eo_end;
      for (boost::tie(eo, eo_end) = boost::out_edges(*vit, m_graph); eo != eo_end; ++eo) {
        boost::tie(e, b) = boost::edge(boost::source(*eo, m_graph), boost::target(*eo, m_graph), m_graph);
        values[0] += m_graph[e]->Get_Massflow()->value();
      }
      continue;
    } else if (boost::out_degree(*vit, m_graph) == 0) { // outgoing system mass flow
      typename boost::graph_traits<Graph>::in_edge_iterator ei, ei_end;
      for (boost::tie(ei, ei_end) = boost::in_edges(*vit, m_graph); ei != ei_end; ++ei) {
        boost::tie(e, b) = boost::edge(boost::source(*ei, m_graph), boost::target(*ei, m_graph), m_graph);
        values[0] -= m_graph[e]->Get_Massflow()->value();
      }
      continue;
    }
    values.push_back(0.);
    // Get all incoming flows
    typename boost::graph_traits<Graph>::in_edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::in_edges(*vit, m_graph); ei != ei_end; ++ei) {
      boost::tie(e, b) = boost::edge(boost::source(*ei, m_graph), boost::target(*ei, m_graph), m_graph);
      values.back() += m_graph[e]->Get_Massflow()->value();
    }
    // Get all outgoing flows
    typename boost::graph_traits<Graph>::out_edge_iterator eo, eo_end;
    for (boost::tie(eo, eo_end) = boost::out_edges(*vit, m_graph); eo != eo_end; ++eo) {
      boost::tie(e, b) = boost::edge(boost::source(*eo, m_graph), boost::target(*eo, m_graph), m_graph);
      values.back() -= m_graph[e]->Get_Massflow()->value();
    }
  }
  return Eigen::Map<Eigen::VectorXd>(values.data(), values.size());
}

void System::Initialize() {
  auto vs = boost::vertices(m_graph);
  // Loop over all vertices, when it is a leaf-vertex connect a massflow vertex and transport edge, needed to solve
  std::array<std::vector<vertex_t>, 2> leafs;
  for (auto vit = vs.first; vit != vs.second; ++vit) {
    if (boost::in_degree(*vit, m_graph) == 0) { // Set incoming massflow
      leafs[0].push_back(*vit);
    } else if (boost::out_degree(*vit, m_graph) == 0) { // Set outgoing massflow
      leafs[1].push_back(*vit);
    }
  }
  bool in_leaf = true;
  bool b;
  edge_t e;
  for (auto &&leaf : leafs) {
    for (auto &&v : leaf) {
      vertex_t u = boost::add_vertex(m_graph);
      m_graph[u] = std::make_shared<Liquid>(*m_graph[v]);
      if (in_leaf) {
        boost::tie(e, b) = boost::add_edge(u, v, m_graph);
        m_graph[e] = std::make_shared<TransportEdge>(m_graph[u], m_graph[v]);
        m_unknown_volumetric_flows.push_back(m_graph[e]->Get_Volumetricflow());
      } else {
        boost::tie(e, b) = boost::add_edge(v, u, m_graph);
        m_graph[e] = std::make_shared<TransportEdge>(m_graph[v], m_graph[u]);
        m_unknown_volumetric_flows.push_back(m_graph[e]->Get_Volumetricflow());
      }
    }
    in_leaf = false;
  }
}

Eigen::VectorXd System::Get_Initial_vector() {
  Eigen::VectorXd initial_vec(n_unknowns());
  initial_values<si::velocity>(m_known_speeds,
                               si::meters_per_second,
                               initial_vec,
                               0,
                               m_unknown_speeds.size(),
                               0.0, 10.0);
  initial_values<si::pressure>(m_known_static_pressures,
                               si::pascal,
                               initial_vec,
                               m_unknown_speeds.size(),
                               m_unknown_speeds.size() + m_unknown_static_pressures.size(),
                               1.0e5, 10.0e5);
  initial_values<si::volumetric_flow>(m_known_volumetric_flows,
                                      si::cubic_meters_per_second,
                                      initial_vec,
                                      m_unknown_speeds.size() + m_unknown_static_pressures.size(),
                                      n_unknowns(),
                                      1., 100.);
  return initial_vec;
}

const shared_volumetric_flow_vector &System::Get_Unknown_volumetric_flow() const {
  return m_unknown_volumetric_flows;
}

const shared_volumetric_flow_vector &System::Get_Known_volumetric_flow() const {
  return m_known_volumetric_flows;
}

}

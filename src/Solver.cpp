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

#include <Eigen/Eigen>
#include <unsupported/Eigen/NonLinearOptimization>

#include <fluids/Solver.h>
#include "../include/fluids/Solver.h"
#include "Functor.h"

namespace Fluids {

Solver::Solver() {

}

Solver::Solver(const std::shared_ptr<System> &system) : m_system(system) {

}

void Solver::Solve() {
  System_Functor func(m_system);
  Eigen::HybridNonLinearSolver<System_Functor> dl(func);
  Eigen::VectorXd x_initial(m_system->n_unknowns());
  // Initial guess
  // Solve
}

void Solver::Solve(const std::shared_ptr<System> &system) {
  if (system != Get_System())
    Set_System(system);
  Solver();
}

const std::shared_ptr<System> &Solver::Get_System() const {
  return m_system;
}

void Solver::Set_System(const std::shared_ptr<System> &system) {
  Solver::m_system = system;
}

}

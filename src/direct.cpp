// fgt — fast Gauss transforms
// Copyright (C) 2016 Peter J. Gadomski <pete.gadomski@gmail.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA

#include <cassert>
#include <cmath>

#include "fgt.hpp"

namespace fgt {

Direct::Direct(const MatrixRef source, double bandwidth)
    : Transform(source, bandwidth) {}

Vector Direct::compute_impl(const MatrixRef target,
                            const VectorRef weights) const {
    double h2 = bandwidth() * bandwidth();
    MatrixRef source = this->source();
    size_t rows_source = source.rows();
    size_t rows_target = target.rows();
    size_t cols = source.cols();
    Vector g = Vector::Zero(rows_target);
#pragma omp parallel for
    for (size_t j = 0; j < rows_target; ++j) {
        for (size_t i = 0; i < rows_source; ++i) {
            double distance = 0.0;
            for (size_t k = 0; k < cols; ++k) {
                double temp = source(i, k) - target(j, k);
                distance += temp * temp;
            }
            g[j] += weights[i] * std::exp(-distance / h2);
        }
    }
    return g;
}

Vector direct(const MatrixRef source, const MatrixRef target,
              double bandwidth) {
    return Direct(source, bandwidth).compute(target);
}

Vector direct(const MatrixRef source, const MatrixRef target, double bandwidth,
              const VectorRef weights) {
    return Direct(source, bandwidth).compute(target, weights);
}
}

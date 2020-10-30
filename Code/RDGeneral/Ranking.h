//
//  Copyright (C) 2004-2015 Greg Landrum and Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//

//! \file Ranking.h
/*!
    \brief Utility functionality used to rank sequences

    Much of this used to be in GraphMol/RankAtoms.h
*/
#include <RDGeneral/export.h>
#ifndef RD_RANKING_H
#define RD_RANKING_H

#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <boost/foreach.hpp>
#include <cstdint>

namespace Rankers {
//! functor for implementing > on two std::pairs.  The first entries are
// compared.
template <typename T1, typename T2>
struct pairGreater
    : public std::binary_function<std::pair<T1, T2>, std::pair<T1, T2>, bool> {
  bool operator()(const std::pair<T1, T2> &v1,
                  const std::pair<T1, T2> &v2) const {
    return v1.first > v2.first;
  }
};

//! function for implementing < on two std::pairs.  The first entries are
// compared.
template <typename T1, typename T2>
struct pairLess
    : public std::binary_function<std::pair<T1, T2>, std::pair<T1, T2>, bool> {
  bool operator()(const std::pair<T1, T2> &v1,
                  const std::pair<T1, T2> &v2) const {
    return v1.first < v2.first;
  }
};

template <typename T>
class argless : public std::binary_function<T, T, bool> {
 public:
  argless(const T &c) : std::binary_function<T, T, bool>(), container(c){};
  bool operator()(unsigned int v1, unsigned int v2) const {
    return container[v1] < container[v2];
  }
  const T &container;
};

//! ranks the entries in a vector
/*!
  \param vect the vector to rank
  \param res  is used to return the ranks of each entry
*/
template <typename T1, typename T2>
void rankVect(const std::vector<T1> &vect, std::vector<unsigned int>& indices, T2 &res, bool reset_indices = true) {
  size_t n = vect.size();
  PRECONDITION(res.size() >= n, "vector size mismatch");
  PRECONDITION(indices.size() == n, "vector/indices size mismatch");
  size_t n_minus_1 = n - 1;

  if (reset_indices) {
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), argless<std::vector<T1>>(vect));
  } else {
    // Use the existing indices for sorting.
    // Only sort runs of previously identical values - use the res vector to detect this,
    // which was used to store the ranks of the previous run.
    for (size_t start = 0; start < n; start++) {
      size_t end = start;
      while (end < n_minus_1 && res[indices[end]] == res[indices[end+1]]) {
        end++;
      }
      if (end != start) {
        std::sort(indices.begin()+start, indices.begin()+end, argless<std::vector<T1>>(vect));
        start = end;
      }
    }
  }

  res[0] = 0;
  int currRank = 0;
  for (size_t i = 1; i < n; i++) {
    unsigned int idx = indices[i];
    if (vect[idx] == vect[indices[i-1]]) {
      res[idx] = currRank;
    } else {
      res[idx] = ++currRank;
    }
  }
}

template <typename T1, typename T2>
void rankVect(const std::vector<T1> &vect, T2 &res) {
  PRECONDITION(res.size() >= vect.size(), "vector size mismatch");
  unsigned int nEntries = rdcast<unsigned int>(vect.size());

  std::vector<unsigned int> indices(nEntries);

  rankVect(vect, indices, res);
}


}  // namespace Rankers
#endif

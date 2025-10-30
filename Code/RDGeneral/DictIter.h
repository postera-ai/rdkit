#pragma once

#include <map>
#include <string>

#include "RDValue.h"

namespace RDKit {
namespace unstable {

struct DictConstIteratorKeys {
  std::map<std::string, RDValue>::const_iterator iter;
  void operator++() { ++iter; }
  const std::string &operator*() const { return iter->first; }
  bool operator!=(const DictConstIteratorKeys &other) const {
    return iter != other.iter;
  }
};

struct DictConstIterableKeys {
  std::map<std::string, RDValue>::const_iterator first, last;
  auto begin() const { return first; }
  auto end() const { return last; }
};

struct DictConstIteratorValues {
  std::map<std::string, RDValue>::const_iterator iter;
  void operator++() { ++iter; }
  const RDValue &operator*() const { return iter->second; }
  bool operator!=(const DictConstIteratorValues &other) const {
    return iter != other.iter;
  }
};

struct DictConstIterableValues {
  std::map<std::string, RDValue>::const_iterator first, last;
  auto begin() const { return first; }
  auto end() const { return last; }
};

struct DictConstIteratorItems {
  std::map<std::string, RDValue>::const_iterator iter;
  void operator++() { ++iter; }
  std::pair<const std::string &, const RDValue &> operator*() const {
    return {iter->first, iter->second};
  }
  bool operator!=(const DictConstIteratorItems &other) const {
    return iter != other.iter;
  }
};

struct DictConstIterableItems {
  std::map<std::string, RDValue>::const_iterator first, last;
  auto begin() const { return first; }
  auto end() const { return last; }
};

}  // namespace unstable
}  // namespace RDKit

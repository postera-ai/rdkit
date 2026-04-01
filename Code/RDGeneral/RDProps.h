//  Copyright (C) 2016-2026 Brian Kelley and other RDKit contributors
//   @@ All Rights Reserved @@
//
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//
#include <RDGeneral/export.h>
#ifndef RDKIT_RDPROPS_H
#define RDKIT_RDPROPS_H
#include "Dict.h"
#include "types.h"

namespace RDKit {

class RDProps {
 protected:
  mutable Dict d_props;
  // It is a quirk of history that this is mutable
  //  as the RDKit allows properties to be set
  //  on const objects.

 public:
  RDProps() : d_props() {}
  RDProps(const RDProps &rhs) : d_props(rhs.d_props) {}
  RDProps &operator=(const RDProps &rhs) {
    if (this == &rhs) {
      return *this;
    }
    d_props = rhs.d_props;
    return *this;
  }
  RDProps(RDProps &&o) noexcept = default;
  RDProps &operator=(RDProps &&rhs) noexcept = default;

  void clear() { d_props.reset(); }
  //! gets the underlying Dictionary
  const Dict &getDict() const { return d_props; }
  Dict &getDict() { return d_props; }

  // ------------------------------------
  //  Local Property Dict functionality
  //  all setProp functions are const because they
  //     are not meant to change the atom chemically
  // ------------------------------------
  STR_VECT getPropList(bool includePrivate = true,
                       bool includeComputed = true) const {
    const STR_VECT &tmp = d_props.keys();
    STR_VECT res, computed;
    if (!includeComputed &&
        getPropIfPresent(RDKit::detail::computedPropName, computed)) {
      computed.emplace_back(keyToString(RDKit::detail::computedPropName));
    }

    auto pos = tmp.begin();
    while (pos != tmp.end()) {
      if ((includePrivate || (*pos)[0] != '_') &&
          std::find(computed.begin(), computed.end(), *pos) == computed.end()) {
        res.push_back(*pos);
      }
      ++pos;
    }
    return res;
  }

  template <typename T>
  void setProp(DictKey key, T val, bool computed = false) const {
    if (computed) {
      addComputedKey(key);
    }
    d_props.setVal(key, val);
  }

  template <typename T>
  void getProp(DictKey key, T &res) const {
    d_props.getVal(key, res);
  }

  template <typename T>
  T getProp(DictKey key) const {
    return d_props.getVal<T>(key);
  }

  template <typename T>
  bool getPropIfPresent(DictKey key, T &res) const {
    return d_props.getValIfPresent(key, res);
  }

  bool hasProp(DictKey key) const { return d_props.hasVal(key); }

  void clearProp(DictKey key) const {
    removeComputedKey(key);
    d_props.clearVal(key);
  }

  void clearComputedProps() const {
    STR_VECT compLst;
    if (getPropIfPresent(RDKit::detail::computedPropName, compLst) &&
        !compLst.empty()) {
      for (const auto &sv : compLst) {
        d_props.clearVal(sv);
      }
      compLst.clear();
      d_props.setVal(RDKit::detail::computedPropName, compLst);
    }
  }

 private:
  void addComputedKey(DictKey dk) const {
    STR_VECT compLst;
    getPropIfPresent(RDKit::detail::computedPropName, compLst);
    const std::string &ks = keyToString(dk);
    if (std::find(compLst.begin(), compLst.end(), ks) == compLst.end()) {
      compLst.push_back(ks);
      d_props.setVal(RDKit::detail::computedPropName, compLst);
    }
  }
  void removeComputedKey(DictKey dk) const {
    STR_VECT compLst;
    if (getPropIfPresent(RDKit::detail::computedPropName, compLst)) {
      const std::string &ks = keyToString(dk);
      auto svi = std::find(compLst.begin(), compLst.end(), ks);
      if (svi != compLst.end()) {
        compLst.erase(svi);
        d_props.setVal(RDKit::detail::computedPropName, compLst);
      }
    }
  }

 public:

  //! update the properties from another
  /*
    \param source    Source to update the properties from
    \param preserve  Existing If true keep existing data, else override from
    the source
  */
  void updateProps(const RDProps &source, bool preserveExisting = false) {
    d_props.update(source.getDict(), preserveExisting);
  }
};
}  // namespace RDKit
#endif

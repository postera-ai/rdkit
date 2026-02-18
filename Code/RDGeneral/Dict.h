//
// Copyright (C) 2003-2026 Greg Landrum and other RDKit contributors
//
//  @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//
/*! \file Dict.h

  \brief Defines the Dict class

*/
#include <RDGeneral/export.h>
#ifndef RD_DICT_H_012020
#define RD_DICT_H_012020

#include <map>
#include <string>
#include <string_view>
#include <vector>
#include "RDValue.h"
#include "Exceptions.h"
#include "PropKeyIntern.h"
#include <RDGeneral/BoostStartInclude.h>
#include <boost/lexical_cast.hpp>
#include <RDGeneral/BoostEndInclude.h>

namespace RDKit {
typedef std::vector<std::string> STR_VECT;

//! \brief The \c Dict class can be used to store objects of arbitrary
//!        type keyed by \c strings.
//!
//!  The actual storage is done using \c RDValue objects.
//!
//!  Internally, keys that match the global interned property set
//!  (common_properties) are stored as compact uint16_t IDs for faster
//!  lookup. Unknown keys go into an overflow vector.
//!
class RDKIT_RDGENERAL_EXPORT Dict {
 public:
  struct Pair {
    std::string key;
    RDValue val;

    Pair() : key(), val() {}
    explicit Pair(std::string s) : key(std::move(s)), val() {}
    explicit Pair(std::string_view s) : key(std::string(s)), val() {}
    Pair(std::string s, const RDValue &v) : key(std::move(s)), val(v) {}
    Pair(std::string_view s, const RDValue &v) : key(std::string(s)), val(v) {}
    void cleanup() { RDValue::cleanup_rdvalue(val); }
  };

  struct InternedPair {
    PropKeyId keyId;
    RDValue val;

    InternedPair() : keyId(kInvalidPropKey), val() {}
    InternedPair(PropKeyId id, const RDValue &v) : keyId(id), val(v) {}
  };

  typedef std::vector<Pair> DataType;

  struct PairView {
    std::string_view key;
    const RDValue &val;
  };

  class const_iterator {
   public:
    using value_type = PairView;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    const_iterator() = default;
    const_iterator(const Dict *dict, std::size_t internIdx,
                   std::size_t overflowIdx)
        : d_dict(dict), d_internIdx(internIdx), d_overflowIdx(overflowIdx) {}

    PairView operator*() const {
      if (d_internIdx < d_dict->_interned.size()) {
        const auto &ip = d_dict->_interned[d_internIdx];
        return {propKeyToString(ip.keyId), ip.val};
      }
      const auto &op = d_dict->_overflow[d_overflowIdx];
      return {op.key, op.val};
    }

    const_iterator &operator++() {
      if (d_internIdx < d_dict->_interned.size()) {
        ++d_internIdx;
      } else {
        ++d_overflowIdx;
      }
      return *this;
    }

    const_iterator operator++(int) {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const const_iterator &other) const {
      return d_internIdx == other.d_internIdx &&
             d_overflowIdx == other.d_overflowIdx;
    }

    bool operator!=(const const_iterator &other) const {
      return !(*this == other);
    }

   private:
    const Dict *d_dict{nullptr};
    std::size_t d_internIdx{0};
    std::size_t d_overflowIdx{0};
  };

  Dict() {}

  Dict(const Dict &other)
      : _interned(other._interned),
        _overflow(other._overflow),
        _hasNonPodData(other._hasNonPodData) {
    if (other._hasNonPodData) {
      // deep copy interned values
      std::vector<InternedPair> idata(other._interned.size());
      _interned.swap(idata);
      for (size_t i = 0; i < _interned.size(); ++i) {
        _interned[i].keyId = other._interned[i].keyId;
        copy_rdvalue(_interned[i].val, other._interned[i].val);
      }
      // deep copy overflow values
      std::vector<Pair> odata(other._overflow.size());
      _overflow.swap(odata);
      for (size_t i = 0; i < _overflow.size(); ++i) {
        _overflow[i].key = other._overflow[i].key;
        copy_rdvalue(_overflow[i].val, other._overflow[i].val);
      }
    }
  }

  Dict(Dict &&other) noexcept = default;

  ~Dict() {
    reset();
  }

  void update(const Dict &other, bool preserveExisting = false) {
    if (!preserveExisting) {
      *this = other;
    } else {
      if (other._hasNonPodData) {
        _hasNonPodData = true;
      }
      for (const auto &oip : other._interned) {
        RDValue *target = findMutableVal(oip.keyId);
        if (!target) {
          _interned.push_back(InternedPair(oip.keyId, RDValue()));
          copy_rdvalue(_interned.back().val, oip.val);
        } else {
          copy_rdvalue(*target, oip.val);
        }
      }
      for (const auto &opair : other._overflow) {
        RDValue *target = nullptr;
        for (auto &dpair : _overflow) {
          if (dpair.key == opair.key) {
            target = &dpair.val;
            break;
          }
        }
        if (!target) {
          _overflow.push_back(Pair(opair.key));
          copy_rdvalue(_overflow.back().val, opair.val);
        } else {
          copy_rdvalue(*target, opair.val);
        }
      }
    }
  }

  Dict &operator=(const Dict &other) {
    if (this == &other) {
      return *this;
    }
    if (_hasNonPodData) {
      reset();
    }

    if (other._hasNonPodData) {
      std::vector<InternedPair> idata(other._interned.size());
      _interned.swap(idata);
      for (size_t i = 0; i < _interned.size(); ++i) {
        _interned[i].keyId = other._interned[i].keyId;
        copy_rdvalue(_interned[i].val, other._interned[i].val);
      }
      std::vector<Pair> odata(other._overflow.size());
      _overflow.swap(odata);
      for (size_t i = 0; i < _overflow.size(); ++i) {
        _overflow[i].key = other._overflow[i].key;
        copy_rdvalue(_overflow[i].val, other._overflow[i].val);
      }
    } else {
      _interned = other._interned;
      _overflow = other._overflow;
    }
    _hasNonPodData = other._hasNonPodData;
    return *this;
  }

  Dict &operator=(Dict &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    if (_hasNonPodData) {
      reset();
    }
    _hasNonPodData = other._hasNonPodData;
    other._hasNonPodData = false;
    _interned = std::move(other._interned);
    _overflow = std::move(other._overflow);
    return *this;
  }

  //----------------------------------------------------------
  std::size_t size() const { return _interned.size() + _overflow.size(); }

  bool empty() const { return _interned.empty() && _overflow.empty(); }

  const_iterator begin() const { return {this, 0, 0}; }
  const_iterator end() const {
    return {this, _interned.size(), _overflow.size()};
  }

  void appendPair(Pair &&pair) {
    if (pair.val.needsCleanup()) {
      _hasNonPodData = true;
    }
    auto id = internPropKey(pair.key);
    if (id) {
      _interned.push_back(InternedPair(*id, pair.val));
      pair.val.type = RDTypeTag::EmptyTag;
    } else {
      _overflow.push_back(std::move(pair));
    }
  }

  const RDValue &getRawVal(const std::string_view what) const {
    auto id = internPropKey(what);
    if (id) {
      for (const auto &ip : _interned) {
        if (ip.keyId == *id) {
          return ip.val;
        }
      }
    } else {
      for (const auto &op : _overflow) {
        if (op.key == what) {
          return op.val;
        }
      }
    }
    throw KeyErrorException(what);
  }

  //----------------------------------------------------------

  bool hasVal(const std::string_view what) const {
    auto id = internPropKey(what);
    if (id) {
      for (const auto &ip : _interned) {
        if (ip.keyId == *id) {
          return true;
        }
      }
      return false;
    }
    for (const auto &op : _overflow) {
      if (op.key == what) {
        return true;
      }
    }
    return false;
  }

  //----------------------------------------------------------
  STR_VECT keys() const {
    STR_VECT res;
    res.reserve(size());
    for (const auto &ip : _interned) {
      res.emplace_back(propKeyToString(ip.keyId));
    }
    for (const auto &op : _overflow) {
      res.push_back(op.key);
    }
    return res;
  }

  //----------------------------------------------------------
  template <typename T>
  void getVal(const std::string_view what, T &res) const {
    res = getVal<T>(what);
  }

  template <typename T>
  T getVal(const std::string_view what) const {
    auto id = internPropKey(what);
    if (id) {
      for (const auto &ip : _interned) {
        if (ip.keyId == *id) {
          return from_rdvalue<T>(ip.val);
        }
      }
    } else {
      for (const auto &op : _overflow) {
        if (op.key == what) {
          return from_rdvalue<T>(op.val);
        }
      }
    }
    throw KeyErrorException(what);
  }

  void getVal(const std::string_view what, std::string &res) const {
    auto id = internPropKey(what);
    if (id) {
      for (const auto &ip : _interned) {
        if (ip.keyId == *id) {
          rdvalue_tostring(ip.val, res);
          return;
        }
      }
    } else {
      for (const auto &op : _overflow) {
        if (op.key == what) {
          rdvalue_tostring(op.val, res);
          return;
        }
      }
    }
    throw KeyErrorException(what);
  }

  //----------------------------------------------------------
  template <typename T>
  bool getValIfPresent(const std::string_view what, T &res) const {
    auto id = internPropKey(what);
    if (id) {
      for (const auto &ip : _interned) {
        if (ip.keyId == *id) {
          res = from_rdvalue<T>(ip.val);
          return true;
        }
      }
      return false;
    }
    for (const auto &op : _overflow) {
      if (op.key == what) {
        res = from_rdvalue<T>(op.val);
        return true;
      }
    }
    return false;
  }

  bool getValIfPresent(const std::string_view what, std::string &res) const {
    auto id = internPropKey(what);
    if (id) {
      for (const auto &ip : _interned) {
        if (ip.keyId == *id) {
          rdvalue_tostring(ip.val, res);
          return true;
        }
      }
      return false;
    }
    for (const auto &op : _overflow) {
      if (op.key == what) {
        rdvalue_tostring(op.val, res);
        return true;
      }
    }
    return false;
  }

  //----------------------------------------------------------
  template <typename T>
  void setVal(const std::string_view what, T &val) {
    static_assert(!std::is_same_v<T, std::string_view>,
                  "T cannot be string_view");
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    _hasNonPodData = true;
    auto id = internPropKey(what);
    if (id) {
      for (auto &ip : _interned) {
        if (ip.keyId == *id) {
          RDValue::cleanup_rdvalue(ip.val);
          ip.val = val;
          return;
        }
      }
      _interned.push_back(InternedPair(*id, val));
    } else {
      for (auto &op : _overflow) {
        if (op.key == what) {
          RDValue::cleanup_rdvalue(op.val);
          op.val = val;
          return;
        }
      }
      _overflow.push_back(Pair(what, val));
    }
  }

  template <typename T>
  void setPODVal(const std::string_view what, T val) {
    static_assert(!std::is_same_v<T, std::string_view>,
                  "T cannot be string_view");
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    auto id = internPropKey(what);
    if (id) {
      for (auto &ip : _interned) {
        if (ip.keyId == *id) {
          RDValue::cleanup_rdvalue(ip.val);
          ip.val = val;
          return;
        }
      }
      _interned.push_back(InternedPair(*id, val));
    } else {
      for (auto &op : _overflow) {
        if (op.key == what) {
          RDValue::cleanup_rdvalue(op.val);
          op.val = val;
          return;
        }
      }
      _overflow.push_back(Pair(what, val));
    }
  }

  void setVal(const std::string_view what, bool val) {
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    setPODVal(what, val);
  }

  void setVal(const std::string_view what, double val) {
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    setPODVal(what, val);
  }

  void setVal(const std::string_view what, float val) {
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    setPODVal(what, val);
  }
  void setVal(const std::string_view what, int val) {
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    setPODVal(what, val);
  }

  void setVal(const std::string_view what, unsigned int val) {
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    setPODVal(what, val);
  }

  void setVal(const std::string_view what, const char *val) {
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    std::string h(val);
    setVal(what, h);
  }

  //----------------------------------------------------------
  void clearVal(const std::string_view what) {
    auto id = internPropKey(what);
    if (id) {
      for (auto it = _interned.begin(); it != _interned.end(); ++it) {
        if (it->keyId == *id) {
          if (_hasNonPodData) {
            RDValue::cleanup_rdvalue(it->val);
          }
          _interned.erase(it);
          return;
        }
      }
    } else {
      for (auto it = _overflow.begin(); it != _overflow.end(); ++it) {
        if (it->key == what) {
          if (_hasNonPodData) {
            RDValue::cleanup_rdvalue(it->val);
          }
          _overflow.erase(it);
          return;
        }
      }
    }
  }

  //----------------------------------------------------------
  void reset() {
    if (_hasNonPodData) {
      for (auto &ip : _interned) {
        RDValue::cleanup_rdvalue(ip.val);
      }
      for (auto &op : _overflow) {
        RDValue::cleanup_rdvalue(op.val);
      }
    }
    {
      std::vector<InternedPair> tmp;
      _interned.swap(tmp);
    }
    {
      DataType tmp;
      _overflow.swap(tmp);
    }
  }

 private:
  friend class const_iterator;

  RDValue *findMutableVal(PropKeyId id) {
    for (auto &ip : _interned) {
      if (ip.keyId == id) {
        return &ip.val;
      }
    }
    return nullptr;
  }

  std::vector<InternedPair> _interned{};
  DataType _overflow{};
  bool _hasNonPodData{false};
};

template <>
inline std::string Dict::getVal<std::string>(
    const std::string_view what) const {
  std::string res;
  getVal(what, res);
  return res;
}

class PairHolder : public Dict::Pair {
 public:
  PairHolder() : Pair() {}

  explicit PairHolder(const PairHolder &p) : Pair(p.key) {
    copy_rdvalue(this->val, p.val);
  }

  explicit PairHolder(PairHolder &&p) : Pair(p.key) {
    this->val = p.val;
    p.val.type = RDTypeTag::EmptyTag;
  }

  explicit PairHolder(Dict::Pair &&p) : Pair(p.key) {
    this->val = p.val;
    p.val.type = RDTypeTag::EmptyTag;
  }

  ~PairHolder() { RDValue::cleanup_rdvalue(this->val); }
};
}  // namespace RDKit
#endif

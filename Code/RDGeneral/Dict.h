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
#include "DictKeyIntern.h"
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
class RDKIT_RDGENERAL_EXPORT Dict {
 public:
  struct InternalPair {
    DictKey key;
    RDValue val;

    InternalPair() : key(0), val() {}
    explicit InternalPair(DictKey k) : key(k), val() {}
    InternalPair(DictKey k, const RDValue &v) : key(k), val(v) {}
    void cleanup() { RDValue::cleanup_rdvalue(val); }
  };

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

  typedef std::vector<Pair> DataType;
  typedef std::vector<InternalPair> InternalDataType;

  Dict() {}

  Dict(const Dict &other) : _data(other._data) {
    _hasNonPodData = other._hasNonPodData;
    if (other._hasNonPodData) {
      InternalDataType data(other._data.size());
      _data.swap(data);
      for (size_t i = 0; i < _data.size(); ++i) {
        _data[i].key = other._data[i].key;
        copy_rdvalue(_data[i].val, other._data[i].val);
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
      for (const auto &opair : other._data) {
        InternalPair *target = nullptr;
        for (auto &dpair : _data) {
          if (dpair.key == opair.key) {
            target = &dpair;
            break;
          }
        }

        if (!target) {
          _data.push_back(InternalPair(opair.key));
          copy_rdvalue(_data.back().val, opair.val);
        } else {
          copy_rdvalue(target->val, opair.val);
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
      InternalDataType data(other._data.size());
      _data.swap(data);
      for (size_t i = 0; i < _data.size(); ++i) {
        _data[i].key = other._data[i].key;
        copy_rdvalue(_data[i].val, other._data[i].val);
      }
    } else {
      _data = other._data;
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
    _data = std::move(other._data);
    return *this;
  }

  //----------------------------------------------------------
  //! \brief Access to the underlying non-POD containment flag
  bool &getNonPODStatus() { return _hasNonPodData; }

  //----------------------------------------------------------
  //! \brief Access to the underlying data as external Pairs (string keys).
  DataType getData() const {
    DataType result;
    result.reserve(_data.size());
    for (const auto &ip : _data) {
      result.push_back(Pair(keyToString(ip.key), ip.val));
    }
    return result;
  }

  //! \brief Direct access to the internal storage (integer keys).
  const InternalDataType &getInternalData() const { return _data; }
  InternalDataType &getInternalData() { return _data; }

  //----------------------------------------------------------

  //! \brief Returns whether or not the dictionary contains a particular key.
  bool hasVal(std::string_view what) const {
    DictKey k = internKey(what);
    return hasVal(k);
  }
  bool hasVal(DictKey k) const {
    for (const auto &data : _data) {
      if (data.key == k) {
        return true;
      }
    }
    return false;
  }

  //----------------------------------------------------------
  //! Returns the set of keys in the dictionary
  STR_VECT keys() const {
    STR_VECT res;
    res.reserve(_data.size());
    for (const auto &item : _data) {
      res.push_back(keyToString(item.key));
    }
    return res;
  }

  //----------------------------------------------------------
  //! \brief Gets the value associated with a particular key
  template <typename T>
  void getVal(std::string_view what, T &res) const {
    res = getVal<T>(what);
  }
  template <typename T>
  void getVal(DictKey k, T &res) const {
    res = getVal<T>(k);
  }

  template <typename T>
  T getVal(std::string_view what) const {
    DictKey k = internKey(what);
    return getVal<T>(k);
  }
  template <typename T>
  T getVal(DictKey k) const {
    for (auto &data : _data) {
      if (data.key == k) {
        return from_rdvalue<T>(data.val);
      }
    }
    throw KeyErrorException(keyToString(k));
  }

  void getVal(std::string_view what, std::string &res) const {
    DictKey k = internKey(what);
    getVal(k, res);
  }
  void getVal(DictKey k, std::string &res) const {
    for (const auto &i : _data) {
      if (i.key == k) {
        rdvalue_tostring(i.val, res);
        return;
      }
    }
    throw KeyErrorException(keyToString(k));
  }

  //----------------------------------------------------------
  //! \brief Potentially gets the value associated with a particular key
  template <typename T>
  bool getValIfPresent(std::string_view what, T &res) const {
    DictKey k = internKey(what);
    return getValIfPresent(k, res);
  }
  template <typename T>
  bool getValIfPresent(DictKey k, T &res) const {
    for (const auto &data : _data) {
      if (data.key == k) {
        res = from_rdvalue<T>(data.val);
        return true;
      }
    }
    return false;
  }

  bool getValIfPresent(std::string_view what, std::string &res) const {
    DictKey k = internKey(what);
    return getValIfPresent(k, res);
  }
  bool getValIfPresent(DictKey k, std::string &res) const {
    for (const auto &i : _data) {
      if (i.key == k) {
        rdvalue_tostring(i.val, res);
        return true;
      }
    }
    return false;
  }

  //----------------------------------------------------------
  //! \brief Sets the value associated with a key
  template <typename T>
  void setVal(std::string_view what, T &val) {
    static_assert(!std::is_same_v<T, std::string_view>,
                  "T cannot be string_view");
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    DictKey k = internKey(what);
    setVal(k, val);
  }
  template <typename T>
  void setVal(DictKey k, T &val) {
    _hasNonPodData = true;
    for (auto &&data : _data) {
      if (data.key == k) {
        RDValue::cleanup_rdvalue(data.val);
        data.val = val;
        return;
      }
    }
    _data.emplace_back(k, val);
  }

  template <typename T>
  void setPODVal(std::string_view what, T val) {
    static_assert(!std::is_same_v<T, std::string_view>,
                  "T cannot be string_view");
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    DictKey k = internKey(what);
    setPODVal(k, val);
  }
  template <typename T>
  void setPODVal(DictKey k, T val) {
    for (auto &&data : _data) {
      if (data.key == k) {
        RDValue::cleanup_rdvalue(data.val);
        data.val = val;
        return;
      }
    }
    _data.emplace_back(k, val);
  }

  void setVal(std::string_view what, bool val) {
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    setPODVal(what, val);
  }
  void setVal(DictKey k, bool val) { setPODVal(k, val); }

  void setVal(std::string_view what, double val) {
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    setPODVal(what, val);
  }
  void setVal(DictKey k, double val) { setPODVal(k, val); }

  void setVal(std::string_view what, float val) {
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    setPODVal(what, val);
  }
  void setVal(DictKey k, float val) { setPODVal(k, val); }

  void setVal(std::string_view what, int val) {
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    setPODVal(what, val);
  }
  void setVal(DictKey k, int val) { setPODVal(k, val); }

  void setVal(std::string_view what, unsigned int val) {
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    setPODVal(what, val);
  }
  void setVal(DictKey k, unsigned int val) { setPODVal(k, val); }

  void setVal(std::string_view what, const char *val) {
    if (what.empty()) {
      throw ValueErrorException("Cannot set value with empty key");
    }
    std::string h(val);
    setVal(what, h);
  }
  void setVal(DictKey k, const char *val) {
    std::string h(val);
    setVal(k, h);
  }

  //----------------------------------------------------------
  //! \brief Clears the value associated with a particular key.
  void clearVal(std::string_view what) {
    DictKey k = internKey(what);
    clearVal(k);
  }
  void clearVal(DictKey k) {
    for (auto it = _data.begin(); it < _data.end(); ++it) {
      if (it->key == k) {
        if (_hasNonPodData) {
          RDValue::cleanup_rdvalue(it->val);
        }
        _data.erase(it);
        return;
      }
    }
  }

  //----------------------------------------------------------
  //! \brief Clears all keys (and values) from the dictionary.
  void reset() {
    if (_hasNonPodData) {
      for (auto &&data : _data) {
        RDValue::cleanup_rdvalue(data.val);
      }
    }
    InternalDataType data;
    _data.swap(data);
  }

 private:
  InternalDataType _data{};
  bool _hasNonPodData{false};
};

template <>
inline std::string Dict::getVal<std::string>(std::string_view what) const {
  std::string res;
  getVal(what, res);
  return res;
}
template <>
inline std::string Dict::getVal<std::string>(DictKey k) const {
  std::string res;
  getVal(k, res);
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

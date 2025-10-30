//
// Copyright (C) 2003-2021 Greg Landrum and other RDKit contributors
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
#include "RDValue.h"
#include "DictIter.h"
#include "Exceptions.h"
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
  typedef std::map<std::string, RDValue> DataType;

  Dict() = default;

  Dict(const Dict &other) : _hasNonPodData(other._hasNonPodData) {
    // Perform a deep copy if the other dictionary has non-POD data
    if (_hasNonPodData) {
      for (const auto &kv : other._data) {
        RDValue tmp;
        copy_rdvalue(tmp, kv.second);
        _data.emplace(kv.first, tmp);
      }
    } else {
      _data = other._data;
    }
  }

  Dict(Dict &&other) noexcept
      : _data(std::move(other._data)), _hasNonPodData(other._hasNonPodData) {
    other._hasNonPodData = false;
  }

  Dict &operator=(const Dict &other) {
    if (this == &other) {
      return *this;
    }
    if (_hasNonPodData) {
      reset();
    }
    _hasNonPodData = other._hasNonPodData;
    if (_hasNonPodData) {
      for (const auto &kv : other._data) {
        RDValue tmp;
        copy_rdvalue(tmp, kv.second);
        _data.emplace(kv.first, tmp);
      }
    } else {
      _data = other._data;
    }
    return *this;
  }

  Dict &operator=(Dict &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    if (_hasNonPodData) {
      reset();
    }
    _data = std::move(other._data);
    _hasNonPodData = other._hasNonPodData;
    other._hasNonPodData = false;
    return *this;
  }

  ~Dict() {
    reset();  // to clear pointers if necessary
  }

  //----------------------------------------------------------
  //! \brief Access to the underlying non-POD containment flag
  //! This is meant to be used only in bulk updates of _data.
  bool &getNonPODStatus() { return _hasNonPodData; }

  //----------------------------------------------------------
  //! \brief Returns whether or not the dictionary contains a particular
  //!        key.
  bool hasVal(std::string_view what) const {
    return _data.find(std::string(what)) != _data.end();
  }

  //----------------------------------------------------------
  //! Returns the set of keys in the dictionary
  /*!
     \return  a \c STR_VECT
  */
  STR_VECT keys() const {
    STR_VECT res;
    res.reserve(_data.size());
    for (const auto &kv : _data) {
      res.push_back(kv.first);
    }
    return res;
  }

  //----------------------------------------------------------
  //! \brief Gets the value associated with a particular key
  /*!
     \param what  the key to lookup
     \param res   a reference used to return the result

     <b>Notes:</b>
      - If \c res is a \c std::string, every effort will be made
        to convert the specified element to a string using the
        \c boost::lexical_cast machinery.
      - If the dictionary does not contain the key \c what,
        a KeyErrorException will be thrown.
  */
  template <typename T>
  void getVal(std::string_view what, T &res) const {
    res = getVal<T>(what);
  }

  //! \overload
  template <typename T>
  T getVal(std::string_view what) const {
    auto it = _data.find(std::string(what));
    if (it == _data.end()) {
      throw KeyErrorException(what);
    }
    return from_rdvalue<T>(it->second);
  }

  //! \overload
  void getVal(std::string_view what, std::string &res) const {
    auto it = _data.find(std::string(what));
    if (it == _data.end()) {
      throw KeyErrorException(what);
    }
    rdvalue_tostring(it->second, res);
  }

  //----------------------------------------------------------
  //! \brief Potentially gets the value associated with a particular key
  //!        returns true on success/false on failure.
  /*!
     \param what  the key to lookup
     \param res   a reference used to return the result

     <b>Notes:</b>
      - If \c res is a \c std::string, every effort will be made
        to convert the specified element to a string using the
        \c boost::lexical_cast machinery.
      - If the dictionary does not contain the key \c what,
        a KeyErrorException will be thrown.
  */
  template <typename T>
  bool getValIfPresent(std::string_view what, T &res) const {
    auto it = _data.find(std::string(what));
    if (it == _data.end()) {
      return false;
    }
    res = from_rdvalue<T>(it->second);
    return true;
  }

  //! \overload
  bool getValIfPresent(std::string_view what, std::string &res) const {
    auto it = _data.find(std::string(what));
    if (it == _data.end()) {
      return false;
    }
    rdvalue_tostring(it->second, res);
    return true;
  }

  //----------------------------------------------------------
  //! \brief Sets the value associated with a key
  /*!
     \param what the key to set
     \param val  the value to store

     <b>Notes:</b>
        - If \c val is a <tt>const char *</tt>, it will be converted
           to a \c std::string for storage.
        - If the dictionary already contains the key \c what,
          the value will be replaced.
  */
  template <typename T>
  void setVal(std::string_view what, T &val) {
    static_assert(!std::is_same_v<T, std::string_view>,
                  "T cannot be string_view");
    _hasNonPodData = true;
    auto key = std::string(what);
    auto it = _data.find(key);
    if (it != _data.end()) {
      RDValue::cleanup_rdvalue(it->second);
      it->second = val;
    } else {
      _data.emplace(std::move(key), val);
    }
  }

  template <typename T>
  void setPODVal(std::string_view what, T val) {
    static_assert(!std::is_same_v<T, std::string_view>,
                  "T cannot be string_view");
    // don't change the hasNonPodData status
    auto key = std::string(what);
    auto it = _data.find(key);
    if (it != _data.end()) {
      RDValue::cleanup_rdvalue(it->second);
      it->second = val;
    } else {
      _data.emplace(std::move(key), val);
    }
  }

  void setVal(std::string_view what, bool val) { setPODVal(what, val); }
  void setVal(std::string_view what, double val) { setPODVal(what, val); }
  void setVal(std::string_view what, float val) { setPODVal(what, val); }
  void setVal(std::string_view what, int val) { setPODVal(what, val); }
  void setVal(std::string_view what, unsigned int val) { setPODVal(what, val); }

  [[deprecated(
      "this overload is unstable, it may be changed or removed without warning")]]
  void moveInsert(std::string_view what, RDValue val) {
    // ownership is transferred

    _hasNonPodData |= !unstable::is_pod(val);

    auto key = std::string(what);
    auto it = _data.find(key);

    if (it != _data.end()) {
      RDValue::cleanup_rdvalue(it->second);
      it->second = val;
    } else {
      _data.emplace(std::move(key), val);
    }

    val.type = RDTypeTag::EmptyTag;
  }

  //! \overload
  void setVal(std::string_view what, const char *val) {
    std::string h(val);
    setVal(what, h);
  }

  //----------------------------------------------------------
  //! \brief Clears the value associated with a particular key,
  //!     removing the key from the dictionary.
  void clearVal(std::string_view what) {
    auto it = _data.find(std::string(what));
    if (it != _data.end()) {
      if (_hasNonPodData) {
        RDValue::cleanup_rdvalue(it->second);
      }
      _data.erase(it);
    }
  }

  //----------------------------------------------------------
  //! \brief Clears all keys (and values) from the dictionary.
  void reset() {
    if (_hasNonPodData) {
      for (auto &kv : _data) {
        RDValue::cleanup_rdvalue(kv.second);
      }
    }
    _data.clear();
  }

  //----------------------------------------------------------
  //! \brief Updates this dictionary with all the keys and values from another
  //! Dict.
  //!        By default, it overwrites keys that already exist. If
  //!        preserveExisting is true, it does not overwrite existing keys.
  void update(const Dict &other, bool preserveExisting = false) {
    if (!preserveExisting) {
      *this = other;
    } else {
      if (other._hasNonPodData) {
        _hasNonPodData = true;
      }
      for (const auto &opair : other._data) {
        auto it = _data.find(opair.first);
        if (it == _data.end()) {
          // need to create blank entry and copy
          RDValue tmp;
          copy_rdvalue(tmp, opair.second);
          _data.emplace(opair.first, tmp);
        } else if (!preserveExisting) {
          RDValue::cleanup_rdvalue(it->second);
          copy_rdvalue(it->second, opair.second);
        }
      }
    }
  }

  [[deprecated(
      "this method is unstable, it maye be changed or removed without warning")]]
  unstable::DictConstIterableKeys iterKeys() const {
    return {std::begin(_data), std::end(_data)};
  }

  [[deprecated(
      "this method is unstable, it maye be changed or removed without warning")]]
  unstable::DictConstIterableValues iterValues() const {
    return {std::begin(_data), std::end(_data)};
  }

  [[deprecated(
      "this method is unstable, it maye be changed or removed without warning")]]
  unstable::DictConstIterableItems unstableItems() const {
    return {std::begin(_data), std::end(_data)};
  }

 private:
  DataType _data;              //!< the actual dictionary
  bool _hasNonPodData{false};  //!< if true, need a deep copy
};

template <>
inline std::string Dict::getVal<std::string>(std::string_view what) const {
  std::string res;
  getVal(what, res);
  return res;
}

// Utility class for holding a Dict::Pair
//  Dict::Pairs require containers for memory management
//  This utility class covers cleanup and copying
class PairHolder {
 public:
  // std::pair<std::string, RDValue> inner;
  std::string key;
  RDValue val;

  explicit PairHolder() {}

  explicit PairHolder(const std::pair<std::string, RDValue> &p) : key(p.first) {
    copy_rdvalue(val, p.second);
  }

  explicit PairHolder(std::pair<std::string, RDValue> &&p)
      : key(std::move(p.first)), val(std::move(p.second)) {
    p.second.type = RDTypeTag::EmptyTag;
  }

  ~PairHolder() { RDValue::cleanup_rdvalue(val); }
};

}  // namespace RDKit
#endif

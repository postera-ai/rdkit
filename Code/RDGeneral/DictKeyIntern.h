#include <RDGeneral/export.h>
#ifndef RD_DICT_KEY_INTERN_H
#define RD_DICT_KEY_INTERN_H

#include <cstdint>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace RDKit {

using DictKey = std::uint16_t;

class RDKIT_RDGENERAL_EXPORT DictKeyIntern {
 public:
  static DictKeyIntern &instance() {
    static DictKeyIntern inst;
    return inst;
  }

  DictKey intern(std::string_view s) {
    {
      std::shared_lock lock(d_mtx);
      auto it = d_str2key.find(s);
      if (it != d_str2key.end()) {
        return it->second;
      }
    }
    std::unique_lock lock(d_mtx);
    auto [it, inserted] = d_str2key.emplace(std::string(s), 0);
    if (inserted) {
      DictKey k = static_cast<DictKey>(d_key2str.size());
      it->second = k;
      d_key2str.push_back(it->first);
    }
    return it->second;
  }

  const std::string &toString(DictKey k) const { return d_key2str[k]; }

 private:
  DictKeyIntern() {
    d_str2key.reserve(256);
    d_key2str.reserve(256);
  }
  DictKeyIntern(const DictKeyIntern &) = delete;
  DictKeyIntern &operator=(const DictKeyIntern &) = delete;

  struct SVHash {
    using is_transparent = void;
    size_t operator()(std::string_view s) const {
      return std::hash<std::string_view>{}(s);
    }
    size_t operator()(const std::string &s) const {
      return std::hash<std::string_view>{}(std::string_view(s));
    }
  };
  struct SVEqual {
    using is_transparent = void;
    bool operator()(std::string_view a, std::string_view b) const {
      return a == b;
    }
  };

  std::shared_mutex d_mtx;
  std::unordered_map<std::string, DictKey, SVHash, SVEqual> d_str2key;
  std::vector<std::string> d_key2str;
};

inline DictKey internKey(std::string_view s) {
  return DictKeyIntern::instance().intern(s);
}
inline const std::string &keyToString(DictKey k) {
  return DictKeyIntern::instance().toString(k);
}

}  // namespace RDKit

#endif

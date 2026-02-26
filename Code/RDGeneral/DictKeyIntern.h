#include <RDGeneral/export.h>
#ifndef RD_DICT_KEY_INTERN_H
#define RD_DICT_KEY_INTERN_H

#include <array>
#include <cstdint>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace RDKit {

using DictKey = std::uint16_t;

inline constexpr std::string_view kWellKnownKeys[] = {
    "__computedProps",
    "2D",
    "BalabanJ",
    "BalanbanJ",
    "Discrims",
    "DistanceMatrix_Paths",
    "MolFileComments",
    "MolFileInfo",
    "NullBond",
    "_2DConf",
    "_3DConf",
    "_AtomID",
    "_BondsPotentialStereo",
    "_chiralAtomRank",
    "_CIPCode",
    "_CIPRank",
    "_CIPComputed",
    "_CIPNeighborOrder",
    "_CanonicalRankingNumber",
    "_ChiralityPossible",
    "_CrippenLogP",
    "_CrippenMR",
    "_MMFFSanitized",
    "_MolFileChiralFlag",
    "MRV SMA",
    "_MolFileRLabel",
    "_MolFileAtomQuery",
    "_MolFileBondQuery",
    "_MolFileBondEndPts",
    "_MolFileBondAttach",
    "_MolFileBondType",
    "_MolFileBondStereo",
    "_MolFileBondCfg",
    "_Name",
    "_NeedsQueryScan",
    "_NonExplicit3DChirality",
    "_QueryFormalCharge",
    "_QueryHCount",
    "_QueryIsotope",
    "_QueryMass",
    "_ReactionDegreeChanged",
    "react_atom_idx",
    "old_mapno",
    "react_idx",
    "_RingClosures",
    "_SLN_s",
    "_SmilesStart",
    "_StereochemDone",
    "_TraversalBondIndexOrder",
    "_TraversalRingClosureBond",
    "_TraversalStartPoint",
    "_TriposAtomType",
    "_Unfinished_SLN_",
    "_UnknownStereo",
    "_connectivityHKDeltas",
    "_connectivityNVals",
    "_crippenLogP",
    "_crippenLogPContribs",
    "_crippenMR",
    "_crippenMRContribs",
    "_GasteigerCharge",
    "_GasteigerHCharge",
    "_doIsoSmiles",
    "_fragSMARTS",
    "_hasMassQuery",
    "_labuteASA",
    "_labuteAtomContribs",
    "_labuteAtomHContrib",
    "_protected",
    "_queryRootAtom",
    "_ringStereoAtoms",
    "_ringStereoWarning",
    "_ringStereochemCand",
    "_ringStereoOtherAtom",
    "_mesoOtherAtom",
    "_chiralPermutation",
    "_smilesAtomOutputOrder",
    "_smilesBondOutputOrder",
    "_starred",
    "_supplementalSmilesLabel",
    "_tpsa",
    "_tpsaAtomContribs",
    "_unspecifiedOrder",
    "_brokenChirality",
    "_rgroupAtomMaps",
    "_rgroupBonds",
    "_rgroupTargetAtoms",
    "_rgroupTargetBonds",
    "dummyLabel",
    "extraRings",
    "isImplicit",
    "maxAttachIdx",
    "molAtomMapNumber",
    "molFileAlias",
    "molFileValue",
    "molInversionFlag",
    "molParity",
    "molStereoCare",
    "molRxnComponent",
    "molRxnRole",
    "molTotValence",
    "_molLinkNodes",
    "numArom",
    "ringMembership",
    "smilesSymbol",
    "atomLabel",
    "OxidationNumber",
    "internalRgroupSmiles",
    "molRingBondCount",
    "molSubstCount",
    "molAttchpt",
    "molAttchord",
    "molAttachOrderTemplate",
    "molClass",
    "molSeqid",
    "molSeqName",
    "molRxnExachg",
    "molReactStatus",
    "_fromAttchpt",
    "natReplace",
    "templateNames",
    "molNote",
    "atomNote",
    "bondNote",
    "_isotopicHs",
    "_QueryAtomGenericLabel",
    "_displayLabel",
    "_displayLabelW",
    "_cxsmilesBondIdx",
    "_cxsmilesOutputIndex",
    "_needsDetectAtomStereo",
    "_needsDetectBondStereo",
    "_potentialStereo",
    "_stereoGroup",
};
inline constexpr size_t kNumWellKnown =
    sizeof(kWellKnownKeys) / sizeof(kWellKnownKeys[0]);

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
    auto it = d_str2key.find(s);
    if (it != d_str2key.end()) {
      return it->second;
    }
    DictKey k = static_cast<DictKey>(d_key2str.size());
    auto [ins, _] = d_str2key.emplace(std::string(s), k);
    d_key2str.push_back(ins->first);
    return k;
  }

  const std::string &toString(DictKey k) const { return d_key2str[k]; }

 private:
  DictKeyIntern() {
    d_str2key.reserve(256);
    d_key2str.reserve(256);
    for (size_t i = 0; i < kNumWellKnown; ++i) {
      auto [ins, _] = d_str2key.emplace(std::string(kWellKnownKeys[i]),
                                         static_cast<DictKey>(i));
      d_key2str.push_back(ins->first);
    }
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

  mutable std::shared_mutex d_mtx;
  std::unordered_map<std::string, DictKey, SVHash, SVEqual> d_str2key;
  std::vector<std::string> d_key2str;
};

inline DictKey internKey(std::string_view s) {
  struct CacheEntry {
    std::string key;
    DictKey val{0};
  };
  static constexpr size_t kCacheSize = 64;
  static thread_local std::array<CacheEntry, kCacheSize> tl_cache{};

  size_t h = std::hash<std::string_view>{}(s) % kCacheSize;
  auto &entry = tl_cache[h];
  if (entry.key.size() == s.size() && entry.key == s) {
    return entry.val;
  }
  DictKey k = DictKeyIntern::instance().intern(s);
  entry.key.assign(s.data(), s.size());
  entry.val = k;
  return k;
}
inline const std::string &keyToString(DictKey k) {
  return DictKeyIntern::instance().toString(k);
}

}  // namespace RDKit

#endif

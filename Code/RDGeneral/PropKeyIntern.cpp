//
// Copyright (C) 2026 Greg Landrum and other RDKit contributors
//
//  @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//
#include "PropKeyIntern.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <string_view>
#include <unordered_map>

namespace RDKit {

namespace {

// Sorted array of all interned property key strings.
// clang-format off
constexpr std::string_view kKeys[] = {
    "2D",
    "BalabanJ",
    "BalanbanJ",
    "Discrims",
    "DistanceMatrix_Paths",
    "MRV SMA",
    "MolFileComments",
    "MolFileInfo",
    "NullBond",
    "OxidationNumber",
    "__computedProps",
    "_2DConf",
    "_3DConf",
    "_AtomID",
    "_BondsPotentialStereo",
    "_CIPCode",
    "_CIPComputed",
    "_CIPNeighborOrder",
    "_CIPRank",
    "_CanonicalRankingNumber",
    "_ChiralityPossible",
    "_CrippenLogP",
    "_CrippenMR",
    "_GasteigerCharge",
    "_GasteigerHCharge",
    "_MMFFSanitized",
    "_MolFileAtomQuery",
    "_MolFileBondAttach",
    "_MolFileBondCfg",
    "_MolFileBondEndPts",
    "_MolFileBondQuery",
    "_MolFileBondStereo",
    "_MolFileBondType",
    "_MolFileChiralFlag",
    "_MolFileRLabel",
    "_Name",
    "_NeedsQueryScan",
    "_NonExplicit3DChirality",
    "_QueryAtomGenericLabel",
    "_QueryFormalCharge",
    "_QueryHCount",
    "_QueryIsotope",
    "_QueryMass",
    "_ReactionDegreeChanged",
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
    "_brokenChirality",
    "_chiralAtomRank",
    "_chiralPermutation",
    "_connectivityHKDeltas",
    "_connectivityNVals",
    "_crippenLogP",
    "_crippenLogPContribs",
    "_crippenMR",
    "_crippenMRContribs",
    "_displayLabel",
    "_displayLabelW",
    "_doIsoSmiles",
    "_fragSMARTS",
    "_fromAttchpt",
    "_hasMassQuery",
    "_isotopicHs",
    "_labuteASA",
    "_labuteAtomContribs",
    "_labuteAtomHContrib",
    "_mesoOtherAtom",
    "_molLinkNodes",
    "_protected",
    "_queryRootAtom",
    "_rgroupAtomMaps",
    "_rgroupBonds",
    "_rgroupTargetAtoms",
    "_rgroupTargetBonds",
    "_ringStereoAtoms",
    "_ringStereoOtherAtom",
    "_ringStereoWarning",
    "_ringStereochemCand",
    "_smilesAtomOutputOrder",
    "_smilesBondOutputOrder",
    "_starred",
    "_supplementalSmilesLabel",
    "_tpsa",
    "_tpsaAtomContribs",
    "_unspecifiedOrder",
    "atomLabel",
    "atomNote",
    "bondNote",
    "dummyLabel",
    "extraRings",
    "internalRgroupSmiles",
    "isImplicit",
    "maxAttachIdx",
    "molAtomMapNumber",
    "molAttachOrderTemplate",
    "molAttchord",
    "molAttchpt",
    "molClass",
    "molFileAlias",
    "molFileValue",
    "molInversionFlag",
    "molNote",
    "molParity",
    "molReactStatus",
    "molRingBondCount",
    "molRxnComponent",
    "molRxnExachg",
    "molRxnRole",
    "molSeqName",
    "molSeqid",
    "molStereoCare",
    "molSubstCount",
    "molTotValence",
    "natReplace",
    "numArom",
    "old_mapno",
    "react_atom_idx",
    "react_idx",
    "ringMembership",
    "smilesSymbol",
    "templateNames",
};
// clang-format on

constexpr std::size_t kNumKeys = sizeof(kKeys) / sizeof(kKeys[0]);
static_assert(kNumKeys == 128, "expected 128 interned keys");
static_assert(kNumKeys <= kInvalidPropKey,
              "too many keys for PropKeyId width");

// Pointer-identity lookup: map from string_view .data() pointer to ID.
// Most callers pass constexpr string_view from common_properties,
// so .data() is a fixed compile-time address. One probe into this
// map costs ~2ns vs ~8ns for the unordered_map fallback.
struct PtrMap {
  std::unordered_map<const char *, PropKeyId> ptrMap;
  std::unordered_map<std::string_view, PropKeyId> strMap;

  PtrMap() {
    ptrMap.reserve(kNumKeys * 2);
    strMap.reserve(kNumKeys);
    for (std::size_t i = 0; i < kNumKeys; ++i) {
      ptrMap.emplace(kKeys[i].data(), static_cast<PropKeyId>(i));
      strMap.emplace(kKeys[i], static_cast<PropKeyId>(i));
    }
  }
};

const PtrMap &getPtrMap() {
  static const PtrMap instance;
  return instance;
}

}  // namespace

std::optional<PropKeyId> internPropKey(std::string_view key) {
  const auto &maps = getPtrMap();
  // Fast path: pointer identity (constexpr string_view from common_properties)
  auto pit = maps.ptrMap.find(key.data());
  if (pit != maps.ptrMap.end() && kKeys[pit->second].size() == key.size()) {
    return pit->second;
  }
  // Slow path: content-based lookup (dynamic strings)
  auto sit = maps.strMap.find(key);
  if (sit != maps.strMap.end()) {
    return sit->second;
  }
  return std::nullopt;
}

std::string_view propKeyToString(PropKeyId id) {
  assert(id < kNumKeys);
  return kKeys[id];
}

std::size_t numInternedKeys() { return kNumKeys; }

}  // namespace RDKit

//
// Copyright (C) 2026 Greg Landrum and other RDKit contributors
//
//  @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//
#pragma once

#include <RDGeneral/export.h>
#include <cstdint>
#include <optional>
#include <string_view>

namespace RDKit {

using PropKeyId = std::uint16_t;
inline constexpr PropKeyId kInvalidPropKey = 0xFFFF;

RDKIT_RDGENERAL_EXPORT std::optional<PropKeyId> internPropKey(
    std::string_view key);

RDKIT_RDGENERAL_EXPORT std::string_view propKeyToString(PropKeyId id);

RDKIT_RDGENERAL_EXPORT std::size_t numInternedKeys();

}  // namespace RDKit

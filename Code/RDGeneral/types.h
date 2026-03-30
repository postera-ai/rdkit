//
//  Copyright 2001-2021 Greg Landrum and other RDKit contributors
//
//  @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//

#include <RDGeneral/export.h>
#ifndef RD_TYPES_H
#define RD_TYPES_H

#ifdef WIN32
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#define _DEFINED_USE_MATH_DEFINES
#endif
#endif
#include <cmath>
#ifdef _DEFINED_USE_MATH_DEFINES
#undef _DEFINED_USE_MATH_DEFINES
#undef _USE_MATH_DEFINES
#endif

#include "Invariant.h"
#include "Dict.h"

#include <vector>
#include <deque>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <list>
#include <limits>

#include <cstring>
#include <any>
#include <RDGeneral/BoostStartInclude.h>
#include <boost/lexical_cast.hpp>
#include <RDGeneral/BoostEndInclude.h>

namespace RDKit {

namespace detail {
#ifndef SWIG
inline constexpr DictKey computedPropName = 0;
#endif
}  // namespace detail

#ifndef SWIG
namespace common_properties {
inline constexpr DictKey TWOD = 1;
inline constexpr DictKey BalabanJ = 2;
inline constexpr DictKey BalanbanJ = 3;
inline constexpr DictKey Discrims = 4;
inline constexpr DictKey DistanceMatrix_Paths = 5;
inline constexpr DictKey MolFileComments = 6;
inline constexpr DictKey MolFileInfo = 7;
inline constexpr DictKey NullBond = 8;
inline constexpr DictKey _2DConf = 9;
inline constexpr DictKey _3DConf = 10;
inline constexpr DictKey _AtomID = 11;
inline constexpr DictKey _BondsPotentialStereo = 12;
inline constexpr DictKey _ChiralAtomRank = 13;
inline constexpr DictKey _CIPCode = 14;
inline constexpr DictKey _CIPRank = 15;
inline constexpr DictKey _CIPComputed = 16;
inline constexpr DictKey _CIPNeighborOrder = 17;
inline constexpr DictKey _CanonicalRankingNumber = 18;
inline constexpr DictKey _ChiralityPossible = 19;
inline constexpr DictKey _CrippenLogP = 20;
inline constexpr DictKey _CrippenMR = 21;
inline constexpr DictKey _MMFFSanitized = 22;
inline constexpr DictKey _MolFileChiralFlag = 23;
inline constexpr DictKey MRV_SMA = 24;
inline constexpr DictKey _MolFileRLabel = 25;
inline constexpr DictKey _MolFileAtomQuery = 26;
inline constexpr DictKey _MolFileBondQuery = 27;
inline constexpr DictKey _MolFileBondEndPts = 28;
inline constexpr DictKey _MolFileBondAttach = 29;
inline constexpr DictKey _MolFileBondType = 30;
inline constexpr DictKey _MolFileBondStereo = 31;
inline constexpr DictKey _MolFileBondCfg = 32;
inline constexpr DictKey _Name = 33;
inline constexpr DictKey _NeedsQueryScan = 34;
inline constexpr DictKey _NonExplicit3DChirality = 35;
inline constexpr DictKey _QueryFormalCharge = 36;
inline constexpr DictKey _QueryHCount = 37;
inline constexpr DictKey _QueryIsotope = 38;
inline constexpr DictKey _QueryMass = 39;
inline constexpr DictKey _ReactionDegreeChanged = 40;
inline constexpr DictKey reactantAtomIdx = 41;
inline constexpr DictKey reactionMapNum = 42;
inline constexpr DictKey reactantIdx = 43;
inline constexpr DictKey _RingClosures = 44;
inline constexpr DictKey _SLN_s = 45;
inline constexpr DictKey _SmilesStart = 46;
inline constexpr DictKey _StereochemDone = 47;
inline constexpr DictKey _TraversalBondIndexOrder = 48;
inline constexpr DictKey _TraversalRingClosureBond = 49;
inline constexpr DictKey _TraversalStartPoint = 50;
inline constexpr DictKey _TriposAtomType = 51;
inline constexpr DictKey _Unfinished_SLN_ = 52;
inline constexpr DictKey _UnknownStereo = 53;
inline constexpr DictKey _connectivityHKDeltas = 54;
inline constexpr DictKey _connectivityNVals = 55;
inline constexpr DictKey _crippenLogP = 56;
inline constexpr DictKey _crippenLogPContribs = 57;
inline constexpr DictKey _crippenMR = 58;
inline constexpr DictKey _crippenMRContribs = 59;
inline constexpr DictKey _GasteigerCharge = 60;
inline constexpr DictKey _GasteigerHCharge = 61;
inline constexpr DictKey _doIsoSmiles = 62;
inline constexpr DictKey _fragSMARTS = 63;
inline constexpr DictKey _hasMassQuery = 64;
inline constexpr DictKey _labuteASA = 65;
inline constexpr DictKey _labuteAtomContribs = 66;
inline constexpr DictKey _labuteAtomHContrib = 67;
inline constexpr DictKey _protected = 68;
inline constexpr DictKey _queryRootAtom = 69;
inline constexpr DictKey _ringStereoAtoms = 70;
inline constexpr DictKey _ringStereoWarning = 71;
inline constexpr DictKey _ringStereochemCand = 72;
inline constexpr DictKey _ringStereoOtherAtom = 73;
inline constexpr DictKey _mesoOtherAtom = 74;
inline constexpr DictKey _chiralPermutation = 75;
inline constexpr DictKey _smilesAtomOutputOrder = 76;
inline constexpr DictKey _smilesBondOutputOrder = 77;
inline constexpr DictKey _starred = 78;
inline constexpr DictKey _supplementalSmilesLabel = 79;
inline constexpr DictKey _tpsa = 80;
inline constexpr DictKey _tpsaAtomContribs = 81;
inline constexpr DictKey _unspecifiedOrder = 82;
inline constexpr DictKey _brokenChirality = 83;
inline constexpr DictKey _rgroupAtomMaps = 84;
inline constexpr DictKey _rgroupBonds = 85;
inline constexpr DictKey _rgroupTargetAtoms = 86;
inline constexpr DictKey _rgroupTargetBonds = 87;
inline constexpr DictKey dummyLabel = 88;
inline constexpr DictKey extraRings = 89;
inline constexpr DictKey isImplicit = 90;
inline constexpr DictKey maxAttachIdx = 91;
inline constexpr DictKey molAtomMapNumber = 92;
inline constexpr DictKey molFileAlias = 93;
inline constexpr DictKey molFileValue = 94;
inline constexpr DictKey molInversionFlag = 95;
inline constexpr DictKey molParity = 96;
inline constexpr DictKey molStereoCare = 97;
inline constexpr DictKey molRxnComponent = 98;
inline constexpr DictKey molRxnRole = 99;
inline constexpr DictKey molTotValence = 100;
inline constexpr DictKey molFileLinkNodes = 101;
inline constexpr DictKey numArom = 102;
inline constexpr DictKey ringMembership = 103;
inline constexpr DictKey smilesSymbol = 104;
inline constexpr DictKey atomLabel = 105;
inline constexpr DictKey OxidationNumber = 106;
inline constexpr DictKey internalRgroupSmiles = 107;
inline constexpr DictKey molRingBondCount = 108;
inline constexpr DictKey molSubstCount = 109;
inline constexpr DictKey molAttachPoint = 110;
inline constexpr DictKey molAttachOrder = 111;
inline constexpr DictKey molAttachOrderTemplate = 112;
inline constexpr DictKey molAtomClass = 113;
inline constexpr DictKey molAtomSeqId = 114;
inline constexpr DictKey molAtomSeqName = 115;
inline constexpr DictKey molRxnExactChange = 116;
inline constexpr DictKey molReactStatus = 117;
inline constexpr DictKey _fromAttachPoint = 118;
inline constexpr DictKey natReplace = 119;
inline constexpr DictKey templateNames = 120;
inline constexpr DictKey molNote = 121;
inline constexpr DictKey atomNote = 122;
inline constexpr DictKey bondNote = 123;
inline constexpr DictKey _isotopicHs = 124;
inline constexpr DictKey _QueryAtomGenericLabel = 125;
inline constexpr DictKey _displayLabel = 126;
inline constexpr DictKey _displayLabelW = 127;
inline constexpr DictKey _cxsmilesBondIdx = 128;
inline constexpr DictKey _cxsmilesOutputIndex = 129;
inline constexpr DictKey _needsDetectAtomStereo = 130;
inline constexpr DictKey _needsDetectBondStereo = 131;
inline constexpr DictKey _potentialStereo = 132;
inline constexpr DictKey _stereoGroup = 133;
}  // namespace common_properties
#endif  // SWIG
#ifndef WIN32
typedef long long int LONGINT;
#else
typedef __int64 LONGINT;
#endif
#ifdef max
#undef max  // FUCK I hate this nonsense
#endif
#ifdef min
#undef min  // FUCK I hate this nonsense
#endif

RDKIT_RDGENERAL_EXPORT extern const double MAX_DOUBLE;
RDKIT_RDGENERAL_EXPORT extern const double EPS_DOUBLE;
RDKIT_RDGENERAL_EXPORT extern const double SMALL_DOUBLE;
RDKIT_RDGENERAL_EXPORT extern const double MAX_INT;
RDKIT_RDGENERAL_EXPORT extern const double MAX_LONGINT;

typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;

typedef std::vector<int> INT_VECT;
typedef INT_VECT::iterator INT_VECT_I;
typedef INT_VECT::const_iterator INT_VECT_CI;
typedef INT_VECT::reverse_iterator INT_VECT_RI;
typedef INT_VECT::const_reverse_iterator INT_VECT_CRI;

typedef std::list<int> INT_LIST;
typedef INT_LIST::iterator INT_LIST_I;
typedef INT_LIST::const_iterator INT_LIST_CI;

typedef std::list<INT_VECT> LIST_INT_VECT;
typedef LIST_INT_VECT::iterator LIST_INT_VECT_I;
typedef LIST_INT_VECT::const_iterator LIST_INT_VECT_CI;

typedef std::vector<INT_VECT> VECT_INT_VECT;
typedef VECT_INT_VECT::iterator VECT_INT_VECT_I;
typedef VECT_INT_VECT::const_iterator VECT_INT_VECT_CI;

typedef std::vector<UINT>::const_iterator UINT_VECT_CI;
typedef std::vector<UINT> UINT_VECT;

typedef std::vector<std::string>::const_iterator STR_VECT_CI;
typedef std::vector<std::string>::iterator STR_VECT_I;
typedef std::vector<std::string> STR_VECT;

typedef std::vector<double> DOUBLE_VECT;
typedef DOUBLE_VECT::iterator DOUBLE_VECT_I;
typedef DOUBLE_VECT::const_iterator DOUBLE_VECT_CI;
typedef std::vector<DOUBLE_VECT> VECT_DOUBLE_VECT;
typedef VECT_DOUBLE_VECT::iterator VECT_DOUBLE_VECT_I;
typedef VECT_DOUBLE_VECT::const_iterator VECT_DOUBLE_VECT_CI;

typedef std::map<std::string, UINT> STR_UINT_MAP;
typedef std::map<std::string, UINT>::const_iterator STR_UINT_MAP_CI;

typedef std::map<int, INT_VECT> INT_INT_VECT_MAP;
typedef INT_INT_VECT_MAP::const_iterator INT_INT_VECT_MAP_CI;

typedef std::map<int, int> INT_MAP_INT;
typedef INT_MAP_INT::iterator INT_MAP_INT_I;
typedef INT_MAP_INT::const_iterator INT_MAP_INT_CI;

typedef std::deque<int> INT_DEQUE;
typedef INT_DEQUE::iterator INT_DEQUE_I;
typedef INT_DEQUE::const_iterator INT_DEQUE_CI;

typedef std::map<int, INT_DEQUE> INT_INT_DEQ_MAP;
typedef INT_INT_DEQ_MAP::const_iterator INT_INT_DEQ_MAP_CI;

typedef std::set<int> INT_SET;
typedef INT_SET::iterator INT_SET_I;
typedef INT_SET::const_iterator INT_SET_CI;

//! functor to compare two doubles with a tolerance
struct RDKIT_RDGENERAL_EXPORT ltDouble {
 public:
  ltDouble() {}
  bool operator()(double d1, double d2) const {
    if (fabs(d1 - d2) < _tol) {
      return false;
    } else {
      return (d1 < d2);
    }
  }

 private:
  double _tol{1.0e-8};
};

//! std::map from double to integer.
typedef std::map<double, int, ltDouble> DOUBLE_INT_MAP;

//! functor for returning the larger of two values
template <typename T>
struct RDKIT_RDGENERAL_EXPORT larger_of {
  T operator()(T arg1, T arg2) { return arg1 > arg2 ? arg1 : arg2; }
};

//! functor for comparing two strings
struct RDKIT_RDGENERAL_EXPORT charptr_functor {
  bool operator()(const char *s1, const char *s2) const {
    return strcmp(s1, s2) < 0;
  }
};

//! \brief calculate the union of two INT_VECTs and put the results in a
//! third vector
RDKIT_RDGENERAL_EXPORT void Union(const INT_VECT &r1, const INT_VECT &r2,
                                  INT_VECT &res);

//! \brief calculate the intersection of two INT_VECTs and put the results in a
//! third vector
RDKIT_RDGENERAL_EXPORT void Intersect(const INT_VECT &r1, const INT_VECT &r2,
                                      INT_VECT &res);

//! calculating the union of the INT_VECT's in a VECT_INT_VECT
/*!
    \param rings   the INT_VECT's to consider
    \param res     used to return results
    \param exclude any values in this optional INT_VECT will be excluded
           from the union.
*/
RDKIT_RDGENERAL_EXPORT void Union(const VECT_INT_VECT &rings, INT_VECT &res,
                                  const INT_VECT *exclude = nullptr);

//! given a current combination of numbers change it to the next possible
// combination
/*!
  \param comb the <b>sorted</b> vector to consider
  \param tot the maximum number possible in the vector

  \return -1 on failure, the index of the last number changed on success.
  Example:
    for all combinations 3 of numbers between 0 and tot=5
    given (0,1,2) the function wil return (0,1,3) etc.


*/
RDKIT_RDGENERAL_EXPORT int nextCombination(INT_VECT &comb, int tot);
};  // namespace RDKit

#endif

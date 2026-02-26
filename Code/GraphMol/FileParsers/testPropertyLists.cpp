//
//  Copyright (C) 2019-2025 Greg Landrum and other RDKit contributors
//   @@ All Rights Reserved @@
//  This file is part of the RDKit.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKit source tree.
//

#include "RDGeneral/test.h"
#include <catch2/catch_all.hpp>

#include <GraphMol/RDKitBase.h>
#include <GraphMol/FileParsers/FileParsers.h>
#include <GraphMol/FileParsers/FileParserUtils.h>
#include <GraphMol/FileParsers/MolSupplier.h>
#include <GraphMol/SmilesParse/SmilesParse.h>

using namespace RDKit;

TEST_CASE("Property list conversion") {
  auto m = "COC"_smiles;
  REQUIRE(m);
  auto getter = [&m](size_t which) { return m->getAtomWithIdx(which); };
  SECTION("basics: iprops") {
    m->setProp(internKey("atom.iprop.foo1"), "1   6 9");
    m->setProp(internKey("atom.iprop.foo2"), "3 n/a   9");
    m->setProp(internKey("atom.iprop.foo3"), "[?]  5 1 ?");
    m->setProp(internKey("atom.iprop.foo4"), "[foo] 3 foo   9");
    FileParserUtils::applyMolListProps<int>(*m, "atom.iprop.", m->getNumAtoms(),
                                            getter);
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(1)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(2)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo2")));
    CHECK(!m->getAtomWithIdx(1)->hasProp(internKey("foo2")));
    CHECK(m->getAtomWithIdx(2)->hasProp(internKey("foo2")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo3")));
    CHECK(m->getAtomWithIdx(1)->hasProp(internKey("foo3")));
    CHECK(!m->getAtomWithIdx(2)->hasProp(internKey("foo3")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo4")));
    CHECK(!m->getAtomWithIdx(1)->hasProp(internKey("foo4")));
    CHECK(m->getAtomWithIdx(2)->hasProp(internKey("foo4")));
    CHECK(m->getAtomWithIdx(1)->getProp<int>(internKey("foo1")) == 6);
    CHECK(m->getAtomWithIdx(2)->getProp<int>(internKey("foo2")) == 9);
    CHECK(m->getAtomWithIdx(1)->getProp<int>(internKey("foo3")) == 1);
    CHECK(m->getAtomWithIdx(1)->getProp<std::string>(internKey("foo3")) == "1");
  }
  SECTION("basics: dprops") {
    m->setProp(internKey("atom.dprop.foo1"), "1   6 9");
    m->setProp(internKey("atom.dprop.foo2"), "3 n/a   9");
    m->setProp(internKey("atom.dprop.foo3"), "[?]  5 1 ?");
    FileParserUtils::applyMolListProps<double>(*m, "atom.dprop.",
                                               m->getNumAtoms(), getter);
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(1)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(2)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo2")));
    CHECK(!m->getAtomWithIdx(1)->hasProp(internKey("foo2")));
    CHECK(m->getAtomWithIdx(2)->hasProp(internKey("foo2")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo3")));
    CHECK(m->getAtomWithIdx(1)->hasProp(internKey("foo3")));
    CHECK(!m->getAtomWithIdx(2)->hasProp(internKey("foo3")));
    CHECK(m->getAtomWithIdx(1)->getProp<double>(internKey("foo1")) == 6);
    CHECK(m->getAtomWithIdx(2)->getProp<double>(internKey("foo2")) == 9);
    CHECK(m->getAtomWithIdx(1)->getProp<double>(internKey("foo3")) == 1);
  }
  SECTION("basics: props") {
    m->setProp(internKey("atom.prop.foo1"), "1   6 9");
    m->setProp(internKey("atom.prop.foo2"), "3 n/a   9");
    m->setProp(internKey("atom.prop.foo3"), "[?]  5 1 ?");
    FileParserUtils::applyMolListProps<std::string>(*m, "atom.prop.",
                                                    m->getNumAtoms(), getter);
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(1)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(2)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo2")));
    CHECK(!m->getAtomWithIdx(1)->hasProp(internKey("foo2")));
    CHECK(m->getAtomWithIdx(2)->hasProp(internKey("foo2")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo3")));
    CHECK(m->getAtomWithIdx(1)->hasProp(internKey("foo3")));
    CHECK(!m->getAtomWithIdx(2)->hasProp(internKey("foo3")));
    CHECK(m->getAtomWithIdx(1)->getProp<std::string>(internKey("foo1")) == "6");
    CHECK(m->getAtomWithIdx(2)->getProp<std::string>(internKey("foo2")) == "9");
    CHECK(m->getAtomWithIdx(1)->getProp<std::string>(internKey("foo3")) == "1");
  }
  SECTION("basics: bprops") {
    m->setProp(internKey("atom.bprop.foo1"), "1   0 0");
    m->setProp(internKey("atom.bprop.foo2"), "0 n/a   1");
    m->setProp(internKey("atom.bprop.foo3"), "[?]  0 1 ?");
    FileParserUtils::applyMolListProps<bool>(*m, "atom.bprop.",
                                             m->getNumAtoms(), getter);
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(1)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(2)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo2")));
    CHECK(!m->getAtomWithIdx(1)->hasProp(internKey("foo2")));
    CHECK(m->getAtomWithIdx(2)->hasProp(internKey("foo2")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo3")));
    CHECK(m->getAtomWithIdx(1)->hasProp(internKey("foo3")));
    CHECK(!m->getAtomWithIdx(2)->hasProp(internKey("foo3")));
    CHECK(m->getAtomWithIdx(1)->getProp<bool>(internKey("foo1")) == false);
    CHECK(m->getAtomWithIdx(2)->getProp<bool>(internKey("foo2")) == true);
    CHECK(m->getAtomWithIdx(1)->getProp<bool>(internKey("foo3")) == true);
  }
  SECTION("basics: bond props") {
    auto bgetter = [&m](size_t which) { return m->getBondWithIdx(which); };

    m->setProp(internKey("bond.prop.foo1"), "1   6");
    m->setProp(internKey("bond.prop.foo2"), "3 n/a");
    m->setProp(internKey("bond.prop.foo3"), "[?]  ? 5");
    FileParserUtils::applyMolListProps<std::string>(*m, "bond.prop.",
                                                    m->getNumBonds(), bgetter);
    CHECK(m->getBondWithIdx(0)->hasProp(internKey("foo1")));
    CHECK(m->getBondWithIdx(1)->hasProp(internKey("foo1")));
    CHECK(m->getBondWithIdx(0)->hasProp(internKey("foo2")));
    CHECK(!m->getBondWithIdx(1)->hasProp(internKey("foo2")));
    CHECK(!m->getBondWithIdx(0)->hasProp(internKey("foo3")));
    CHECK(m->getBondWithIdx(1)->hasProp(internKey("foo3")));
    CHECK(m->getBondWithIdx(1)->getProp<std::string>(internKey("foo1")) == "6");
    CHECK(m->getBondWithIdx(1)->getProp<std::string>(internKey("foo3")) == "5");
  }
}
TEST_CASE("processMolPropertyLists") {
  SECTION("basics") {
    auto m = "COC"_smiles;
    REQUIRE(m);
    m->setProp(internKey("atom.iprop.foo1"), "1   6 9");
    m->setProp(internKey("atom.dprop.foo2"), "3 n/a   9");
    m->setProp(internKey("atom.prop.foo3"), "[?]  5 1 ?");
    m->setProp(internKey("atom.bprop.foo4"), "1 0 0");
    m->setProp(internKey("bond.prop.foo2"), "3 7");
    FileParserUtils::processMolPropertyLists(*m);
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(1)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(2)->hasProp(internKey("foo1")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo2")));
    CHECK(!m->getAtomWithIdx(1)->hasProp(internKey("foo2")));
    CHECK(m->getAtomWithIdx(2)->hasProp(internKey("foo2")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo3")));
    CHECK(m->getAtomWithIdx(1)->hasProp(internKey("foo3")));
    CHECK(!m->getAtomWithIdx(2)->hasProp(internKey("foo3")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("foo4")));
    CHECK(m->getAtomWithIdx(1)->hasProp(internKey("foo4")));
    CHECK(m->getAtomWithIdx(2)->hasProp(internKey("foo4")));
    CHECK(m->getAtomWithIdx(1)->getProp<int>(internKey("foo1")) == 6);
    CHECK(m->getAtomWithIdx(2)->getProp<double>(internKey("foo2")) == 9);
    CHECK(m->getAtomWithIdx(1)->getProp<std::string>(internKey("foo3")) == "1");
    CHECK(m->getAtomWithIdx(1)->getProp<bool>(internKey("foo4")) == false);

    CHECK(m->getBondWithIdx(0)->hasProp(internKey("foo2")));
    CHECK(m->getBondWithIdx(1)->hasProp(internKey("foo2")));
    CHECK(m->getBondWithIdx(0)->getProp<int>(internKey("foo2")));
    CHECK(m->getBondWithIdx(1)->getProp<int>(internKey("foo2")));
  }
}

TEST_CASE("basic SDF handling") {
  std::string sdf = R"SDF(
     RDKit  2D

  3  3  0  0  0  0  0  0  0  0999 V2000
    0.8660    0.0000    0.0000 C   0  0  0  0  0  0  0  0  0  0  0  0
   -0.4330    0.7500    0.0000 N   0  0  0  0  0  0  0  0  0  0  0  0
   -0.4330   -0.7500    0.0000 C   0  0  0  0  0  0  0  0  0  0  0  0
  1  2  1  0
  2  3  1  0
  3  1  1  0
M  END
>  <atom.dprop.PartialCharge>  (1) 
0.008 -0.314 0.008

>  <atom.iprop.NumHeavyNeighbors>  (1) 
2 2 2

>  <atom.prop.AtomLabel>  (1)
C1 N2 C3

>  <atom.bprop.IsCarbon>  (1) 
1 0 1

>  <atom.prop.PartiallyMissing>  (1) 
one n/a three

>  <atom.iprop.PartiallyMissingInt>  (1) 
[?] 2 2 ?

>  <bond.prop.foo>  (1)
bar baz blah

> <atom.iprop.TooLong>  (1) 
0 1 2 3 4

$$$$
)SDF";
  SECTION("no processing") {
    RDKit::SDMolSupplier suppl;
    suppl.setData(sdf);
    suppl.setProcessPropertyLists(false);
    std::unique_ptr<RDKit::ROMol> m(suppl[0]);
    REQUIRE(m);
    CHECK(m->hasProp(internKey("atom.prop.AtomLabel")));
    CHECK(!m->getAtomWithIdx(0)->hasProp(internKey("AtomLabel")));
  }
  SECTION("with processing") {
    RDKit::SDMolSupplier suppl;
    suppl.setData(sdf);
    std::unique_ptr<RDKit::ROMol> m(suppl[0]);
    REQUIRE(m);
    CHECK(m->hasProp(internKey("atom.prop.AtomLabel")));
    CHECK(m->getAtomWithIdx(0)->hasProp(internKey("AtomLabel")));
    CHECK(m->hasProp(internKey("bond.prop.foo")));
    std::string val;
    CHECK(m->getBondWithIdx(0)->getPropIfPresent(internKey("foo"), val));
    CHECK(val == "bar");

    CHECK(m->hasProp(internKey("atom.iprop.TooLong")));
    CHECK(!m->getAtomWithIdx(0)->hasProp(internKey("TooLong")));
  }
}

TEST_CASE("createAtomPropertyLists") {
  SECTION("basics") {
    auto m = "COC"_smiles;
    REQUIRE(m);
    m->getAtomWithIdx(0)->setProp<int>(internKey("foo1"), 1);
    m->getAtomWithIdx(2)->setProp<int>(internKey("foo1"), 9);
    FileParserUtils::createAtomIntPropertyList(*m, "foo1");
    REQUIRE(m->hasProp(internKey("atom.iprop.foo1")));
    CHECK(m->getProp<std::string>(internKey("atom.iprop.foo1")) == "1 n/a 9");

    m->getAtomWithIdx(0)->setProp<double>(internKey("foo2"), 1);
    m->getAtomWithIdx(1)->setProp<double>(internKey("foo2"), 4);
    m->getAtomWithIdx(2)->setProp<double>(internKey("foo2"), 9);
    FileParserUtils::createAtomDoublePropertyList(*m, "foo2");
    REQUIRE(m->hasProp(internKey("atom.dprop.foo2")));
    CHECK(m->getProp<std::string>(internKey("atom.dprop.foo2")) == "1 4 9");

    m->getAtomWithIdx(0)->setProp<std::string>(internKey("foo3"), "1");
    m->getAtomWithIdx(1)->setProp<std::string>(internKey("foo3"), "4");
    FileParserUtils::createAtomStringPropertyList(*m, "foo3", "?");
    REQUIRE(m->hasProp(internKey("atom.prop.foo3")));
    CHECK(m->getProp<std::string>(internKey("atom.prop.foo3")) == "[?] 1 4 ?");

    m->getAtomWithIdx(0)->setProp<bool>(internKey("foo4"), 1);
    m->getAtomWithIdx(1)->setProp<bool>(internKey("foo4"), 0);
    m->getAtomWithIdx(2)->setProp<bool>(internKey("foo4"), 0);
    FileParserUtils::createAtomBoolPropertyList(*m, "foo4");
    REQUIRE(m->hasProp(internKey("atom.bprop.foo4")));
    CHECK(m->getProp<std::string>(internKey("atom.bprop.foo4")) == "1 0 0");

    m->getBondWithIdx(0)->setProp<bool>(internKey("foo5"), 1);
    m->getBondWithIdx(1)->setProp<bool>(internKey("foo5"), 0);
    FileParserUtils::createBondBoolPropertyList(*m, "foo5");
    REQUIRE(m->hasProp(internKey("bond.bprop.foo5")));
    CHECK(m->getProp<std::string>(internKey("bond.bprop.foo5")) == "1 0");
  }
  SECTION("long lines") {
    auto m = "COC"_smiles;
    REQUIRE(m);
    auto getter = [&m](size_t which) { return m->getAtomWithIdx(which); };
    m->getAtomWithIdx(0)->setProp<std::string>(internKey("foo1"), std::string(80, 'a'));
    m->getAtomWithIdx(1)->setProp<std::string>(internKey("foo1"), std::string(80, 'b'));
    m->getAtomWithIdx(2)->setProp<std::string>(internKey("foo1"), std::string(80, 'c'));
    FileParserUtils::createAtomStringPropertyList(*m, "foo1");
    REQUIRE(m->hasProp(internKey("atom.prop.foo1")));
    std::string ps = m->getProp<std::string>(internKey("atom.prop.foo1"));
    CHECK(ps.length() > 240);
    CHECK(ps.find("\n") != std::string::npos);
    for (auto &atom : m->atoms()) {
      atom->clearProp(internKey("foo1"));
    }
    FileParserUtils::applyMolListProps<std::string>(*m, "atom.prop.",
                                                    m->getNumAtoms(), getter);
    CHECK(m->getAtomWithIdx(0)->getProp<std::string>(internKey("foo1")) ==
          std::string(80, 'a'));
    CHECK(m->getAtomWithIdx(1)->getProp<std::string>(internKey("foo1")) ==
          std::string(80, 'b'));
    CHECK(m->getAtomWithIdx(2)->getProp<std::string>(internKey("foo1")) ==
          std::string(80, 'c'));
  }
}

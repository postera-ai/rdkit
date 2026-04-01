//
//  Copyright (c) 2017, Novartis Institutes for BioMedical Research Inc.
//  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Novartis Institutes for BioMedical Research Inc.
//       nor the names of its contributors may be used to endorse or promote
//       products derived from this software without specific prior written
//       permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include <RDGeneral/test.h>
#include <RDGeneral/utils.h>
#include <GraphMol/RDKitBase.h>
#include <GraphMol/RDKitQueries.h>
#include <GraphMol/MolPickler.h>
#include <GraphMol/MonomerInfo.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/FileParsers/MolSupplier.h>
#include <GraphMol/FileParsers/FileParsers.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <GraphMol/SmilesParse/SmartsWrite.h>
#include <GraphMol/Substruct/SubstructMatch.h>

#include <RDGeneral/RDLog.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace RDKit;

void testGlobalPickleProps() {
  BOOST_LOG(rdInfoLog) << "-----------------------\n";
  BOOST_LOG(rdInfoLog) << "Testing pickling of properties" << std::endl;

  std::vector<double> v;
  v.push_back(1234.);
  v.push_back(444.);
  v.push_back(1123.);

  ROMol *m = SmilesToMol("CC");
  m->setProp(internKey("double"), 1.0);
  m->setProp(internKey("int"), 100);
  m->setProp(internKey("bool"), true);
  m->setProp(internKey("boolfalse"), false);
  m->setProp(internKey("dvec"), v);

  Atom *a = m->getAtomWithIdx(0);
  a->setProp(internKey("double"), 1.0);
  a->setProp(internKey("int"), 100);
  a->setProp(internKey("bool"), true);
  a->setProp(internKey("boolfalse"), false);
  a->setProp(internKey("dvec"), v);
  a->setProp(internKey("_private"), true);

  Bond *b = m->getBondWithIdx(0);
  b->setProp(internKey("double"), 1.0);
  b->setProp(internKey("int"), 100);
  b->setProp(internKey("bool"), true);
  b->setProp(internKey("boolfalse"), false);
  b->setProp(internKey("dvec"), v);
  b->setProp(internKey("_private"), true);

  std::string pkl;
  {
    MolPickler::setDefaultPickleProperties(PicklerOps::AllProps);
    MolPickler::pickleMol(*m, pkl);
    RWMol *m2 = new RWMol(pkl);
    TEST_ASSERT(m2);
    TEST_ASSERT(m2->getProp<double>(internKey("double")) == 1.0);
    TEST_ASSERT(m2->getProp<int>(internKey("int")) == 100);
    TEST_ASSERT(m2->getProp<bool>(internKey("bool")) == true);
    TEST_ASSERT(m2->getProp<bool>(internKey("boolfalse")) == false);

    a = m2->getAtomWithIdx(0);
    TEST_ASSERT(a->getProp<double>(internKey("double")) == 1.0);
    TEST_ASSERT(a->getProp<int>(internKey("int")) == 100);
    TEST_ASSERT(a->getProp<bool>(internKey("bool")) == true);
    TEST_ASSERT(a->getProp<bool>(internKey("boolfalse")) == false);
    TEST_ASSERT(a->getProp<bool>(internKey("_private")) == true);

    b = m2->getBondWithIdx(0);
    TEST_ASSERT(b->getProp<double>(internKey("double")) == 1.0);
    TEST_ASSERT(b->getProp<int>(internKey("int")) == 100);
    TEST_ASSERT(b->getProp<bool>(internKey("bool")) == true);
    TEST_ASSERT(b->getProp<bool>(internKey("boolfalse")) == false);
    TEST_ASSERT(b->getProp<bool>(internKey("_private")) == true);
    // TEST_ASSERT(b->getProp<std::vector<double> >("dvec") == v);
    delete m2;
  }

  {
    MolPickler::setDefaultPickleProperties(PicklerOps::MolProps);
    MolPickler::pickleMol(*m, pkl);
    RWMol *m2 = new RWMol(pkl);
    TEST_ASSERT(m2);
    TEST_ASSERT(m2->getProp<double>(internKey("double")) == 1.0);
    TEST_ASSERT(m2->getProp<int>(internKey("int")) == 100);
    TEST_ASSERT(m2->getProp<bool>(internKey("bool")) == true);
    TEST_ASSERT(m2->getProp<bool>(internKey("boolfalse")) == false);

    a = m2->getAtomWithIdx(0);
    TEST_ASSERT(!a->hasProp(internKey("double")));
    TEST_ASSERT(!a->hasProp(internKey("int")));
    TEST_ASSERT(!a->hasProp(internKey("bool")));
    TEST_ASSERT(!a->hasProp(internKey("boolfalse")));
    TEST_ASSERT(!a->hasProp(internKey("_private")));

    b = m2->getBondWithIdx(0);
    TEST_ASSERT(!b->hasProp(internKey("double")));
    TEST_ASSERT(!b->hasProp(internKey("int")));
    TEST_ASSERT(!b->hasProp(internKey("bool")));
    TEST_ASSERT(!b->hasProp(internKey("boolfalse")));
    TEST_ASSERT(!b->hasProp(internKey("_private")));
    delete m2;
  }

  {
    MolPickler::setDefaultPickleProperties(PicklerOps::AtomProps);
    MolPickler::pickleMol(*m, pkl);
    RWMol *m2 = new RWMol(pkl);
    TEST_ASSERT(m2);
    TEST_ASSERT(!m2->hasProp(internKey("double")));
    TEST_ASSERT(!m2->hasProp(internKey("int")));
    TEST_ASSERT(!m2->hasProp(internKey("bool")));
    TEST_ASSERT(!m2->hasProp(internKey("boolfalse")));

    a = m2->getAtomWithIdx(0);
    TEST_ASSERT(a->getProp<double>(internKey("double")) == 1.0);
    TEST_ASSERT(a->getProp<int>(internKey("int")) == 100);
    TEST_ASSERT(a->getProp<bool>(internKey("bool")) == true);
    TEST_ASSERT(a->getProp<bool>(internKey("boolfalse")) == false);
    TEST_ASSERT(!a->hasProp(internKey("_private")));

    b = m2->getBondWithIdx(0);
    TEST_ASSERT(!b->hasProp(internKey("double")));
    TEST_ASSERT(!b->hasProp(internKey("int")));
    TEST_ASSERT(!b->hasProp(internKey("bool")));
    TEST_ASSERT(!b->hasProp(internKey("boolfalse")));
    TEST_ASSERT(!b->hasProp(internKey("_private")));
    delete m2;
  }

  {
    MolPickler::setDefaultPickleProperties(PicklerOps::AtomProps |
                                           PicklerOps::PrivateProps);

    MolPickler::pickleMol(*m, pkl);
    RWMol *m2 = new RWMol(pkl);
    TEST_ASSERT(m2);
    TEST_ASSERT(!m2->hasProp(internKey("double")));
    TEST_ASSERT(!m2->hasProp(internKey("int")));
    TEST_ASSERT(!m2->hasProp(internKey("bool")));
    TEST_ASSERT(!m2->hasProp(internKey("boolfalse")));

    a = m2->getAtomWithIdx(0);
    TEST_ASSERT(a->getProp<double>(internKey("double")) == 1.0);
    TEST_ASSERT(a->getProp<int>(internKey("int")) == 100);
    TEST_ASSERT(a->getProp<bool>(internKey("bool")) == true);
    TEST_ASSERT(a->getProp<bool>(internKey("boolfalse")) == false);
    TEST_ASSERT(a->getProp<bool>(internKey("_private")) == true);

    b = m2->getBondWithIdx(0);
    TEST_ASSERT(!b->hasProp(internKey("double")));
    TEST_ASSERT(!b->hasProp(internKey("int")));
    TEST_ASSERT(!b->hasProp(internKey("bool")));
    TEST_ASSERT(!b->hasProp(internKey("boolfalse")));
    TEST_ASSERT(!b->hasProp(internKey("_private")));
    delete m2;
  }

  {
    MolPickler::setDefaultPickleProperties(PicklerOps::BondProps);
    MolPickler::pickleMol(*m, pkl);
    RWMol *m2 = new RWMol(pkl);
    TEST_ASSERT(m2);
    TEST_ASSERT(!m2->hasProp(internKey("double")));
    TEST_ASSERT(!m2->hasProp(internKey("int")));
    TEST_ASSERT(!m2->hasProp(internKey("bool")));
    TEST_ASSERT(!m2->hasProp(internKey("boolfalse")));

    a = m2->getAtomWithIdx(0);
    TEST_ASSERT(!a->hasProp(internKey("double")));
    TEST_ASSERT(!a->hasProp(internKey("int")));
    TEST_ASSERT(!a->hasProp(internKey("bool")));
    TEST_ASSERT(!a->hasProp(internKey("boolfalse")));
    TEST_ASSERT(!a->hasProp(internKey("_private")));

    b = m2->getBondWithIdx(0);
    TEST_ASSERT(b->getProp<double>(internKey("double")) == 1.0);
    TEST_ASSERT(b->getProp<int>(internKey("int")) == 100);
    TEST_ASSERT(b->getProp<bool>(internKey("bool")) == true);
    TEST_ASSERT(b->getProp<bool>(internKey("boolfalse")) == false);
    TEST_ASSERT(!b->hasProp(internKey("_private")));
    delete m2;
  }

  {
    MolPickler::setDefaultPickleProperties(PicklerOps::BondProps |
                                           PicklerOps::PrivateProps);

    MolPickler::pickleMol(*m, pkl);
    RWMol *m2 = new RWMol(pkl);
    TEST_ASSERT(m2);
    TEST_ASSERT(!m2->hasProp(internKey("double")));
    TEST_ASSERT(!m2->hasProp(internKey("int")));
    TEST_ASSERT(!m2->hasProp(internKey("bool")));
    TEST_ASSERT(!m2->hasProp(internKey("boolfalse")));

    a = m2->getAtomWithIdx(0);
    TEST_ASSERT(!a->hasProp(internKey("double")));
    TEST_ASSERT(!a->hasProp(internKey("int")));
    TEST_ASSERT(!a->hasProp(internKey("bool")));
    TEST_ASSERT(!a->hasProp(internKey("boolfalse")));
    TEST_ASSERT(!a->hasProp(internKey("_private")));

    b = m2->getBondWithIdx(0);
    TEST_ASSERT(b->getProp<double>(internKey("double")) == 1.0);
    TEST_ASSERT(b->getProp<int>(internKey("int")) == 100);
    TEST_ASSERT(b->getProp<bool>(internKey("bool")) == true);
    TEST_ASSERT(b->getProp<bool>(internKey("boolfalse")) == false);
    TEST_ASSERT(b->getProp<bool>(internKey("_private")) == true);
    delete m2;
  }

  delete m;

  BOOST_LOG(rdErrorLog) << "\tdone" << std::endl;
}

int main(int, char **) {
  RDLog::InitLogs();

  testGlobalPickleProps();
}

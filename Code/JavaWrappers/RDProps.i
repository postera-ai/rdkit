/* 
*
*  Copyright (c) 2015, Novartis Institutes for BioMedical Research Inc.
*  All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met: 
*
*     * Redistributions of source code must retain the above copyright 
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above
*       copyright notice, this list of conditions and the following 
*       disclaimer in the documentation and/or other materials provided 
*       with the distribution.
*     * Neither the name of Novartis Institutes for BioMedical Research Inc. 
*       nor the names of its contributors may be used to endorse or promote 
*       products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

%{
#include <RDGeneral/types.h>
#include <RDGeneral/RDProps.h>
%}


%include <RDGeneral/RDProps.h>

%extend RDKit::RDProps {
  std::string getProp(const std::string &key) {
    std::string res;
    self->getProp(RDKit::internKey(key), res);
    return res;
  }
  int getIntProp(const std::string &key) {
    return self->getProp<int>(RDKit::internKey(key));
  }
  bool getBoolProp(const std::string &key) {
    return self->getProp<bool>(RDKit::internKey(key));
  }
  double getDoubleProp(const std::string &key) {
    return self->getProp<double>(RDKit::internKey(key));
  }
  void setProp(const std::string &key, const std::string &val, bool computed = false) {
    self->setProp(RDKit::internKey(key), val, computed);
  }
  void setIntProp(const std::string &key, int val, bool computed = false) {
    self->setProp(RDKit::internKey(key), val, computed);
  }
  void setBoolProp(const std::string &key, bool val, bool computed = false) {
    self->setProp(RDKit::internKey(key), val, computed);
  }
  void setDoubleProp(const std::string &key, double val, bool computed = false) {
    self->setProp(RDKit::internKey(key), val, computed);
  }
  bool hasProp(const std::string &key) {
    return self->hasProp(RDKit::internKey(key));
  }
  void clearProp(const std::string &key) {
    self->clearProp(RDKit::internKey(key));
  }
}


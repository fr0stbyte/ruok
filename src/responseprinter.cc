/*
    This file is part of ruok - a program that measures timings of transferring data with URL syntax
    Copyright (C) 2011 by Radu Brumariu [brum76@gmail.com]
    
    ruok is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ruok is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ruok.  If not, see <http://www.gnu.org/licenses/>.
*/
#include<fstream>
#include<iomanip>
#include<libxml/parser.h>
#include<libxml/tree.h>

#include "responseprinter.h"

// forward declaration for libjson validator
extern "C" int parse_and_report(const char *filename);

namespace ruok {

  ResponsePrinter::ResponsePrinter() {
    timings = std::list<double>();
    responseCode = 0;
    valids = std::map<std::string, bool>();
  }

  ResponsePrinter::~ResponsePrinter() {
    timings.clear();
    responseCode = 0;
    valids.clear();
  }

  void ResponsePrinter::addTiming(double data){
    timings.push_back(data);
  }

  long ResponsePrinter::getResponseCode(void) {
    return responseCode;
  }

  double ResponsePrinter::getDocumentSize(void) {
    return dsize;
  }

  void ResponsePrinter::setDocumentSize(double data) {
    dsize = data;
  }

  void ResponsePrinter::addResponseCode(long code){
    responseCode = code;
  }

  bool ResponsePrinter::checkXML(std::string filename){
    xmlParserCtxtPtr ctxt;
    xmlDocPtr doc;
    valids["xml"] = false;
    ctxt = xmlNewParserCtxt();
    if(ctxt == NULL){
      return valids["xml"];
    }

    doc = xmlCtxtReadFile(ctxt, filename.c_str(), NULL, XML_PARSE_DTDVALID|XML_PARSE_NOERROR);

    if(doc == NULL) {
      valids["xml"] = false;
    } else {
      if(ctxt->valid == 0){
	valids["xml"] = false;
      }
      xmlFreeDoc(doc);
    }
    xmlFreeParserCtxt(ctxt);

    xmlCleanupParser();

  }

  bool ResponsePrinter::checkJSON(std::string filename) {
    valids["json"] = false;
    if(parse_and_report(filename.c_str()) == 0) {
       valids["json"] = true; 
    } else { 
      valids["json"] = false; 
    }
    return valids["json"];
  }


  std::ostream& operator<<(std::ostream &os, ResponsePrinter obj){
    std::list<double>::iterator it;
    for(it = obj.timings.begin(); it != obj.timings.end(); it++){
      os << std::left << std::setfill(' ') << std::setw(20) << std::fixed << (*it);
    }
    os << std::left << std::setfill(' ') << std::setw(20)  << std::fixed << obj.getDocumentSize();

    os << std::left << std::setfill(' ') << std::setw(20)  << obj.getResponseCode();
    
    if(obj.valids.find("json") != obj.valids.end()) {
      os << std::left << std::setfill(' ') << std::setw(20) << ( (obj.valids["json"])?"\x1b[32mOK\x1b[0m":"\x1b[31mNOT OK\x1b[0m" );
    }
    if(obj.valids.find("xml") != obj.valids.end()) {
      os << std::left << std::setfill(' ') << std::setw(20) << ( (obj.valids["xml"])?"\x1b[32mOK\x1b[0m":"\x1b[31mNOT OK\x1b[0m" );
    }
    os << std::endl;

    return os; 
  }




}

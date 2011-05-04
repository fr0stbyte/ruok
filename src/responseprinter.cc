#include<iomanip>
#include<libxml/parser.h>
#include<libxml/tree.h>

#include "responseprinter.h"


namespace ruok {

  ResponsePrinter::ResponsePrinter() {
    timings = std::list<double>();
    responseCode = 0;
    valids = std::map<std::string, bool>();
  }

  ResponsePrinter::~ResponsePrinter() {
    timings.clear();
    // delete(timings);
    responseCode = 0;
    valids.clear();
  }

  void ResponsePrinter::addTiming(double data){
    timings.push_back(data);
  }
  /*
  double* ResponsePrinter::getTiming(){
    if(timings->empty()) {
      return NULL;
    }
    double *result = new double(timings->front());
    timings->pop_front();
    return result;
  }
  */
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

  std::ostream& operator<<(std::ostream &os, ResponsePrinter obj){
    std::list<double>::iterator it;
    for(it = obj.timings.begin(); it != obj.timings.end(); it++){
      os << std::left << std::setfill(' ') << std::setw(20) << std::fixed << (*it);
    }
    os << std::left << std::setfill(' ') << std::setw(20)  << std::fixed << obj.getDocumentSize();

    os << std::left << std::setfill(' ') << std::setw(20)  << obj.getResponseCode();
    
    
    for(std::map<std::string,bool>::iterator it = obj.valids.begin(); it != obj.valids.end(); it++) {
      //      os << (*it).first << ":" << ( ((*it).second)?"OK":"NOT OK" ) << "\t\t";
      os << std::left << std::setfill(' ') << std::setw(20)  << ( ((*it).second)?"\x1b[32mOK\x1b[0m":"\x1b[31mNOT OK\x1b[0m" );
    }
    os << std::endl;

    return os; 
  }




}

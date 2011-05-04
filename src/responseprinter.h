#ifndef _RUOK_RESPONSEPRINTER_H
#define _RUOK_RESPONSEPRINTER_H
#include<list>
#include<map>
#include<string>
#include<iostream>

namespace ruok {

  class ResponsePrinter {
  private :
    std::list<double> timings;
    long responseCode;
    std::map<std::string,bool> valids;
    double dsize;

  public :
    ResponsePrinter();
    ~ResponsePrinter();

    void addTiming(double data);
    //    double getTiming(void);
    void addResponseCode(long code);
    long getResponseCode(void);
    double getDocumentSize(void);
    void setDocumentSize(double data);
    bool checkXML(std::string filename);
    bool isvalid(std::string key);

    //operator to print out results
    friend std::ostream &operator<<(std::ostream &os, ResponsePrinter obj);

  };


} // end namespace 

#endif  // end _RUOK_RESPONSEPRINTER_H

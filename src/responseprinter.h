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
    bool checkXML(int fd);
    bool checkJSON(FILE* fp);
    bool isvalid(std::string key);

    //operator to print out results
    friend std::ostream &operator<<(std::ostream &os, ResponsePrinter obj);

  };


} // end namespace 

#endif  // end _RUOK_RESPONSEPRINTER_H

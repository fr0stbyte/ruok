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
#ifndef _RUOK_OPTIONS_H
#define _RUOK_OPTIONS_H
#define GNU_SOURCE 

#include<string>
#include<curl/curl.h>
#include<getopt.h>

namespace ruok {

  struct config {
    int help;
    int json;
    int rate;
    int period;
    int no_header;
    int ms;
    std::string ua;
    std::string url;
    int verbose;
    int version;
    int xml;
    size_t (*callback)(void*,size_t,size_t,void*);
  };
  

  class OptionsParser {
  private:
    struct config m_config;

  public:
    OptionsParser();
    int parse(int ac, char** av);
    struct config getConfig() { return m_config; };
    void printHelp(const char* prog);
  };

}

#endif // end _RUOK_OPTIONS_H

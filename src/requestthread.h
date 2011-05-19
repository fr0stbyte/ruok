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
#ifndef _RUOK_REQUESTTHREAD_H
#define _RUOK_REQUESTTHREAD_H


#include<iostream>
#include<iomanip>
#include<string>
#include<curl/curl.h>

#include "options.h"

namespace ruok {

  struct tmpfile {
    const char* filename;
    FILE* s;
  };

  int processRequest(struct config *C);
  //  int processMultiRequest(std::string url, struct config *C);
  void startThreads(struct config C);

  size_t default_callback(void *p, size_t n, size_t l, void *d);
  size_t checkcontent_callback(void *p, size_t n, size_t l, void *d);

  void printHeader(struct config C);

  class MainThread {
  private:
    struct config m_Cfg;
  public:
    MainThread(struct config C);
    void start();
  };

} // end namespace

#endif // end _RUOK_REQUESTTHREAD_H

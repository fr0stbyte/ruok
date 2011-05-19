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
#include "options.h"
#include "requestthread.h"

int main(int argc, char** argv) {

  curl_global_init(CURL_GLOBAL_DEFAULT);

  ruok::OptionsParser* options = new ruok::OptionsParser();
  options->parse(argc, argv);

  struct ruok::config Cfg = options->getConfig();

  delete(options);
  
  if(!Cfg.no_header) {printHeader(Cfg);}
  
  
  ruok::MainThread mt(Cfg);
  mt.start();
 
  curl_global_cleanup();
  return 0;

}

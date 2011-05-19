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

#include<list>
#include<cstdio>
#include "requestthread.h"
#include "options.h"
#include "responseprinter.h"

namespace ruok {
  
  //struct config;
  CURLINFO timers[] = { CURLINFO_NAMELOOKUP_TIME, CURLINFO_CONNECT_TIME, CURLINFO_APPCONNECT_TIME, CURLINFO_PRETRANSFER_TIME, CURLINFO_STARTTRANSFER_TIME, CURLINFO_TOTAL_TIME };

  int processRequest(struct config *C) {
    int ret = EXIT_FAILURE;
    CURL* handle;
    CURLcode res;
    std::list<double> results;
    struct tmpfile tfile = {tmpnam(NULL), NULL};
    int factor = (C->ms)?1000:1;
  
    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, C->callback);
    if(C->xml || C->json) {
      curl_easy_setopt(handle, CURLOPT_WRITEDATA, &tfile);
    } else {
      curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);
    }
    curl_easy_setopt(handle, CURLOPT_VERBOSE, long(C->verbose));
    curl_easy_setopt(handle, CURLOPT_USERAGENT, C->ua.c_str());
    curl_easy_setopt(handle, CURLOPT_URL, C->url.c_str());
 
    res = curl_easy_perform(handle);  

    if(CURLE_OK == res) {
      CURLcode r;
      double result;
      long rcode;
      double last=0;

      ResponsePrinter rp = ResponsePrinter();
      last = 0;
      
      for( int j = 0; j<= 4; j++) {
	r = curl_easy_getinfo(handle, timers[j] , &result);
	if(CURLE_OK != r || result == 0) { 
	  rp.addTiming(result*factor);
	} else {
	  rp.addTiming((result-last)*factor);
	  last = result;
	}
      }
      r = curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &result);
      if(CURLE_OK != r) { 
	rp.addTiming(-1);
      } else {
	rp.addTiming(result*factor);
      }
      
      r = curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD, &result);
      if(CURLE_OK != r) { 
	rp.setDocumentSize(-1);
      } else {
	rp.setDocumentSize(result);
      }

      if(tfile.s) {
	if(C->xml) {
	  rp.checkXML(tfile.filename);
	}
	  
	if(C->json) {
	  rp.checkJSON(tfile.filename);
	}
	fclose(tfile.s);
      }
      
      r = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &rcode);
      if(CURLE_OK != r) { rcode = -1; }   
      rp.addResponseCode(rcode);
      std::cout << rp ;
      ret = EXIT_SUCCESS;
    }
    curl_easy_cleanup(handle);
    return ret;
  }

  void printHeader(struct config C) {
    if(C.verbose) {std::cout << "Using url : " << C.url << std::endl;}
    std::cout << std::left << std::setfill(' ') << std::setw(20) << "DNS";
    std::cout << std::left << std::setfill(' ') << std::setw(20) << "Connect";
    std::cout << std::left << std::setfill(' ') << std::setw(20) << "SSL";
    std::cout << std::left << std::setfill(' ') << std::setw(20) << "Protocol";
    std::cout << std::left << std::setfill(' ') << std::setw(20) << "First byte";
    std::cout << std::left << std::setfill(' ') << std::setw(20) << "Total";
    std::cout << std::left << std::setfill(' ') << std::setw(20) << "Bytes";
    std::cout << std::left << std::setfill(' ') << std::setw(20) << "Return code";
    if(C.json){
      std::cout << std::left << std::setfill(' ') << std::setw(20) << "JSON";
    }
    if(C.xml){
      std::cout << std::left << std::setfill(' ') << std::setw(20) << "XML";
    }
    std::cout << std::endl;
  }



  size_t default_callback(void *p, size_t n, size_t l, void *d) {
    (void)p;
    (void)d;
    return n*l;
  }


  size_t checkcontent_callback(void *p, size_t n, size_t l, void *d) {
    struct tmpfile *out = (struct tmpfile*)d;
    if(out && !out->s) {
      out->s = fopen(out->filename, "wb+");
      if(!out->s){
	return -1;
      }
    }
    fseek(out->s, 0, SEEK_END);
    fwrite(p, n, l, out->s);
  }

  MainThread::MainThread(struct config Cfg)
    : m_Cfg(Cfg) {};

  void MainThread::start() {
    static int period = m_Cfg.period;
    std::list<pid_t> pids;
    int ret;
 
    while(period > 0) {
      useconds_t sl = (useconds_t)(1.0*1000/m_Cfg.rate);
      for(int i=0; i < m_Cfg.rate; i++) {
	  //	  std::cout << " Count : " << i << " Period : " << period << std::endl;
	  pid_t pid = fork();
	  if(pid > 0) {  // parent
	    usleep(sl);
	    int status;
	    while(waitpid(-1, &status, WNOHANG) > 0) { 1; }
	  } else if(pid == 0) { // child 
	    ret = ruok::processRequest(&m_Cfg);
	    _exit(ret);
	  } else { // failure to fork
	  perror("Failed to fork : ");
	  exit(EXIT_FAILURE);
	  }
	}
	int status;
	while(waitpid(-1, &status, 0) > 0) { 1; }
      }
      period--;
    }
  

} // end namespace 

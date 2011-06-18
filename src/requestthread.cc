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
#include<cstdlib>
#include<sys/wait.h>
#include<curl/curl.h>
#include "requestthread.h"
#include "options.h"
#include "responseprinter.h"

namespace ruok {
  
  //struct config;
  CURLINFO timers[] = { CURLINFO_NAMELOOKUP_TIME, CURLINFO_CONNECT_TIME, CURLINFO_APPCONNECT_TIME, CURLINFO_PRETRANSFER_TIME, CURLINFO_STARTTRANSFER_TIME, CURLINFO_TOTAL_TIME, CURLINFO_REDIRECT_TIME };

  int processRequest(struct config *C) {
    int ret = EXIT_FAILURE;
    CURL* handle;
    CURLcode res;
    std::list<double> results;
    char tempfile[] = "/tmp/ruokXXXXXX";
    struct tmpfile tfile = {0,NULL};
    tfile.fd = mkstemp(tempfile);
    if(tfile.fd) {
      tfile.fp = fdopen(tfile.fd, "wb+");
    } 
    int factor = (C->ms)?1000:1;
  
    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, C->callback);
    if(C->xml || C->json) {
      curl_easy_setopt(handle, CURLOPT_WRITEDATA, &tfile);
    } else {
      curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);
    }
    
    if(C->headers) {
      curl_easy_setopt(handle, CURLOPT_HTTPHEADER, C->headers);
    }
    curl_easy_setopt(handle, CURLOPT_VERBOSE, long(C->verbose));
    curl_easy_setopt(handle, CURLOPT_USERAGENT, C->ua.c_str());
    curl_easy_setopt(handle, CURLOPT_URL, C->url.c_str());

    // automatically set referer when it follows a Location:redirect:
    curl_easy_setopt(handle, CURLOPT_AUTOREFERER, 1L);

    // follow Location ( redirects ) 
    if( C->follow_redirects ) {
      curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
    }

    res = curl_easy_perform(handle);  

    if(C->headers){
      curl_slist_free_all(C->headers);
    }
    if(CURLE_OK == res) {
      CURLcode r;
      double result;
      long rcode;
      double last=0;

      ResponsePrinter rp = ResponsePrinter();
      last = 0;
      
      for( int j = 0; j<=4; j++) {
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
      r = curl_easy_getinfo(handle, CURLINFO_REDIRECT_TIME, &result);
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

      if(tfile.fp) {
	if(C->xml) {
	  rp.checkXML(tfile.fd);
	}
	  
	if(C->json) {
	  rp.checkJSON(tfile.fp);
	}
	fclose(tfile.fp);
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
    std::cout << std::left << std::setfill(' ') << std::setw(20) << "Redirect";
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
    /*    if(out && !out->fd) {
      out->fp = fdopen(out->fd, "wb+");
      if(!out->fp){
	return -1;
      }
      }*/
    fseek(out->fp, 0, SEEK_END);
    size_t ret = fwrite(p, n, l, out->fp);
    return ret;
  }

  MainThread::MainThread(struct config Cfg)
    : m_Cfg(Cfg) {};

  void MainThread::start() {
    static int period = m_Cfg.period;
    std::list<pid_t> pids;
    int ret;
 
    while(period > 0) {
      useconds_t sl = (useconds_t)(1.0*1000000/m_Cfg.rate);
      for(int i=0; i < m_Cfg.rate; i++) {
	  pid_t pid = fork();
	  if(pid > 0) {  // parent
	    usleep(sl);
	    int status;
	    while(waitpid(-1, &status, WNOHANG) > 0) { ; }
	  } else if(pid == 0) { // child 
	    ret = ruok::processRequest(&m_Cfg);
	    _exit(ret);
	  } else { // failure to fork
	    perror("Failed to fork : ");
	    exit(EXIT_FAILURE);
	  }
      }
      int status;
      while(waitpid(-1, &status, 0) > 0) { ; }
      period--;
    }
    
  }
  

} // end namespace 

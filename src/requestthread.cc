#include<list>
#include<cstdio>
#include "requestthread.h"
#include "options.h"
#include "responseprinter.h"

namespace ruok {
  
  //struct config;
  CURLINFO timers[] = { CURLINFO_NAMELOOKUP_TIME, CURLINFO_CONNECT_TIME, CURLINFO_APPCONNECT_TIME, CURLINFO_PRETRANSFER_TIME, CURLINFO_STARTTRANSFER_TIME, CURLINFO_TOTAL_TIME };

  int processRequest(struct config *C) {

    CURL* handle;
    CURLcode res;
    std::list<double> results;
    struct tmpfile tfile = {tmpnam(NULL), NULL};
    int factor = (C->ms)?1000:1;
  
   handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, C->callback);
    if(C->xml) {
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
      
      
      if(tfile.s){
	rp.checkXML(tfile.filename);
	fclose(tfile.s);
      }
      
      r = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &rcode);
      if(CURLE_OK != r) { rcode = -1; }
      curl_easy_cleanup(handle);
      rp.addResponseCode(rcode);
      std::cout << rp ;
      return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
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


  size_t checkxml_callback(void *p, size_t n, size_t l, void *d) {
    int i;
    struct tmpfile *out = (struct tmpfile*)d;
    if(out && !out->s) {
      out->s = fopen(out->filename, "wb+");
      if(!out->s){
	return -1;
      }
    }
    fseek(out->s, 0, SEEK_END);
    //    std::cout << out->filename << std::endl;
    return fwrite(p, n, l, out->s);
  }

  MainThread::MainThread(struct config Cfg)
    : m_Cfg(Cfg) {};

  void MainThread::start() {
    static int period = m_Cfg.period;
    int ret;
    //    boost::thread_group tg;
 
    while(period > 0){
      for(int i=0;i<m_Cfg.rate;i++) {
	if(!fork()) {  // child
	  ret = ruok::processRequest(&m_Cfg);
	  _exit(ret);
	}
      }
      for(int i=0; i<m_Cfg.rate;i++) { // parent 
	  int status;
	  wait(&status);
      }
      period--;
    }
  }

} // end namespace 

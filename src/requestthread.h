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
  size_t checkxml_callback(void *p, size_t n, size_t l, void *d);

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

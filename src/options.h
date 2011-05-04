#ifndef _RUOK_OPTIONS_H
#define _RUOK_OPTIONS_H
#define GNU_SOURCE 

#include<string>
#include<curl/curl.h>
#include<getopt.h>

namespace ruok {

  struct config {
    int help;
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

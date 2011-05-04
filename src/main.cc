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

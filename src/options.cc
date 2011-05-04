#include "requestthread.h"
#include "options.h"

#if HAVE_CONFIG_H
#include <config.h>
#else
#define PACKAGE_NAME "ruok"
#define PACKAGE_VERSION "0.3"
#endif

namespace ruok {

  OptionsParser::OptionsParser() 
    { 
      m_config.callback = default_callback;
      m_config.xml = 0;
      m_config.verbose = 0L;
      m_config.no_header = 0;
      m_config.ms = 0;
      m_config.version = 0;
      m_config.ua = std::string("Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_5_8; en-us) AppleWebKit/533.21.1 (KHTML, like Gecko) Version/5.0.5 Safari/533.21.1");
      m_config.url = std::string("");
      m_config.rate = 1;
      m_config.period = 10;
      m_config.help = 0;
    }

  int OptionsParser::parse(int ac, char** av) {
    int c;
    static struct option longopts[] = 
      {
	{ "help", no_argument, &m_config.help , 1 },
	{ "miliseconds", no_argument, &m_config.ms, 1 },
	{ "no-header", no_argument, 0, 'n' },
	{ "period", required_argument, 0, 'p' },
	{ "rate", required_argument, 0, 'r' },
	{ "url", required_argument, 0, 'u' },
	{ "user-agent", required_argument, 0, 'U' },
	{ "verbose", no_argument, 0, 'v' },
	{ "version", no_argument, 0, 'V' },
	{ "xml", no_argument, 0, 'x' },
	{0, 0, 0, 0}
      };


    while(1) {
      int idx = 0;
      c = getopt_long(ac, av, "?hmnp:r:u:U:vVx" , longopts, &idx);
      if (c == -1) { //end of options 
	break;
      }

      switch(c) {
	case 0:
	if(longopts[idx].flag !=0) break;
	std::cout << "Option : " << longopts[idx].name << std::endl;
	if(optarg) {
	  std::cout << "Optarg : " << optarg << std::endl;
	}
	break;

      case 'm':
	m_config.ms = 1;
	break;

      case 'n':
	m_config.no_header = 1;
	break;

      case 'p':
	m_config.period = atoi(optarg);
	std::cout << "Period set to : " << m_config.period << std::endl;
	break;

      case 'r':
	m_config.rate = atoi(optarg);
	std::cout << "Rate set to : " << m_config.rate << std::endl;
	break;
      
      case 'u':
	m_config.url = optarg;
	std::cout << "Url set to : " << m_config.url << std::endl;
	break;

      case 'U':
	m_config.ua = optarg;
	std::cout << "User agent set to : " << m_config.ua << std::endl;
	break;

      case 'v' :
	m_config.verbose = 1;
	break;

      case 'V':
	m_config.version = 1;
	break;

      case 'x':
	m_config.xml = 1;
	break;

      case 'h':
      case '?':
	m_config.help = 1;
	break;
     
      default:
	break;
      }
    }

    if(m_config.help){
      printHelp(av[0]);
      exit(EXIT_SUCCESS);
    }

    if(m_config.version) {
      std::cout << PACKAGE_NAME << " : Version : " << PACKAGE_VERSION << std::endl;
      exit(EXIT_SUCCESS);
    }

    if(m_config.no_header){
      std::cout << "no header" << std::endl;
    }

    if(m_config.xml){
      m_config.callback = checkxml_callback;
    }

    if(m_config.url == "") {
      std::cout << "No url provided -- giving up" << std::endl;
      exit(EXIT_FAILURE);
    }


  }


  void OptionsParser::printHelp(const char* prog) {
    std::cout << prog << "\n" \
	      <<  "\t--help,h,?\t" << std::setfill(' ') << std::setw(10) << " - Displays this information\n" \
	      << "\t--miliseconds,m\t" << std::setw(10) << "- Displays times in miliseconds [ seconds is default ]\n" \
	      << "\t--no-header,n" << std::setw(10) << "- When printing results don't print the header [ default on ]\n" \
	      << "\t--period,p" << std::setw(10) << "- Time to run the tests in seconds [ default 10 ]\n" \
	      << "\t--rate,r"<< std::setw(10) << "- Rate of requests per second [ default 1 ]\n" \
	      << "\t--url,u" << std::setw(10) << "- Url to connect to [ required ]\n" \
	      << "\t--user-agent,U" << std::setw(10) << "- User agent to use\n" \
	      << "\t--verbose,v" << std::setw(10) << "- Be verbose\n" \
	      << "\t--version,V" << std::setw(10) << "- Print version\n" \
	      << "\t--xml,x\t-\tValidate retrieved data through an XML parser" << std::endl;
  }

}

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
#include<cstdlib>
#include "requestthread.h"
#include "options.h"

namespace ruok {

  OptionsParser::OptionsParser() 
    { 
      m_config.callback = default_callback;
#ifdef LIBXML2_FOUND
      m_config.xml = 0;
#endif
      m_config.json = 0;
      m_config.verbose = 0L;
      m_config.follow_redirects = 0;
      m_config.no_header = 0;
      m_config.ms = 0;
      m_config.version = 0;
      m_config.ua = std::string("Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_5_8; en-us) AppleWebKit/533.21.1 (KHTML, like Gecko) Version/5.0.5 Safari/533.21.1");
      m_config.url = std::string("");
      m_config.rate = 1;
      m_config.period = 10;
      m_config.help = 0;
      m_config.headers = NULL;
    }

  void OptionsParser::parse(int ac, char** av) {
    int c;
    static struct option longopts[] = 
      {
        { "follow", no_argument, 0, 'f' },
	{ "help", no_argument, 0 , 'h' },
	{ "Header", required_argument, 0, 'H' },
	{ "json", no_argument, 0, 'j' },
	{ "miliseconds", no_argument, 0 , 'm' },
	{ "no-header", no_argument, 0, 'n' },
	{ "period", required_argument, 0, 'p' },
	{ "rate", required_argument, 0, 'r' },
	{ "url", required_argument, 0, 'u' },
	{ "user-agent", required_argument, 0, 'U' },
	{ "verbose", no_argument, 0, 'v' },
	{ "version", no_argument, 0, 'V' },
#ifdef LIBXML2_FOUND
	{ "xml", no_argument, 0, 'x' },
#endif
	{0, 0, 0, 0}
      };


    while(1) {
      int idx = 0;
      c = getopt_long(ac, av, "?fhH:jmnp:r:u:U:vVx" , longopts, &idx);
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

      case 'f':
        m_config.follow_redirects = 1;
        break;

      case 'j':
	m_config.json = 1;
	break;

      case 'H':
        m_config.headers = curl_slist_append(m_config.headers, optarg);
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

#ifdef LIBXML2_FOUND
      case 'x':
	m_config.xml = 1;
	break;
#endif

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
#ifdef LIBXML2_FOUND
    if(m_config.xml || m_config.json){
      m_config.callback = checkcontent_callback;
    }
#else
    if(m_config.json){
      m_config.callback = checkcontent_callback;
    }
#endif

    if(m_config.url == "") {
      std::cout << "No url provided -- giving up" << std::endl;
      exit(EXIT_FAILURE);
    }


  }


  void OptionsParser::printHelp(const char* prog) {
    std::cout << prog << "\n" \
              <<  "\t--follow,f\t" << std::setfill(' ') << std::setw(10) << "  Follow redirects\n" \
	      <<  "\t--help,h,?\t" << std::setfill(' ') << std::setw(10) << "  Displays this information\n" \
              <<  "\t--Header,H\t" << std::setfill(' ') << std::setw(10) << "  Add a Header directive \n" \
	      <<  "\t--json,j\t" << std::setfill(' ') << std::setw(10) << "  Validate retrieved data through a JSON parser\n" \
	      << "\t--miliseconds,m\t" << std::setw(10) << "  Displays times in miliseconds [ seconds is default ]\n" \
	      << "\t--no-header,n" << std::setw(10) << "  When printing results don't print the header [ default on ]\n" \
	      << "\t--period,p" << std::setw(10) << "  Time to run the tests in seconds [ default 10 ]\n" \
	      << "\t--rate,r"<< std::setw(10) << "  Rate of requests per second [ default 1 ]\n" \
	      << "\t--url,u" << std::setw(10) << "  Url to connect to [ required ]\n" \
	      << "\t--user-agent,U" << std::setw(10) << "  User agent to use\n" \
	      << "\t--verbose,v" << std::setw(10) << "  Be verbose\n" \
	      << "\t--version,V" << std::setw(10) << "  Print version\n";
#ifdef LIBXML2_FOUND
    std::cout << "\t--xml,x\t-\tValidate retrieved data through an XML parser";
#endif
      std::cout << std::endl;

  }

}

#include <iostream>
#include <map>
#include <boost/multiprecision/gmp.hpp>
#include <boost/math/special_functions/factorials.hpp>
#include "lib_version.h"
#include "CLParser.h"
#include "allocator.h"

using namespace std;

void help()
{
      cout << R"(
 This tool gets ip_filter.tsv from stdin. It extracts IPs from them,
 then it sortes and filters IPs. The result will be sent to stdout

    ip_filter  [-version | -? ]
       Options:  
       -version    -get version of program
       -?          -about program (this info)

 Example to use (Linux):
    ip_filter.tsv | ip_filter

 Example to use (Windows):
    ip_filter < ip_filter.tsv)"
           << endl;
}

void version_ip_filter()
{
      cout << "Version ip_filter: " << version() << endl;
}


using namespace boost::multiprecision;
using namespace boost::math;

#ifndef _TEST

int main(int argc, char* argv[])
{
      ParserCommandLine PCL;
      PCL.AddFormatOfArg("?", no_argument, '?');
      PCL.AddFormatOfArg("help", no_argument, '?');
      PCL.AddFormatOfArg("version", no_argument, 'v');

      PCL.SetShowError(false);
      PCL.Parser(argc, argv);

      if (PCL.Option['?'])
      {
            help();
            return 0;
      }
      if (PCL.Option['v'])
      {
            version_ip_filter();
            return 0;
      }

      map <int, mpz_int> default_allocator_map;
      for (int i = 0; i < 10; i++)
      {
            default_allocator_map[i] = static_cast<mpz_int>(factorial<mpf_float_100>(i));
      }


      return 0;
}

#endif

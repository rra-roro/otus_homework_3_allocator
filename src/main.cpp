#include <iostream>
#include <map>
#include <charconv>
#include <boost/multiprecision/gmp.hpp>
#include <boost/math/special_functions/factorials.hpp>
#include "lib_version.h"
#include "CLParser.h"
#include "allocator.h"
#include "forward_list.h"

using namespace std;

void help()
{
      cout << R"(
 This demo is my custom allocator.

    allocator  [-version | -? | -max_item <size> ]
       Options:  
       -version    -get version of program
       -?          -about program (this info)
       -max_item   -amount items in containers
)" << endl;
}

void version_ip_filter()
{
      cout << "Version allocator: " << version() << endl;
}

int my_factorial(int i)
{
      int ret = 1;
      for (int j = 0; j < i; j++)
      {
            ret *= j;
      }

      return ret;
}

using namespace boost::multiprecision;
using namespace boost::math;

#ifndef _TEST

int main(int argc, char* argv[])
{
      int max_item = 10;

      ParserCommandLine PCL;
      PCL.AddFormatOfArg("?", no_argument, '?');
      PCL.AddFormatOfArg("help", no_argument, '?');
      PCL.AddFormatOfArg("version", no_argument, 'v');
      PCL.AddFormatOfArg("max_item", required_argument, 'm');


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

      if (PCL.Option['m'])
      {
            size_t SizeParam = PCL.Option['m'].ParamOption[0].size();
            const char* const ptrStr = PCL.Option['m'].ParamOption[0].data();
            from_chars(ptrStr, ptrStr + SizeParam, max_item);
      }

      if (PCL.Option['m'])
      {
            map<int, mpz_int> map_default_allocator;
            for (int i = 0; i < max_item; i++)
            {
                  //   map_default_allocator[i] = static_cast<mpz_int>(factorial<mpf_float_100>(i));
                  map_default_allocator[i] = static_cast<mpz_int>(i);
            }

            map<int, mpz_int, less<mpz_int>, custom_allocator<pair<const int, mpz_int>, 10>> map_my_allocator;
            for (int i = 0; i < max_item; i++)
            {
                  //   map_my_allocator[i] = static_cast<mpz_int>(factorial<mpf_float_100>(i));
                  map_my_allocator[i] = static_cast<mpz_int>(i);
            }
      }
      else
      {
            map<int, int> map_default_allocator;
            for (int i = 0; i < max_item; i++)
            {
                  map_default_allocator[i] = my_factorial(i);
            }

            map<int, int, less<int>, custom_allocator<pair<const int, int>, 10>> map_my_allocator;
            for (int i = 0; i < max_item; i++)
            {
                  map_my_allocator[i] = my_factorial(i);
            }

            for (auto& item : map_my_allocator)
            {
                  cout << item.first << " " << item.second << "\n";
            }
      }

      custom_forward_list<int> list_default_allocator;
      for (int i = 0; i < max_item; ++i)
      {
            list_default_allocator.push_front(i);
      }

      custom_forward_list<int, custom_allocator<int, 10>> list_my_allocator;
      for (int i = 0; i < max_item; ++i)
      {
            list_my_allocator.push_front(i);
      }

      for (auto& obj : list_my_allocator)
      {
            cout << obj << " ";
      }

      return 0;
}

#endif

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
using namespace boost::multiprecision;
using namespace boost::math;

void help()
{
      cout << R"(
 This demo is my custom allocator.

    allocator  [-version | -? | -max_item <size> ]
       Options:  
       -version    -get version of program
       -?          -about program (this info)
       -max_item   -amount items in containers. default 10
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

template <typename T>
auto factorial(int i) -> T
{
      if constexpr (std::is_same<int, T>::value)
            return my_factorial(i);
      else
            return static_cast<T>(factorial<mpf_float_100>(i));
}

template <typename T>
void task_map(int max_item)
{
      map<int, T> map_default_allocator;
      for (int i = 0; i < max_item; i++)
      {
            map_default_allocator[i] = factorial<T>(i);
      }

      map<int, T, less<T>, custom_allocator<pair<const int, T>, 10>> map_my_allocator;
      for (int i = 0; i < max_item; i++)
      {
            map_my_allocator[i] = factorial<T>(i);
      }

      for (auto& item : map_my_allocator)
      {
            cout << item.first << " " << item.second << "\n";
      }
}

void task_custom_forward_list(int max_item)
{
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
}


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
            task_map<mpz_int>(max_item);
      }
      else
      {
            task_map<int>(max_item);
      }

      task_custom_forward_list(max_item);

      return 0;
}

#endif

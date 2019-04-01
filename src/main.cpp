#include <iostream>
#include <map>
#include <charconv>
#include <boost/multiprecision/gmp.hpp>
#include "lib_version.h"
#include "CLParser.h"
#include "allocator.h"
#include "forward_list.h"
#include "factorial.h"

using namespace std;
using namespace boost::multiprecision;
using namespace roro_lib;

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

void version_allocator()
{
      cout << "Version allocator: " << version() << endl;
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
      cout << "\n";
}


#ifndef _TEST

int main(int argc, char* argv[])
{
      int max_item = 10;

      try
      {
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
                  version_allocator();
                  return 0;
            }

            if (PCL.Option['m'])
            {
                  const size_t size_param = PCL.Option['m'].ParamOption[0].size();
                  const char* const ptr_str = PCL.Option['m'].ParamOption[0].data();
                  from_chars(ptr_str, ptr_str + size_param, max_item);
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

      }
      catch (const std::exception& ex)
      {
            cerr << "Error: " << ex.what() << endl;
            return EXIT_FAILURE;
      }
      catch (...)
      {
            cerr << "Error: unknown exception" << endl;
            return EXIT_FAILURE;
      }

      return 0;
}

#endif

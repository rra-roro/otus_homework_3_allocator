#include <string>
#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"

#include "lib_version.h"

#define _TEST 1

#include "allocator.h"
#include "forward_list.h"
#include "factorial.h"


TEST(version, test1)
{
      ASSERT_TRUE(version() > 0);
}

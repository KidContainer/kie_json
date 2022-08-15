#include <kie_json.hpp>
#include <iostream>
#include <gtest/gtest.h>


// Demonstrate some basic assertions.
TEST(FromJson, StructComplex) {
  using namespace kie::json;

  struct Inner{
    kie::json::Field<int,"i"> i;
    kie::json::Field<std::vector<int>, "v"> v;
  };

  struct A{
    kie::json::Field<std::vector<int>, "i"> i;
    bool b;
    kie::json::Field<Inner, "inner"> inner;
    kie::json::Field<std::vector<Inner>, "inner_vec"> inner_vec;
  };
  

  A a{.i=std::vector{1,2,3}, .b=true, .inner=Inner{.i=10, .v=std::vector{1,2,3}}};
  EXPECT_EQ(to_json(a).dump(), "{\"i\":[1,2,3],\"inner\":{\"i\":10,\"v\":[1,2,3]},\"inner_vec\":null}");

  std::vector<Inner> vec{Inner{}, Inner{.i=2, .v=std::vector{1,2,3}}};
  a.inner_vec = vec;
  EXPECT_EQ(to_json(a).dump(), "{\"i\":[1,2,3],\"inner\":{\"i\":10,\"v\":[1,2,3]},\"inner_vec\":[{\"i\":0,\"v\":null},{\"i\":2,\"v\":[1,2,3]}]}");

  A b{.inner_vec = vec};
  EXPECT_EQ(to_json(b).dump(), "{\"i\":null,\"inner\":{\"i\":0,\"v\":null},\"inner_vec\":[{\"i\":0,\"v\":null},{\"i\":2,\"v\":[1,2,3]}]}");

}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
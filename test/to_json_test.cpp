#include <kie_json.hpp>
#include <iostream>
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(ToJson, NotRecognized) {
  using namespace kie::json;
  EXPECT_EQ(to_json(1).dump(), "null");
  EXPECT_EQ(to_json('a').dump(), "null");
  EXPECT_EQ(to_json("hello").dump(), "null");
  EXPECT_EQ(to_json(std::string{"hello"}).dump(), "null");
  EXPECT_EQ(to_json(true).dump(), "null");
  EXPECT_EQ(to_json(1.1).dump(), "null");
}

// Demonstrate some basic assertions.
TEST(ToJson, Container) {
  using namespace kie::json;
  EXPECT_EQ(to_json(std::vector{1}).dump(), "[1]");
  EXPECT_EQ(to_json(std::vector{1,2}).dump(), "[1,2]");
  EXPECT_EQ(to_json(std::vector{1,2,3}).dump(), "[1,2,3]");
  EXPECT_EQ(to_json(std::vector<int>{}).dump(), "null");
  EXPECT_EQ(to_json(std::vector{1.1,1.2,1.3}).dump(), "[1.1,1.2,1.3]");

  EXPECT_EQ(to_json(std::list{1}).dump(), "[1]");
  EXPECT_EQ(to_json(std::list{1,2}).dump(), "[1,2]");
  EXPECT_EQ(to_json(std::list{1,2,3}).dump(), "[1,2,3]");
  EXPECT_EQ(to_json(std::list<int>{}).dump(), "null");
  EXPECT_EQ(to_json(std::list{1.1,1.2,1.3}).dump(), "[1.1,1.2,1.3]");

  EXPECT_EQ(to_json(std::array{1}).dump(), "[1]");
  EXPECT_EQ(to_json(std::array{1,2}).dump(), "[1,2]");
  EXPECT_EQ(to_json(std::array{1,2,3}).dump(), "[1,2,3]");
  EXPECT_EQ(to_json(std::array<int, 10>{}).dump(), "[0,0,0,0,0,0,0,0,0,0]");
  EXPECT_EQ(to_json(std::array{1.1,1.2,1.3}).dump(), "[1.1,1.2,1.3]");
}



// Demonstrate some basic assertions.
TEST(ToJson, StructNotRecgonized) {
  using namespace kie::json;
  struct Inner{
    int i;
  };

  struct A{
    int i;
    bool b;
    Inner inner;
  };

  EXPECT_EQ(to_json(A{}).dump(), "null");
}


// Demonstrate some basic assertions.
TEST(ToJson, StructPartial) {
  using namespace kie::json;
  struct Inner{
    int i;
  };

  struct A{
    kie::json::JsonField<int> i={.tag="ii"};
    bool b;
    Inner inner;
  };

  struct B{
    kie::json::JsonField<int> i = {.tag="i"};
    bool b;
    kie::json::JsonField<Inner> inner = {.value={.i=10}, .tag="inner"};
  };

  struct InnerRecognized{
     kie::json::JsonField<int> i = {.value=10, .tag="i"};
     bool b;
     kie::json::JsonField<Inner> inner = {.tag="inner"};
  };

  struct C{
    kie::json::JsonField<int> i = {.tag="i"};
    bool b;
    kie::json::JsonField<InnerRecognized> inner = {.value={.i={.value=20, .tag="i"}}, .tag="inner_recognized"};
  };

  EXPECT_EQ(to_json(A{}).dump(), "{\"ii\":0}");
  EXPECT_EQ(to_json(B{}).dump(), "{\"i\":0,\"inner\":null}");
  EXPECT_EQ(to_json(C{}).dump(), "{\"i\":0,\"inner_recognized\":{\"i\":20,\"inner\":null}}");
}

// Demonstrate some basic assertions.
TEST(ToJson, StructContainer) {
  using namespace kie::json;

  struct A{
    kie::json::JsonField<std::vector<int>> i = {.value={1,2,3,4,5}, .tag="i"};
    bool b;
  };

  EXPECT_EQ(to_json(A{}).dump(), "{\"i\":[1,2,3,4,5]}");
}

// Demonstrate some basic assertions.
TEST(ToJson, StructComplex) {
  using namespace kie::json;

  struct Inner{
    kie::json::JsonField<int> i = {.value=10, .tag="i"};
    kie::json::JsonField<std::vector<int>> v = {.value={1,2,3,4,5}, .tag="v"};
  };

  struct A{
    kie::json::JsonField<std::vector<int>> i = {.value={1,2,3,4,5}, .tag="i"};
    bool b;
    kie::json::JsonField<Inner> inner = {.tag="inner"};
    kie::json::JsonField<std::array<Inner,3>> inner_array = {.tag="inner_array"};
  };

  EXPECT_EQ(to_json(A{}).dump(), "{\"i\":[1,2,3,4,5],\"inner\":{\"i\":10,\"v\":[1,2,3,4,5]},\"inner_array\":[{\"i\":10,\"v\":[1,2,3,4,5]},{\"i\":10,\"v\":[1,2,3,4,5]},{\"i\":10,\"v\":[1,2,3,4,5]}]}");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
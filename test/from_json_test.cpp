#include <kie_json.hpp>
#include <iostream>
#include <gtest/gtest.h>


// Demonstrate some basic assertions.
TEST(FromJson, Container) {
  using namespace kie::json;
  EXPECT_EQ(from_json<std::vector<int>>("[1]"), std::vector{1});
  EXPECT_EQ(from_json<std::vector<int>>("[1,2]"), (std::vector<int>{1,2}));
  EXPECT_EQ(from_json<std::vector<int>>("[1,2,3]"), (std::vector{1,2,3}));
  EXPECT_EQ(from_json<std::vector<int>>("null"), (std::vector<int>{}));
  EXPECT_EQ(from_json<std::vector<double>>("[1.1,1.2,1.3]"), (std::vector<double>{1.1,1.2,1.3}));

  EXPECT_EQ(from_json<std::list<int>>("[1]"), std::list{1});
  EXPECT_EQ(from_json<std::list<int>>("[1,2]"), (std::list{1,2}));
  EXPECT_EQ(from_json<std::list<int>>("[1,2,3]"), (std::list{1,2,3}));
  EXPECT_EQ(from_json<std::list<int>>("null"), (std::list<int>{}));
  EXPECT_EQ(from_json<std::list<double>>("[1.1,1.2,1.3]"), (std::list<double>{1.1,1.2,1.3}));
}



// Demonstrate some basic assertions.
TEST(FromJson, StructNotRecgonized) {
  using namespace kie::json;
  struct Inner{
    int i;
  };

  struct A{
    int i;
    bool b;
    Inner inner;
  };

  auto t = from_json<A>("null");
  EXPECT_EQ(t.i, 0);
  EXPECT_EQ(t.b, false);
  EXPECT_EQ(t.inner.i, 0);
}


// Demonstrate some basic assertions.
TEST(FromJson, StructPartial) {
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
     kie::json::JsonField<int> i = {.tag="i"};
     bool b;
     kie::json::JsonField<Inner> inner = {.tag="inner"};
  };

  struct C{
    kie::json::JsonField<int> i = {.tag="i"};
    bool b;
    kie::json::JsonField<InnerRecognized> inner = {.tag="inner_recognized"};
  };

  auto a = from_json<A>("{\"ii\":10}");
  EXPECT_EQ(a.i.value, 10);
  EXPECT_EQ(a.b, false);
  EXPECT_EQ(a.inner.i, 0);


  auto b = from_json<B>("{\"i\":10,\"inner\":null}");
  EXPECT_EQ(b.i.value, 10);
  EXPECT_EQ(b.b, false);
  EXPECT_EQ(b.inner.value.i, 0);

  auto c = from_json<C>("{\"i\":0,\"inner_recognized\":{\"i\":20,\"inner\":null}}");
  EXPECT_EQ(c.i.value, 0);
  EXPECT_EQ(c.b, false);
  EXPECT_EQ(c.inner.value.i.value, 20);
  EXPECT_EQ(c.inner.value.b, false);
  EXPECT_EQ(c.inner.value.inner.value.i, 0);
}

// Demonstrate some basic assertions.
TEST(FromJson, StructContainer) {
  using namespace kie::json;

  struct A{
    kie::json::JsonField<std::vector<int>> i = {.value={1,2,3,4,5}, .tag="i"};
    bool b;
  };

  auto a = from_json<A>("{\"i\":[1,2,3,4,5]}");
  EXPECT_EQ(a.i.value, (std::vector<int>{1,2,3,4,5}));
  EXPECT_EQ(a.b, false);
}

// Demonstrate some basic assertions.
TEST(FromJson, StructComplex) {
  using namespace kie::json;

  struct Inner{
    kie::json::JsonField<int> i = {.tag="i"};
    kie::json::JsonField<std::vector<int>> v = {.tag="v"};
  };

  struct A{
    kie::json::JsonField<std::vector<int>> i = {.tag="i"};
    bool b;
    kie::json::JsonField<Inner> inner = {.tag="inner"};
    kie::json::JsonField<std::vector<Inner>> inner_vec = {.tag="inner_vec"};
  };

  auto a = from_json<A>("{\"i\":[1,2,3,4,5],\"inner\":{\"i\":10,\"v\":[1,2,3,4,5]},\"inner_vec\":[{\"i\":10,\"v\":[1,2,3,4,5]},{\"i\":10,\"v\":[1,2,3,4,5]},{\"i\":10,\"v\":[1,2,3,4,5]}]}");
  
  EXPECT_EQ(a.i.value, (std::vector<int>{1,2,3,4,5}));
  EXPECT_EQ(a.b, false);
  EXPECT_EQ(a.inner.value.i.value, 10);
  EXPECT_EQ(a.inner.value.v.value, (std::vector<int>{1,2,3,4,5}));
  EXPECT_EQ(a.inner_vec.value[0].i.value, 10);
  EXPECT_EQ(a.inner_vec.value[0].v.value, (std::vector<int>{1,2,3,4,5}));
  EXPECT_EQ(a.inner_vec.value[1].i.value, 10);
  EXPECT_EQ(a.inner_vec.value[1].v.value, (std::vector<int>{1,2,3,4,5}));
  EXPECT_EQ(a.inner_vec.value[2].i.value, 10);
  EXPECT_EQ(a.inner_vec.value[2].v.value, (std::vector<int>{1,2,3,4,5}));
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
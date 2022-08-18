# Kie_JSON

[![CMake](https://github.com/Kidsunbo/kie_json/actions/workflows/cmake.yml/badge.svg)](https://github.com/Kidsunbo/kie_json/actions/workflows/cmake.yml)
[![Latest version of 'kie_json' @ Cloudsmith](https://api-prd.cloudsmith.io/v1/badges/version/kie/kies/conan/kie_json/latest/xc=_;xp=_/?render=true&show_latest=true)](https://cloudsmith.io/~kie/repos/kies/packages/detail/conan/kie_json/latest/xc=_;xp=_/)
[![codecov](https://codecov.io/gh/Kidsunbo/kie_json/branch/master/graph/badge.svg?token=X3THMAG9TA)](https://codecov.io/gh/Kidsunbo/kie_json)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/Kidsunbo/kie_json.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Kidsunbo/kie_json/alerts/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


**Kie_JSON** is a library that aims to convenience and simple usage. C++ struct can be serialized and deserialized into/from json as easy as possible. It depends on [nlohmann_json](https://github.com/nlohmann/json) for json parsing and [Boost.PFR](https://github.com/boostorg/pfr) for reflection.

## This project has been merged into [kie_toolbox_cpp](https://github.com/Kidsunbo/kie_toolbox_cpp)

## Install

For use this library, one could always download it into your workspace and include it as it's a single header library. But some other way is also available.

#### Conan
This library is also available in my private conan repo.  Use it with conan by the following
command.
```shell
conan remote add kie-kies https://conan.cloudsmith.io/kie/kies/
```
Then add `kie_json/0.1.0` to your `conanfile.txt`.

#### CMake
Thanks to the new `CMakeDeps` and `CMakeToolchain` generators which are non intrusive. This library can be used without conan directly.

Download this library.
```shell
cd your_lib_folder

git clone https://github.com/Kidsunbo/kie_json.git
```

Add to your `CMakeLists.txt`
```cmake
add_subdirectory(kie_json)

target_link_libraries(<target> PUBLIC kie_json)
```

But this is not recommended because I use conan global target as dependency, which doesn't exist with normal `find_package`.

## Usage
This library only recgonized struct type which is aggregate as input or output. It's enough for dto in most cases. For fields that are able to be serialized and deserialized, just wrap it with `kie::json::Field<T, "tag">` which contains a `tag` as its name. Then use `to_json` and `from_json` for serde operation.

``` c++
struct A{
    kie::json::Field<int, "i"> i;
};

A a{.i= 10}; 
std::cout<<kie::json::to_json(a)<<std::endl; //{"i": 10}

```


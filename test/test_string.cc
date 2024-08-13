//
// Created by cc498 on 2024/8/13.
//

// #include <doctest/doctest.h>
#include <type_traits>
#include <iostream>
#include <cjson/json_object.h>

#include <charconv>

#include "cjson/parser.h"
using namespace std;
int main() {
//   const string str = R"( {"haha"  :  [ "\\t\n\uD83D\uDE00 \"Hello, \u03A9!\"", 10, [10,true,null] ],
//
// "haha" :null })";
//   cjson::Parser<string,string> parser(str.cbegin(),str.cend());
//   auto t = parser.fromJson();
  // CHECK(cjson::to_string(t.type()) == "string");
  // cout <<cjson::to_string(t.type());
  // cout << "cc " << t.getString();
  cjson::Json::object_t o{};
  o.emplace("haha","ff");
  o.emplace("haha",1);
  cout << "fds";

}
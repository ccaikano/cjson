//
// Created by cc498 on 2024/8/13.
//

// #include <doctest/doctest.h>
#include <type_traits>
#include <iostream>
#include <cjson/json_object.h>

#include <charconv>

#include "cjson/parser.h"

template <typename map_t, std::enable_if_t<cjson::is_map<std::decay_t<map_t>>, int> = 0>
void test(map_t &&m) {

}
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
  // cjson::Json::object_t o{};
  // o.emplace("haha","ff");
  // o.emplace("haha",1);
  // cout << "fds";
  // std::map<string,int> m;
  // std::unordered_map<std::string, std::string> p = {
  //   {"fsadf", "fsdaf"},
  //   {"key2", "value2"},
  //   {"key3", "value3"}
  // };
  std::unordered_map<string,map<string,string>> m = {
    {"haha",{{"fsdf","fsdf"}}},
      {"key2",{{"value2","v"}}}
  };
  // test(m);
  // cjson::Json::object_t o (std::make_move_iterator(m.begin()),std::make_move_iterator(m.end()));
  // static_assert(cjson::is_map<decltype(m)>);
  // std::unordered_map<string,cjson::Json> b(std::make_move_iterator(m.begin()),std::make_move_iterator(m.end()));
  cout << m.at("haha").at("fsdf");
  cjson::Json j (m);
  cout << m.contains("haha");

}
//
// Created by CC on 2024/3/7.
//

#include <doctest/doctest.h>
#include <type_traits>
#include <iostream>
#include <cjson/json_object.h>

#include <charconv>

#include "cjson/parser.h"

using namespace std;
TEST_SUITE_BEGIN("unit");

TEST_CASE("test")
{
  cjson::JsonObject<> nullt;
  CHECK(cjson::to_string(nullt.type()) == "null");
  int it = 10;
  cjson::JsonObject<> i(it);
  CHECK(cjson::to_string(i.type()) == "int");
  cjson::JsonObject<> d{10.1};
  CHECK(cjson::to_string(d.type()) == "float");
  cjson::Json str_t("str");
  std::cout << cjson::to_string(str_t.type()) << endl;
  CHECK(cjson::to_string(str_t.type()) == "string");
  std::string t ("test");
  cjson::JsonObject<> k(t);
  std::cout << t << "\n";

  vector<cjson::Json> array;
  array.emplace_back("test");
  array.emplace_back("jiejie");
  cjson::JsonObject arr(array);
  CHECK(cjson::to_string(arr.type()) == "array");
  CHECK(array.empty());

  std::map<string,cjson::Json> map;
  map["fasdf"] = std::move(str_t);
  // cjson::Json m();
  // CHECK(map.empty());
  // CHECK(cjson::to_string(m.type())=="object");

}
TEST_CASE("parse")
{
  // string str = "10      ";
  // cjson::Parser<string,string> parser(str.cbegin(),str.cend());
  // auto t = parser.fromJson();
  // CHECK(cjson::to_string(t.type()) == "int");
  // cout << "\"";
  const string str = R"("\uD83D\uDE00")";
  cjson::Parser<string,string> parser(str.cbegin(),str.cend());
  auto t = parser.fromJson();
  // CHECK(cjson::to_string(t.type()) == "string");
  cout <<cjson::to_string(t.type());
  cout << "cc " << t.getString();
}
TEST_CASE("parse_string")
{
  const string str = R"("\uD83D\uDE00")";
  cjson::Parser<string,string> parser(str.cbegin(),str.cend());
  auto t = parser.fromJson();
  // CHECK(cjson::to_string(t.type()) == "string");
  cout << "\"";
}
TEST_SUITE_END;
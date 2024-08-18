//
// Created by CC on 8/18/24.
//
#include <doctest/doctest.h>

#include <map>
#include <cjson/json_object.hpp>
TEST_CASE("json_object") {
  using namespace cjson;
    Json root;

    root["hello"];
    root["Pi"] = 3.1416;
    root["null"] = nullptr;
    root["obj"] = {
        { "obj_key1", "Hi" },
        { "obj_key2", 123 },
        { "obj_key3", true },
    };
    root["obj"].emplace("obj_key4", 789);

    root["obj"].emplace("obj_key5", Json{{ "key4 child", "i am object value" }});
    root["another_obj"]["child"]["grand"] = "i am grand";

    // take union
    root["obj"].insert(Json {
        { "obj_key6", "i am string" },
        { "obj_key7",  { "i", "am", "array" } },
    });

    root["arr"] = { 1, 2, 3 };
    std::cout << root.dump() << std::endl;

    root["arr"].emplace(4);
    root["arr"].emplace(5);
    // root["arr"].insert( { 6, 7 });
    //
    std::string str = "hello";
    root["str"] = str;
    CHECK(root["str"].asString() == str);

    std::vector<int> vec_op = { 1, 2, 3, 4, 5 };
    Json vec_obj = vec_op;
    // if (vec_obj.as_collection<int>() != root["arr from vec"].as_collection<int>()) {
    //     std::cerr << "error: " << root["arr from vec"].as_string() << std::endl;
    //     return false;
    // }
    //
    // std::map<std::string, int> map_op {
    //     { "key1", 1 },
    //     { "key2", 2 },
    // };
    // root["obj from map"] = map_op;
    // json::object map_obj = map_op;
    // if (map_obj.as_map<int>() != root["obj from map"].as_map<int>()) {
    //     std::cerr << "error: " << root["obj from map"].as_string() << std::endl;
    //     return false;
    // }
    //
    // auto jarr = root.get("arr", json::array());
    // if (jarr.size() != 7) {
    //     std::cerr << "error: " << jarr.size() << std::endl;
    //     return false;
    // }
    //
    // bool is_int_array = root["arr"].is_array() && root["arr"].all<int>();
    // if (!is_int_array) {
    //     std::cerr << "not all int: " << root["arr"].as_string() << std::endl;
    //     return false;
    // }
    // auto to_vec = root["arr"].as_collection<int>();
    // auto to_vec_2 = root["arr"].as_collection<int, std::vector>();
    // auto to_list = root["arr"].as_collection<int, std::list>();
    // auto to_set = root["arr"].as_collection<int, std::set>();
    // auto to_hashset = root["arr"].as_collection<int, std::unordered_set>();
    // auto to_deque = root["arr"].as_collection<int, std::deque>();
    // auto to_q = root["arr"].as_collection<int, std::queue>();
    //
    // std::vector<int> vec = { 1, 2, 3, 4, 5 };
    // root["arr from vec"] = vec;
    //
    // std::set<std::string> set = { "a", "bb\n\nb", "cc\t" };
    // root["arr from set"] = set;
    //
    // std::map<std::string, int> map {
    //     { "key1", 1 },
    //     { "key2", 2 },
    // };
    // root["obj from map"] = map;
    //
    // bool is_int_map = root["obj from map"].is_object() && root["obj from map"].all<int>();
    // if (!is_int_map) {
    //     std::cerr << "not all int: " << root["obj from map"].as_string() << std::endl;
    //     return false;
    // }
    // auto as_map = root["obj from map"].as_map<int>();
    // auto as_map_2 = root["obj from map"].as_map<int, std::map>();
    // auto to_hashmap = root["obj from map"].as_map<int, std::unordered_map>();
    //
    // std::vector<std::list<std::set<int>>> complex { { { 1, 2, 3 }, { 4, 5 } },
    //                                                 { { 6 }, { 7, 8 } } };
    // root["complex"] = complex;
    //
    // root["a\\n"] = "1a\\n";
    // root["a\n"] = "2a\n";
    // if (root["a\\n"].as_string() != "1a\\n") {
    //     std::cerr << "error: " << root["a\\n"].as_string() << std::endl;
    //     return false;
    // }
    // if (root["a\n"].as_string() != "2a\n") {
    //     std::cerr << "error: " << root["a\n"].as_string() << std::endl;
    //     return false;
    // }
    // if (root["a\\n"].to_string() != "\"1a\\\\n\"") {
    //     std::cerr << "error: " << root["a\\n"].to_string() << std::endl;
    //     return false;
    // }
    // if (root["a\n"].to_string() != "\"2a\\n\"") {
    //     std::cerr << "error: " << root["a\n"].to_string() << std::endl;
    //     return false;
    // }
    //
    // std::cout << root << std::endl;
    // std::ofstream ofs("serializing.json");
    // ofs << root;
    // ofs.close();
    //
    // return true;

}
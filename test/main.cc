
#include <cjson/json_object.hpp>

int main() {
  using namespace cjson;
  Json root;
  std::cout << root.dump() << std::endl;

  // root["hello"];
  std::cout << root.dump() << std::endl;
  root["Pi"] = 3.1416;
  root["null"] = nullptr;
  root["obj"] = {
      {"obj_key1", "Hi"},
      {"obj_key2", 123},
      {"obj_key3", true},
  };
  root["obj"].emplace("obj_key4", 789);

  root["obj"].emplace("obj_key5", Json{{"key4 child", "i am object value"}});
  root["another_obj"]["child"]["grand"] = "i am grand";

  // take union
  root["obj"].insert(Json{
      {"obj_key6", "i am string"},
      {"obj_key7", {"i", "am", "array"}},
  });

  root["arr"] = {1, 2, 3};
  std::cout << root.dump() << std::endl;

  root["arr"].emplace(4);
  root["arr"].emplace(5);
  // root["arr"].insert( { 6, 7 });
  //
  std::string str = "hello";
  root["str"] = str;
  //   CHECK(root["str"].asString() == str);

  std::vector<int> vec_op = {1, 2, 3, 4, 5};
  Json vec_obj = vec_op;
}

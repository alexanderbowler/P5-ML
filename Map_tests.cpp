
#include <iostream>
#include <string>
#include <vector>
#include "Map.h"
#include "unit_test_framework.h"

using std::pair;
using std::string;
using std::vector;

//using Iterator = typename BinarySearchTree<Pair_type, PairComp>::Iterator;


TEST(DefaultCnstrcAndEmpty) {
  // A map stores two types, key and value
  Map<string, double> words;

  // One way to use a map is like an array
  ASSERT_TRUE(words.empty());
}
TEST(DefaultsizeEmpty) {
  // A map stores two types, key and value
  Map<string, double> words;

  // One way to use a map is like an array
  ASSERT_TRUE(words.size()==0);
} 
TEST(Insert1) {
  // A map stores two types, key and value
  Map<string, double> words;

  words.insert(std::pair<string,double>("near",1));
  ASSERT_TRUE((*words.find("near")).second==1);
  ASSERT_TRUE(words.size()==1);
} 
TEST(InsertFind2) {
  // A map stores two types, key and value
  Map<string, double> words;

  words.insert(std::pair<string,double>("near",1));
  words.insert(std::pair<string,double>("hear",2));
  words.insert(std::pair<string,double>("there",40));
  words.insert(std::pair<string,double>("question",5));
  ASSERT_TRUE((*words.find("there")).second==40);
  ASSERT_TRUE((*words.find("question")).second==5);
  ASSERT_TRUE((*words.find("near")).second==1);
  ASSERT_TRUE((*words.find("hear")).second==2);
  ASSERT_TRUE(words.find("where")==words.end());
  ASSERT_TRUE(words.size()==4);
} 

TEST(Insert3) {
  // A map stores two types, key and value
  Map<string, double> words;

  words.insert(std::pair<string,double>("near",1));
  words.insert(std::pair<string,double>("hear",2));
  words.insert(std::pair<string,double>("there",40));
  auto it = words.insert(std::pair<string,double>("question",5));
  ASSERT_TRUE(it.second);
  ASSERT_TRUE((*(it.first)).second==5);
  it = words.insert(std::pair<string,double>("question",9));
  ASSERT_TRUE(!it.second);
  ASSERT_TRUE((*(it.first)).second==5);
  ASSERT_TRUE((*words.find("there")).second==40);
  ASSERT_TRUE((*words.find("question")).second==5);
  ASSERT_TRUE((*words.find("near")).second==1);
  ASSERT_TRUE((*words.find("hear")).second==2);
  ASSERT_TRUE(words.find("where")==words.end());
  ASSERT_TRUE(words.size()==4);
} 

TEST(more){
  Map<string, double> words;

  // One way to use a map is like an array
  words["hello"] = 1;
  ASSERT_EQUAL(words["hello"], 1);
  // Maps store a std::pair type, which "glues" one key to one value.
  // The CS term is Tuple, a fixed-size heterogeneous container.
  pair<string, double> tuple;
  tuple.first = "world";
  tuple.second = 2;
  words.insert(tuple);
  ASSERT_EQUAL(words["world"], 2);

  // Here's the C++11 way to insert a pair
  words.insert({"pi", 3.14159});
  ASSERT_ALMOST_EQUAL(words["pi"], 3.14159, 0.00001);

  vector<string> expected_keys = { "hello", "pi", "world" };
  vector<double> expected_values = { 1, 3.14159, 2 };
  vector<string> actual_keys;
  vector<double> actual_values;
  // Iterate over map contents using a C++11 range-for loop
  // This is the equivalent without C++11:
  // for (Map<string, double>::Iterator it = words.begin();
  //      it != words.end(); ++it) {
  //   pair<string, double> &p = *it;
  for (auto &p : words) {
    auto word = p.first; //key
    auto number = p.second; //value
    actual_keys.push_back(word);
    actual_values.push_back(number);
  }
  ASSERT_EQUAL(expected_keys, actual_keys);
  ASSERT_EQUAL(expected_values, actual_values);

  // Check if a key is in the map.  find() returns an iterator.
  auto found_it = words.find("pi");
  ASSERT_NOT_EQUAL(found_it, words.end());
  auto &word = (*found_it).first; //key
  auto number = (*found_it).second; //value
  ASSERT_EQUAL(word, "pi");
  ASSERT_ALMOST_EQUAL(number, 3.14159, 0.00001);

  // When using the [] notation. An element not found is automatically created.
  // If the value type of the map is numeric, it will always be 0 "by default".
  ASSERT_EQUAL(words["bleh"], 0.0);
}

TEST_MAIN()

//============================================================================
// Name        : StroustrupHash.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unordered_map>
#include <iomanip>
using namespace std;

struct Record {
     string name;
     int val;
};

//Specializations for multi-valued key
namespace std {
   template<>
   struct hash<Record>{
        size_t operator()(const Record &r) const
        {

             return hash<string>{}(r.name)^hash<int>{}(r.val);
        }
   };
   template<>
   struct equal_to<Record> {
        bool operator()(const Record& r, const Record& r2) const
        {
             return r.name==r2.name && r.val==r2.val;
        }
   };
}

int main() {

  unordered_map<Record, double> um{};

  Record r1{};
  r1.name = std::string("R1");
  r1.val = 1;
  double v1 = 20.0;

  um[r1] = v1;

  Record r2{};
  r2.name = std::string("R2");
  r2.val = 1;
  double v2 = 30.0;

  um[r2] = v2;

  //Create a function object and then immediately
  //call operator() to get the hash value
  std::size_t term1 = std::hash<std::string>{}(r2.name);

  //Create a function object and then immediately
  //call operator() to get the hash value
  std::size_t term2 = std::hash<int>{}(r2.val);

  std::size_t computedHash =
      term1 ^ term2;

	cout << "Computed Hash (dec) = "
	     << computedHash << "\n"
	     << "Computed Hash (hex) = "
	     << std::hex << computedHash << "\n";

}

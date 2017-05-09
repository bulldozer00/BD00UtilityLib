//============================================================================
// Name        : MultiValueMapKey.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <cstdint>

//This struct will serve as the multi-valued map key to lookup the centerFreq
//and compute the doppler vel for each {primaryCpiId, adjCpiId} pair
struct PrimaryCpiIDAndAdjCpiId {
    int32_t primaryCpiId;
    int32_t adjCpiId;
};

//The following specialized hash() and equal_to() function templates
//are derived from the example in
//Stroustrup's "The C++ Programming Language"
//31.4.3.4. Hash and Equality Functions
namespace std {
   template<>
   struct hash<PrimaryCpiIDAndAdjCpiId>{
        size_t operator()(const PrimaryCpiIDAndAdjCpiId &r) const
        {
             return hash<int32_t>{}(r.primaryCpiId)^hash<int32_t>{}(r.adjCpiId);
        }
   };
   template<>
   struct equal_to<PrimaryCpiIDAndAdjCpiId> {
        bool operator()(const PrimaryCpiIDAndAdjCpiId& r, const PrimaryCpiIDAndAdjCpiId& r2) const
        {
             return r.primaryCpiId==r2.primaryCpiId && r.adjCpiId==r2.adjCpiId;
        }
   };
}
int main() {

PrimaryCpiIDAndAdjCpiId entry1Key = {1,2};
double entry1Val = 10.0;

PrimaryCpiIDAndAdjCpiId entry2Key = {1,3};
double entry2Val = 20.0;

PrimaryCpiIDAndAdjCpiId entry3Key = {2,4};
double entry3Val = 30.0;

PrimaryCpiIDAndAdjCpiId entry4Key = {2,5};
double entry4Val = 40.0;

PrimaryCpiIDAndAdjCpiId entry5Key = {2,5};
double entry5Val = 50.0;

std::unordered_map<PrimaryCpiIDAndAdjCpiId, double> cpiIdTocenterFreqsTable{
  {entry1Key, entry1Val},
  {entry2Key, entry2Val},
  {entry3Key, entry3Val},
  {entry4Key, entry4Val},
  {entry5Key, entry5Val}};

PrimaryCpiIDAndAdjCpiId entry6Key = {2,5};
double entry6Val = 50.0;

auto iter = cpiIdTocenterFreqsTable.insert({entry6Key, entry6Val});
if(not iter.second) {
  std::cout << "Insertion Failed\n";
}

//Try to insert duplicate of key {2,5}
PrimaryCpiIDAndAdjCpiId entry7Key = {2,5};
double entry7Val = 100.0;

iter = cpiIdTocenterFreqsTable.insert({entry7Key, entry7Val});
if(not iter.second) {
  std::cout << "Insertion Failed - entry already present!\n";
}

std::cout << "Done\n";

}

//============================================================================
// Name        : CurveFitting.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
// This tells Catch to provide a main() -
// only do this in one cpp file
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "CurveFitter.h"

TEST_CASE( "CurveFitter", "[Tl]" ) {

  REQUIRE_THROWS_AS(
        CurveFitter cf{}, VCurveFitterException );

  std::initializer_list<DataPt> init{
    {1.0, 5.0},
    {2.0, 5.0}};

  CurveFitter cf{init};
  DataPt dp = cf.addDataPts(init);
  REQUIRE( dp.first == 0.0);
  REQUIRE( dp.second == 5.0);

}

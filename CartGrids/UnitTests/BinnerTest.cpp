#include "catch.hpp"

#include "../Binner.h"

using namespace cruRrp::msp::topics::util;

TEST_CASE( "Meters To Bin - Unipolar", "[Binner]" ) {

  //4 bins covering 0.0 to 12.0 meters
  GridDef gd{0.0, 12.0, 3.0};
  Binner metersToBin(gd);

  REQUIRE(4 == metersToBin.getNumBins());

  REQUIRE(0 == metersToBin(-1.0));
  REQUIRE(0 == metersToBin(.5));
  REQUIRE(0 == metersToBin(2.99));

  REQUIRE(1 == metersToBin(3.0));
  REQUIRE(1 == metersToBin(4.2));
  REQUIRE(1 == metersToBin(5.99));

  REQUIRE(2 == metersToBin(6.0));
  REQUIRE(2 == metersToBin(7.0));
  REQUIRE(2 == metersToBin(8.99));

  REQUIRE(3 == metersToBin(9.0));
  REQUIRE(3 == metersToBin(11.0));
  REQUIRE(3 == metersToBin(14.0));

}

TEST_CASE( "Meters To Bin - Bipolar", "[Binner]" ) {

  //4 bins covering -6.0 to +6.0 meters
  GridDef gd{-6.0, 6.0, 3.0};
  Binner metersToBin(gd);

  REQUIRE(4 == metersToBin.getNumBins());

  REQUIRE(0 == metersToBin(-7.0));
  REQUIRE(0 == metersToBin(-5.0));
  REQUIRE(0 == metersToBin(-5.99));

  REQUIRE(1 == metersToBin(-3.0));
  REQUIRE(1 == metersToBin(-1.2));
  REQUIRE(1 == metersToBin(-.01));

  REQUIRE(2 == metersToBin(0.0));
  REQUIRE(2 == metersToBin(1.5));
  REQUIRE(2 == metersToBin(2.99));

  REQUIRE(3 == metersToBin(3.0));
  REQUIRE(3 == metersToBin(4.5));
  REQUIRE(3 == metersToBin(6.0));
  REQUIRE(3 == metersToBin(7.0));

}

TEST_CASE( "Resize Binner", "[Binner]" ) {

  //4 bins covering -6.0 to +6.0 meters
  GridDef gd1{-6.0, 6.0, 3.0};
  Binner metersToBin(gd1);

  REQUIRE(4 == metersToBin.getNumBins());

  GridDef gd2{-1000.0, 1000.0, 50.0};
  metersToBin.resize(gd2);

  REQUIRE(40 == metersToBin.getNumBins());

}




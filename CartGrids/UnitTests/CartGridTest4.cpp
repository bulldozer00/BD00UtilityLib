#include "catch.hpp"
#include "../CartGrid.hpp"

using namespace cruRrp::msp::topics::util;

class TracData{};

TEST_CASE( "2D retrieval by search extent", "[CartGrid4]" ) {

  // 6 km X 6 km  X 6 km, deltaBin = 2 km
  GridDef xGrid{-3000.0, 3000.0, 2000.0}; //3 bins
  GridDef yGrid{-3000.0, 3000.0, 2000.0}; //3 bins
  GridDef zGrid{-3000.0, 3000.0, 6000.0}; //1 bin
  CartGrid<int32_t> grid{xGrid, yGrid, zGrid};

  REQUIRE(9 == grid.getNumBins());

  //Populate the whole grid
  //Add reference trk id 10 to bin 0
  REQUIRE(0 == grid.addEntry({-3000.0, -3000.0, -3000.0}, 10));
  //Add reference trk id 20 to bin 1
  REQUIRE(1 == grid.addEntry({-3000.0, 0.0, -3000.0}, 20));
  //Add reference trk id 30 to bin 2
  REQUIRE(2 == grid.addEntry({-3000.0, 1500.0, -3000.0}, 30));
  //Add reference trk id 40 to bin 3
  REQUIRE(3 == grid.addEntry({0.0, -3000.0, -3000.0}, 40));
  //Add reference trk id 50 to bin 4
  REQUIRE(4 == grid.addEntry({0.0, 0.0, -3000.0}, 50));
  //Add reference trk id 60 to bin 5
  REQUIRE(5 == grid.addEntry({0.0, 1500.0, -3000.0}, 60));
  //Add reference trk id 70 to bin 6
  REQUIRE(6 == grid.addEntry({1500.0, -3000.0, -3000.0}, 70));
  //Add reference trk id 80 to bin 7
  REQUIRE(7 == grid.addEntry({1500.0, 0.0, -3000.0}, 80));
  //Add reference trk id 90 to bin 8
  REQUIRE(8 == grid.addEntry({1500.0, 3000.0, -3000.0}, 90));

  //Search the whole grid around the origin
  XyzPosCoords searchExtent1{10000.0, 10000.0, 10000.0};
  auto assocTrks =  grid.getEntriesBySearchExtent({0.0, 0.0, 0.0}, searchExtent1);
  REQUIRE(9 == assocTrks.size());

  std::sort(assocTrks.begin(), assocTrks.end());
  REQUIRE(10 == assocTrks[0]);
  REQUIRE(20 == assocTrks[1]);
  REQUIRE(30 == assocTrks[2]);
  REQUIRE(40 == assocTrks[3]);
  REQUIRE(50 == assocTrks[4]);
  REQUIRE(60 == assocTrks[5]);
  REQUIRE(70 == assocTrks[6]);
  REQUIRE(80 == assocTrks[7]);
  REQUIRE(90 == assocTrks[8]);

  //Search none of the grid around the origin
  XyzPosCoords searchExtent2{0.0, 0.0, 0.0};
  assocTrks.clear();
  assocTrks =  grid.getEntriesBySearchExtent({0.0, 0.0, 0.0}, searchExtent2);
  REQUIRE(1 == assocTrks.size()); //Expect back only the ref track
  REQUIRE(50 == assocTrks[0]);

  //Search across X only around the origin
  XyzPosCoords searchExtent3{10000.0, 0.0, 0.0};
  assocTrks.clear();
  assocTrks =  grid.getEntriesBySearchExtent({0.0, 0.0, 0.0}, searchExtent3);
  REQUIRE(3 == assocTrks.size());

  std::sort(assocTrks.begin(), assocTrks.end());
  REQUIRE(20 == assocTrks[0]);
  REQUIRE(50 == assocTrks[1]);
  REQUIRE(80 == assocTrks[2]);

  //Search across Y only around the origin
  XyzPosCoords searchExtent4{0.0, 10000.0, 0.0};
  assocTrks.clear();
  assocTrks =  grid.getEntriesBySearchExtent({0.0, 0.0, 0.0}, searchExtent4);
  REQUIRE(3 == assocTrks.size());

  std::sort(assocTrks.begin(), assocTrks.end());
  REQUIRE(40 == assocTrks[0]);
  REQUIRE(50 == assocTrks[1]);
  REQUIRE(60 == assocTrks[2]);

  //Search across Z only around the origin
  XyzPosCoords searchExtent5{0.0, 0.0, 100000.0};
  assocTrks.clear();
  assocTrks =  grid.getEntriesBySearchExtent({0.0, 0.0, 0.0}, searchExtent5);
  REQUIRE(1 == assocTrks.size()); //Expect back only the ref track
  REQUIRE(50 == assocTrks[0]);

  //Search across X around track at lower left corner {-3000.0, -3000.0, -3000.0}
  XyzPosCoords searchExtent6{10000.0, 0.0, 0.0};
  assocTrks.clear();
  assocTrks =  grid.getEntriesBySearchExtent({-3000.0, -3000.0, -3000.0}, searchExtent6);
  REQUIRE(3 == assocTrks.size());
  std::sort(assocTrks.begin(), assocTrks.end());
  REQUIRE(10 == assocTrks[0]);
  REQUIRE(40 == assocTrks[1]);
  REQUIRE(70 == assocTrks[2]);

}

TEST_CASE( "3D retrieval by searching the whole grid", "[CartGrid4]" ) {

  // 6 km X 6 km X 6 km deltaBin = 2 km
  GridDef xGrid{-3000.0, 3000.0, 2000.0};
  GridDef yGrid{-3000.0, 3000.0, 2000.0};
  GridDef zGrid{-3000.0, 3000.0, 2000.0};

  CartGrid<int32_t> grid{xGrid, yGrid, zGrid};

  REQUIRE(27 == grid.getNumBins());

  //Add reference trk id 10 to the origin
  grid.addEntry({0.0, 0.0, 0.0}, 10);

  //Add trk id 20 offset by +1 bin in Z
  grid.addEntry({0.0, 0.0, 2000.0}, 20);

  //Add trk id 30 offset by +1 bin in Y
  grid.addEntry({0.0, 2000.0, 0.0}, 30);

  //Add trk id 40 offset by -1 bin in X
  grid.addEntry({-2000.0, 0.0, 0.0}, 40);

  //Search the whole grid!
  XyzPosCoords searchExtent{6000.0, 6000.0, 6000.0};
  auto assocTrks =  grid.getEntriesBySearchExtent({0.0, 0.0, 0.0}, searchExtent);
  REQUIRE(4 == assocTrks.size());

  std::sort(assocTrks.begin(), assocTrks.end());
  REQUIRE(10 == assocTrks[0]);
  REQUIRE(20 == assocTrks[1]);
  REQUIRE(30 == assocTrks[2]);
  REQUIRE(40 == assocTrks[3]);

}


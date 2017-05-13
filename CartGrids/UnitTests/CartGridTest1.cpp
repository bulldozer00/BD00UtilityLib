#include "catch.hpp"
#include "../CartGrid.hpp"
#include "../CartGrid2.hpp"

using namespace cruRrp::msp::topics::util;

class TrackData{};

constexpr double METERS_PER_NMI{1852.0};

TEST_CASE( "CartGrid1 Different Container Types", "[CartGrid1]" ) {

  // 1 NMI X 1 NMI X 1 NMI; 1KM bin sizes
  GridDef xGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};
  GridDef yGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};
  GridDef zGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};

  //Default container: std::vector<TrackData>
  CartGrid<std::shared_ptr<TrackData>> gridVec{xGrid, yGrid, zGrid};

  //Try a grid of ptrs
  CartGrid<TrackData*> gridPtrs{xGrid, yGrid, zGrid};

}

TEST_CASE( "CartGrid2 add/remove", "[CartGrid1]" ) {

  // 1 NMI X 1 NMI X 1 NMI; 1KM bin sizes
  GridDef xGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};
  GridDef yGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};
  GridDef zGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};

  //Try std::list<int32_t>
  CartGrid2<SmartEntry<int32_t>> gridList{xGrid, yGrid, zGrid};

  //add 3 entries to the grid at bin 0
  auto e1 =SmartEntry<int32_t>{100};
  gridList.addEntry({0.0, 0.0, 0.0}, e1);
  auto e2 =SmartEntry<int32_t>{200};
  gridList.addEntry({0.0, 0.0, 0.0}, e2);
  auto e3 =SmartEntry<int32_t>{300};
  gridList.addEntry({0.0, 0.0, 0.0}, e3);

  //Retrieve the 3 entries
  std::vector<SmartEntry<int32_t>> smartEntries = gridList.getEntriesByPos({0.0, 0.0, 0.0});

  //verify
  REQUIRE(3 == smartEntries.size());
  auto iter = smartEntries.begin();
  REQUIRE(100 == iter->entry);
  REQUIRE(0 == iter->binIdx);
  ++iter;
  REQUIRE(200 == iter->entry);
  REQUIRE(0 == iter->binIdx);
  ++iter;
  REQUIRE(300 == iter->entry);
  REQUIRE(0 == iter->binIdx);

  //original list = [100, 200, 300]

  //remove the second  entry
  iter = smartEntries.begin();
  ++iter;
  gridList.removeEntry(*iter);

  //modified list = [100, 300]

  smartEntries.clear();
  smartEntries = gridList.getEntriesByPos({0.0, 0.0, 0.0});
  REQUIRE(2 == smartEntries.size());
  iter = smartEntries.begin();
  REQUIRE(100 == iter->entry);
  REQUIRE(0 == iter->binIdx);
  ++iter;
  REQUIRE(300 == iter->entry);
  REQUIRE(0 == iter->binIdx);

  //remove the first  entry
  iter = smartEntries.begin();
  gridList.removeEntry(*iter);

  //modified list = [300]

  smartEntries.clear();
  smartEntries = gridList.getEntriesByPos({0.0, 0.0, 0.0});
  REQUIRE(1 == smartEntries.size());
  iter = smartEntries.begin();
  REQUIRE(300 == iter->entry);
  REQUIRE(0 == iter->binIdx);

  //Remove the third entry
  iter = smartEntries.begin();
  gridList.removeEntry(*iter);
  smartEntries.clear();
  smartEntries = gridList.getEntriesByPos({0.0, 0.0, 0.0});
  REQUIRE(0 == smartEntries.size());

}

TEST_CASE( "3D Bin To 1D Vec Idx", "[CartGrid1]" ) {

  // 1 NMI X 1 NMI X 1 NMI; 1KM bin sizes
  // 1 NMI X 1 NMI X 1 NMI; 1KM bin sizes
  GridDef xGrid{-.5 * METERS_PER_NMI, .5 * METERS_PER_NMI, 1000.0};
  GridDef yGrid{-.5 * METERS_PER_NMI, .5 * METERS_PER_NMI, 1000.0};
  GridDef zGrid{-.5 * METERS_PER_NMI, .5 * METERS_PER_NMI, 1000.0};

  CartGrid<std::shared_ptr<TrackData>> grid{xGrid, yGrid, zGrid};
  CartGrid2<std::shared_ptr<TrackData>> grid2{xGrid, yGrid, zGrid};

  REQUIRE(8 == grid.getNumBins());
  REQUIRE(8 == grid.getNumBins());

  REQUIRE(0 == grid.binToVecIdx({0,0,0}));
  REQUIRE(0 == grid2.binToVecIdx({0,0,0}));

  REQUIRE(1 == grid.binToVecIdx({0,0,1}));
  REQUIRE(1 == grid2.binToVecIdx({0,0,1}));

  REQUIRE(2 == grid.binToVecIdx({0,1,0}));
  REQUIRE(2 == grid2.binToVecIdx({0,1,0}));

  REQUIRE(3 == grid.binToVecIdx({0,1,1}));
  REQUIRE(3 == grid2.binToVecIdx({0,1,1}));

  REQUIRE(4 == grid.binToVecIdx({1,0,0}));
  REQUIRE(4 == grid2.binToVecIdx({1,0,0}));

  REQUIRE(5 == grid.binToVecIdx({1,0,1}));
  REQUIRE(5 == grid2.binToVecIdx({1,0,1}));

  REQUIRE(6 == grid.binToVecIdx({1,1,0}));
  REQUIRE(6 == grid2.binToVecIdx({1,1,0}));

  REQUIRE(7 == grid.binToVecIdx({1,1,1}));
  REQUIRE(7 == grid2.binToVecIdx({1,1,1}));

}

TEST_CASE( "3D Pos To Vec Idx : Unipolar", "[CartGrid1]" ) {

  // 1 NMI X 1 NMI X 1 NMI; 1KM bin sizes
  GridDef xGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};
  GridDef yGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};
  GridDef zGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};

  CartGrid<std::shared_ptr<TrackData>> grid{xGrid, yGrid, zGrid};
  CartGrid2<std::shared_ptr<TrackData>> grid2{xGrid, yGrid, zGrid};

  REQUIRE(8 == grid.getNumBins());
  REQUIRE(8 == grid2.getNumBins());

  REQUIRE(0 == grid.positionToBin({500.0, 500.0, 500.0}));
  REQUIRE(0 == grid2.positionToBin({500.0, 500.0, 500.0}));

  REQUIRE(1 == grid.positionToBin({500.0, 500.0, 1500.0}));
  REQUIRE(1 == grid2.positionToBin({500.0, 500.0, 1500.0}));

  REQUIRE(2 == grid.positionToBin({500.0, 1500.0, 500.0}));
  REQUIRE(2 == grid2.positionToBin({500.0, 1500.0, 500.0}));

  REQUIRE(3 == grid.positionToBin({500.0, 1500.0, 1500.0}));
  REQUIRE(3 == grid2.positionToBin({500.0, 1500.0, 1500.0}));

  REQUIRE(4 == grid.positionToBin({1500.0, 500.0, 500.0}));
  REQUIRE(4 == grid2.positionToBin({1500.0, 500.0, 500.0}));

  REQUIRE(5 == grid.positionToBin({1500.0, 500.0, 1500.0}));
  REQUIRE(5 == grid2.positionToBin({1500.0, 500.0, 1500.0}));

  REQUIRE(6 == grid.positionToBin({1500.0, 1500.0, 500.0}));
  REQUIRE(6 == grid2.positionToBin({1500.0, 1500.0, 500.0}));

  REQUIRE(7 == grid.positionToBin({1500.0, 1500.0, 1500.0}));
  REQUIRE(7 == grid2.positionToBin({1500.0, 1500.0, 1500.0}));

  //Outside of grid cases

  REQUIRE(0 == grid.positionToBin({-1500.0, -1500.0, -1500.0}));
  REQUIRE(0 == grid2.positionToBin({-1500.0, -1500.0, -1500.0}));

  REQUIRE(7 == grid.positionToBin({2500.0, 2500.0, 2500.0}));
  REQUIRE(7 == grid2.positionToBin({2500.0, 2500.0, 2500.0}));

}











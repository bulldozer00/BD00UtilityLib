#include "catch.hpp"
#include "../CartGrid.hpp"
#include "../CartGrid2.hpp"

class TrackData {};

using namespace cruRrp::msp::topics::util;

constexpr double METERS_PER_NMI{1852.0};

TEST_CASE( "resizeGrid", "[CartGrid2]" ) {

  // 1 NMI X 1 NMI X 1 NMI; 1KM bin sizes
  GridDef xGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};
  GridDef yGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};
  GridDef zGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};

  CartGrid<std::shared_ptr<TrackData>> grid{xGrid, yGrid, zGrid};
  CartGrid2<std::shared_ptr<TrackData>> grid2{xGrid, yGrid, zGrid};

  REQUIRE(8 == grid.getNumBins());
  REQUIRE(8 == grid2.getNumBins());

  //Resize the X dimension to have 4 bins instead of 2
  GridDef xGrid1{0.0, 1.0 * METERS_PER_NMI, 500.0};

  grid.resize(xGrid1, yGrid, zGrid);
  grid2.resize(xGrid1, yGrid, zGrid);

  REQUIRE(16 == grid.getNumBins());
  REQUIRE(16 == grid2.getNumBins());

  //Resize the Y dimension to have 4 bins instead of 2
  GridDef yGrid1{0.0, 1.0 * METERS_PER_NMI, 500.0};

  grid.resize(xGrid1, yGrid1, zGrid);
  grid2.resize(xGrid1, yGrid1, zGrid);

  REQUIRE(32 == grid.getNumBins());
  REQUIRE(32 == grid2.getNumBins());

  //Resize the Z dimension to have 4 bins instead of 2
  GridDef zGrid1{0.0, 1.0 * METERS_PER_NMI, 500.0};

  grid.resize(xGrid1, yGrid1, zGrid1);
  grid2.resize(xGrid1, yGrid1, zGrid1);

  REQUIRE(64 == grid.getNumBins());
  REQUIRE(64 == grid2.getNumBins());

}


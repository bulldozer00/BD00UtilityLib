#include "catch.hpp"
#include "../CartGrid.hpp"
#include "../CartGrid2.hpp"

using namespace cruRrp::msp::topics::util;

class TrackData{};

constexpr double METERS_PER_NMI{1852.0};

TEST_CASE( "insertion", "[CartGrid3]" ) {

  // 1 NMI X 1 NMI X 1 NMI; 1KM bin sizes
  GridDef xGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};
  GridDef yGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};
  GridDef zGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};

  CartGrid<std::shared_ptr<TrackData>> grid{xGrid, yGrid, zGrid};
  CartGrid2<SmartEntry<std::shared_ptr<TrackData>>> grid2{xGrid, yGrid, zGrid};

  REQUIRE(8 == grid.getNumBins());
  REQUIRE(8 == grid2.getNumBins());

  //Add a trk to bin 0
  XyzPosCoords posCoords1{500.0, 500.0, 500.0};
  auto trk1 = std::make_shared<TrackData>();
  REQUIRE(0 == grid.addEntry(posCoords1, trk1));
  SmartEntry<std::shared_ptr<TrackData>> se1{std::make_shared<TrackData>()};
  REQUIRE(0 == grid2.addEntry(posCoords1, se1));

  //Add another trk to bin 0
  XyzPosCoords posCoords2{50.0, 50.0, 50.0};
  auto trk2 = std::make_shared<TrackData>();
  REQUIRE(0 == grid.addEntry(posCoords2, trk2));
  SmartEntry<std::shared_ptr<TrackData>> se2{std::make_shared<TrackData>()};
  REQUIRE(0 == grid2.addEntry(posCoords2, se2));

  //Add a trk to bin 1
  XyzPosCoords posCoords3{50.0, 50.0, 1500.0};
  auto trk3 = std::make_shared<TrackData>();
  REQUIRE(1 == grid.addEntry(posCoords3, trk3));
  SmartEntry<std::shared_ptr<TrackData>> se3{std::make_shared<TrackData>()};
  REQUIRE(1 == grid2.addEntry(posCoords3, se3));

  //Expect 3 tracks in the grid
  REQUIRE(3 == grid.getNumTotalEntries());
  REQUIRE(3 == grid2.getNumTotalEntries());

}

TEST_CASE( "retrieval by pos", "[CartGrid3]" ) {

  // 1 NMI X 1 NMI X 1 NMI; 1KM bin sizes
  GridDef xGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};
  GridDef yGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};
  GridDef zGrid{0.0, 1.0 * METERS_PER_NMI, 1000.0};

  CartGrid<int32_t> grid{xGrid, yGrid, zGrid};
  CartGrid2<SmartEntry<int32_t>> grid2{xGrid, yGrid, zGrid};

  REQUIRE(8 == grid.getNumBins());

  //Add a trk to bin 0
  XyzPosCoords posCoords1{500.0, 500.0, 500.0};
  REQUIRE(0 == grid.addEntry(posCoords1, 200));
  auto x = SmartEntry<int32_t>{200};
  REQUIRE(0 == grid2.addEntry(posCoords1, x));

  //Add another trk to bin 0
  XyzPosCoords posCoords2{50.0, 50.0, 50.0};
  REQUIRE(0 == grid.addEntry(posCoords2, 100));
  x = SmartEntry<int32_t>{100};
  REQUIRE(0 == grid2.addEntry(posCoords2, x));

  //Add another trk to bin 0
   XyzPosCoords posCoords2a{50.0, 50.0, 50.0};
   REQUIRE(0 == grid.addEntry(posCoords2a, 50));
   x = SmartEntry<int32_t>{50};
   REQUIRE(0 == grid2.addEntry(posCoords2a, x));

  //Add a trk to bin 1
  XyzPosCoords posCoords3{50.0, 50.0, 1500.0};
  REQUIRE(1 == grid.addEntry(posCoords3, 300));
  x = SmartEntry<int32_t>{300};
  REQUIRE(1 == grid2.addEntry(posCoords3, x));

  REQUIRE(4 == grid.getNumTotalEntries());
  REQUIRE(4 == grid2.getNumTotalEntries());

  //Get tracks in bin 0
  auto trks0 = grid.getEntriesByPos(posCoords1);
  REQUIRE(3 == trks0.size());
  REQUIRE(200 == trks0[0]);
  REQUIRE(100 == trks0[1]);
  REQUIRE(50 == trks0[2]);

  auto trks00 = grid2.getEntriesByPos(posCoords1);
  REQUIRE(3 == trks00.size());
  auto iter = trks00.begin();
  REQUIRE(200 == iter->entry);
  ++iter;
  REQUIRE(100 == iter->entry);
  ++iter;
  REQUIRE(50 == iter->entry);

  //Get tracks in bin 1
  const auto& trks1 = grid.getEntriesByPos(posCoords3);
  REQUIRE(1 == trks1.size());
  const auto& trks01 = grid2.getEntriesByPos(posCoords3);
  REQUIRE(1 == trks01.size());

  grid.clearAllEntries();
  REQUIRE(0 == grid.getNumTotalEntries());
  grid2.clearAllEntries();
  REQUIRE(0 == grid2.getNumTotalEntries());

}


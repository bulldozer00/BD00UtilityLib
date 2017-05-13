#include "catch.hpp"
#include "../GridTypes.h"
#include "../CartGrid.hpp"
#include "../CartGrid2.hpp"
#include <chrono>
#include <iostream>
#include <cstdint>

using namespace cruRrp::msp::topics::util;
using namespace std::chrono;

constexpr int32_t NUM_ENTRIES_PER_BIN{5000};

//200 bins per dimension (8M bins total)
constexpr double MIN{-5000000.0};
constexpr double MAX{5000000.0};
constexpr double BIN_SIZE{50000};

constexpr double SEARCH_X{MAX};
constexpr double SEARCH_Y{MAX};
constexpr double SEARCH_Z{MAX};

TEST_CASE( "Smart Entry Grid Perf", "[PerfCompTest]" ) {

  GridDef xGrid{MIN, MAX, BIN_SIZE};
  GridDef yGrid{MIN, MAX, BIN_SIZE};
  GridDef zGrid{MIN, MAX, BIN_SIZE};

  //Create a Grid2
  CartGrid2<SmartEntry<int32_t>> gridList{xGrid, yGrid, zGrid};
  REQUIRE(8000000 == gridList.getNumBins());

  auto tstart = system_clock::now();

  //add NUM_ENTRIES entries to the grid at {0, 0, 0}
  for(int32_t i=0; i<NUM_ENTRIES_PER_BIN; ++i) {
    gridList.addEntry({0.0, 0.0, 0.0}, SmartEntry<int32_t>{i});
  }

  //add NUM_ENTRIES entries to the grid at {500.0, 0, 0}
  for(int32_t i=0; i<NUM_ENTRIES_PER_BIN; ++i) {
    gridList.addEntry({MAX/2.0, 0.0, 0.0}, SmartEntry<int32_t>{i});
  }

  //add NUM_ENTRIES entries to the grid at {0.0, 500.0, 0}
  for(int32_t i=0; i<NUM_ENTRIES_PER_BIN; ++i) {
    gridList.addEntry({0.0, MAX/2.0, 0.0}, SmartEntry<int32_t>{i});
  }

  //add NUM_ENTRIES entries to the grid at {0.0, 0.0, 500.0}
  for(int32_t i=0; i<NUM_ENTRIES_PER_BIN; ++i) {
    gridList.addEntry({0.0, 0.0, MAX/2.0}, SmartEntry<int32_t>{i});
  }

  //search the whole grid
  const auto& results1 = gridList.getEntriesBySearchExtent({0.0, 0.0, 0.0}, {SEARCH_X, SEARCH_Y, SEARCH_Z});
  REQUIRE(20000 ==  results1.size());

  //search one quarter of the grid
  const auto& results2 = gridList.getEntriesBySearchExtent({0.0, 0.0, 0.0}, {SEARCH_X/4.0, SEARCH_Y/4.0, SEARCH_Z/4.0});
  REQUIRE(5000 ==  results2.size());

  //search one quad of the grid
  const auto& results3 = gridList.getEntriesBySearchExtent({0.0, 0.0, 0.0}, {SEARCH_X, 0.0, 0.0});
  REQUIRE(10000 ==  results3.size());

  gridList.clearAllEntries();

  auto tend = system_clock::now();
  std::cout << "list grid perf = "
            << duration_cast<microseconds>(tend - tstart).count()
            << " micros"
            << std::endl;

}

TEST_CASE( "Dumb Entry Grid Perf", "[PerfCompTest]" ) {

  GridDef xGrid{MIN, MAX, BIN_SIZE};
  GridDef yGrid{MIN, MAX, BIN_SIZE};
  GridDef zGrid{MIN, MAX, BIN_SIZE};

  //Create a Grid
  CartGrid<int32_t> gridVec{xGrid, yGrid, zGrid};

  REQUIRE(8000000 == gridVec.getNumBins());

  auto tstart = system_clock::now();

  //add NUM_ENTRIES entries to the grid at {0, 0, 0}
  for(int32_t i=0; i<NUM_ENTRIES_PER_BIN; ++i) {
    gridVec.addEntry({0.0, 0.0, 0.0}, i);
  }

  //add NUM_ENTRIES entries to the grid at {500.0, 0, 0}
  for(int32_t i=0; i<NUM_ENTRIES_PER_BIN; ++i) {
    gridVec.addEntry({MAX/2.0, 0.0, 0.0}, i);
  }

  //add NUM_ENTRIES entries to the grid at {0.0, 500.0, 0}
  for(int32_t i=0; i<NUM_ENTRIES_PER_BIN; ++i) {
    gridVec.addEntry({0.0, MAX/2.0, 0.0}, i);
  }

  //add NUM_ENTRIES entries to the grid at {0.0, 0.0, 500.0}
  for(int32_t i=0; i<NUM_ENTRIES_PER_BIN; ++i) {
    gridVec.addEntry({0.0, 0.0, MAX/2.0}, i);
  }

  //search the whole grid
  const auto& results1 = gridVec.getEntriesBySearchExtent({0.0, 0.0, 0.0}, {SEARCH_X, SEARCH_Y, SEARCH_Z});
  REQUIRE(20000 ==  results1.size());

  //search one quarter of the grid
  const auto& results2 = gridVec.getEntriesBySearchExtent({0.0, 0.0, 0.0}, {SEARCH_X/4.0, SEARCH_Y/4.0, SEARCH_Z/4.0});
  REQUIRE(5000 ==  results2.size());

  //search one quad of the grid
  const auto& results3 = gridVec.getEntriesBySearchExtent({0.0, 0.0, 0.0}, {SEARCH_X, 0.0, 0.0});
  REQUIRE(10000 ==  results3.size());

  gridVec.clearAllEntries();

  auto tend = system_clock::now();
  std::cout << "vec grid perf = "
            << duration_cast<microseconds>(tend - tstart).count()
            << " micros"
            << std::endl;

}



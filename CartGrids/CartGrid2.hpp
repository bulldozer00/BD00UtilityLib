
#ifndef CARTGRID2_HPP_
#define CARTGRID2_HPP_

#include "CartGrid2.h"

namespace cruRrp {
  namespace msp {
    namespace topics {
      namespace util {

template<typename SmartEntry>
CartGrid2<SmartEntry>::CartGrid2(const GridDef& xGridDef, const GridDef& yGridDef, const GridDef& zGridDef) {

    resize(xGridDef, yGridDef, zGridDef);

}

template<typename SmartEntry>
void CartGrid2<SmartEntry>::resize(const GridDef& xGridDef, const GridDef& yGridDef, const GridDef& zGridDef) {

  std::lock_guard<std::recursive_mutex> lg{_mtx};

  _xGridDef = xGridDef;

  _xBinner.resize(xGridDef);

  _numXbins = _xBinner.getNumBins();

  _yGridDef = yGridDef;

  _yBinner.resize(yGridDef);

  _numYbins = _yBinner.getNumBins();

  _zGridDef = zGridDef;

  _zBinner.resize(zGridDef);

  _numZbins = _zBinner.getNumBins();

  _numTotalBins = _numXbins * _numYbins * _numZbins;

  _entries.clear();

  _entries.resize(_numTotalBins);

  _occupiedBins.clear();

}

template<typename SmartEntry>
int32_t CartGrid2<SmartEntry>::addEntry(const XyzPosCoords& pos, const SmartEntry& entry) {

  int32_t binIdx{positionToBin(pos)};

  //make a copy of the user supplied object
  SmartEntry se{entry};

  //store the bin index in it
  se.binIdx = binIdx;

  std::lock_guard<std::recursive_mutex> lg{_mtx};

  //insert returns an iterator to the object just inserted into the list
  auto iter = _entries[binIdx].insert(_entries[binIdx].end(), se);

  //make the iterator in the object point to its position in the list
  _entries[binIdx].back().iter = iter;

  //mark the bin as occupied
  _occupiedBins.insert(binIdx);

  return binIdx;

}

template<typename SmartEntry>
const std::vector<SmartEntry>& CartGrid2<SmartEntry>::getEntriesByPos(const XyzPosCoords& pos) {

  std::lock_guard<std::recursive_mutex> lg{_mtx};

  _searchResults.clear();

  std::copy(_entries[positionToBin(pos)].begin(),
            _entries[positionToBin(pos)].end(),
            std::back_inserter(_searchResults));

  return _searchResults;

}

template<typename SmartEntry>
void CartGrid2<SmartEntry>::removeEntry(const SmartEntry& smartEntry) {

  std::lock_guard<std::recursive_mutex> lg{_mtx};

  //go directly to the right bin and right entry in the bin
  _entries[smartEntry.binIdx].erase(smartEntry.iter);

}

template<typename SmartEntry>
const std::vector<SmartEntry>& CartGrid2<SmartEntry>::getEntriesBySearchExtent(const XyzPosCoords& refPos, const XyzPosCoords& searchExtent) {

  std::lock_guard<std::recursive_mutex> lg{_mtx};

  //For each dimension, convert the search extent into hard-limited start & stop bins

  //X dimension first

  double xStartMeters{refPos.xMeters - searchExtent.xMeters};
  xStartMeters = (xStartMeters < _xGridDef._minMeters) ? _xGridDef._minMeters : xStartMeters;

  double xStopMeters{refPos.xMeters + searchExtent.xMeters};
  xStopMeters = (xStopMeters > _xGridDef._maxMeters) ? _xGridDef._maxMeters : xStopMeters;

  int32_t xStartBin{_xBinner(xStartMeters)};

  int32_t xStopBin{_xBinner(xStopMeters)};

  //Y dimension next

  double yStartMeters{refPos.yMeters - searchExtent.yMeters};
  yStartMeters = (yStartMeters < _yGridDef._minMeters) ? _yGridDef._minMeters : yStartMeters;

  double yStopMeters{refPos.yMeters + searchExtent.yMeters};
  yStopMeters = (yStopMeters > _yGridDef._maxMeters) ? _yGridDef._maxMeters : yStopMeters;

  int32_t yStartBin{_yBinner(yStartMeters)};

  int32_t yStopBin{_yBinner(yStopMeters)};

  //Z dimension last

  double zStartMeters{refPos.zMeters - searchExtent.yMeters};
  zStartMeters = (zStartMeters < _zGridDef._minMeters) ? _zGridDef._minMeters : zStartMeters;

  double zStopMeters{refPos.zMeters + searchExtent.yMeters};
  zStopMeters = (zStopMeters > _zGridDef._maxMeters) ? _zGridDef._maxMeters : zStopMeters;

  int32_t zStartBin{_zBinner(zStartMeters)};

  int32_t zStopBin{_zBinner(zStopMeters)};

  //Create our container for "moving" our results out of our local scope
  //and into the caller's scope
  _searchResults.clear();

  for(int32_t x=xStartBin; x<=xStopBin; x++) {
    for(int32_t y=yStartBin; y<=yStopBin; y++) {
      for(int32_t z=zStartBin; z<=zStopBin; z++) {

        int32_t vecIdx{binToVecIdx({x, y, z})};

        std::copy(_entries[vecIdx].begin(),
                  _entries[vecIdx].end(),
                   std::back_inserter(_searchResults));

      }  // end z
    }  // end y
  }  // end x

  return _searchResults;

}

template<typename SmartEntry>
int32_t CartGrid2<SmartEntry>::getNumBins() const {

  std::lock_guard<std::recursive_mutex> lg{_mtx};

  return static_cast<int32_t>(_entries.size());

}

template<typename SmartEntry>
int32_t CartGrid2<SmartEntry>::getNumTotalEntries() const {

  int32_t count{};

  std::lock_guard<std::recursive_mutex> lg{_mtx};

  std::for_each(_occupiedBins.begin(), _occupiedBins.end(),
      [&](int32_t binIdx) { count += _entries[binIdx].size();});


  return count;

}

template<typename SmartEntry>
void CartGrid2<SmartEntry>::clearAllEntries() {

  std::lock_guard<std::recursive_mutex> lg{_mtx};

  std::for_each(_occupiedBins.begin(), _occupiedBins.end(),
      [&](int32_t binIdx) { _entries[binIdx].clear();});

  _occupiedBins.clear();

}

template<typename SmartEntry>
int32_t CartGrid2<SmartEntry>::positionToBin(const XyzPosCoords& pos) const {

  XyzBinCoords binCoords{};

  std::lock_guard<std::recursive_mutex> lg{_mtx};

  binCoords.xBin = _xBinner(pos.xMeters);

  binCoords.yBin = _yBinner(pos.yMeters);

  binCoords.zBin = _xBinner(pos.zMeters);

  return binToVecIdx(binCoords);

}

template<typename SmartEntry>
int32_t CartGrid2<SmartEntry>::binToVecIdx(const XyzBinCoords& gridPos) const {

  std::lock_guard<std::recursive_mutex> lg{_mtx};

  //reference: "Memory layout of multi-dimensional arrays"
  //Equation is for row-major layout (since C++ specifies row-major layout for multi-dimensional arrays)
  //http://eli.thegreenplace.net/2015/memory-layout-of-multi-dimensional-arrays/

  return gridPos.zBin + _numZbins * (gridPos.yBin + (_numYbins * gridPos.xBin));

}

      } // namespace util
    } // namespace topics
  } // namespace msp
} // namespace cruRrp

#endif /* CARTGRID2_HPP_ */

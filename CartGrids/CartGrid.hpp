
#ifndef CARTGRID_HPP_
#define CARTGRID_HPP_

#include "CartGrid.h"

namespace cruRrp {
  namespace msp {
    namespace topics {
      namespace util {

template<typename Entry>
CartGrid<Entry>::CartGrid(const GridDef& xGridDef, const GridDef& yGridDef, const GridDef& zGridDef) {

    resize(xGridDef, yGridDef, zGridDef);

}

template<typename Entry>
void CartGrid<Entry>::resize(const GridDef& xGridDef, const GridDef& yGridDef, const GridDef& zGridDef) {

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

template<typename Entry>
int32_t CartGrid<Entry>::addEntry(const XyzPosCoords& pos, const Entry& entry) {

  int32_t binIdx{positionToBin(pos)};

  _entries[binIdx].emplace_back(entry);

  _occupiedBins.insert(binIdx);

  return binIdx;

}

template<typename Entry>
const std::vector<Entry>& CartGrid<Entry>::getEntriesBySearchExtent(const XyzPosCoords& refPos, const XyzPosCoords& searchExtent) {

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

template<typename Entry>
const std::vector<Entry>& CartGrid<Entry>::getEntriesByPos(const XyzPosCoords& pos) const {

  return _entries[positionToBin(pos)];

}

template<typename Entry>
int32_t CartGrid<Entry>::getNumBins() const {

  return static_cast<int32_t>(_entries.size());

}

template<typename Entry>
int32_t CartGrid<Entry>::getNumTotalEntries() const {

  int32_t count{};

  std::for_each(_occupiedBins.begin(), _occupiedBins.end(),
      [&](int32_t binIdx) { count += _entries[binIdx].size();});

  return count;

}

template<typename Entry>
void CartGrid<Entry>::clearAllEntries() {

  std::for_each(_occupiedBins.begin(), _occupiedBins.end(),
      [&](int32_t binIdx) { _entries[binIdx].clear();});

  _occupiedBins.clear();

}

template<typename Entry>
int32_t CartGrid<Entry>::positionToBin(const XyzPosCoords& pos) const {

  XyzBinCoords binCoords{};

  binCoords.xBin = _xBinner(pos.xMeters);

  binCoords.yBin = _yBinner(pos.yMeters);

  binCoords.zBin = _xBinner(pos.zMeters);

  return binToVecIdx(binCoords);

}

template<typename Entry>
int32_t CartGrid<Entry>::binToVecIdx(const XyzBinCoords& gridPos) const {

  //reference: "Memory layout of multi-dimensional arrays"
  //Equation is for row-major layout (since C++ specifies row-major layout for multi-dimensional arrays)
  //http://eli.thegreenplace.net/2015/memory-layout-of-multi-dimensional-arrays/

  return gridPos.zBin + _numZbins * (gridPos.yBin + (_numYbins * gridPos.xBin));

}

      } // namespace util
    } // namespace topics
  } // namespace msp
} // namespace cruRrp

#endif /* CARTGRID_HPP_ */

#ifndef GRIDTYPES_H_
#define GRIDTYPES_H_
#include <cstdint>
#include <list>

namespace cruRrp {
  namespace msp {
    namespace topics {
      namespace util {

template<typename Entry>
struct SmartEntry {

  SmartEntry(const Entry& entryVal, int32_t binIdxVal = 0,
             typename std::list<SmartEntry>::iterator iterVal = typename std::list<SmartEntry>::iterator()) :
    entry(entryVal), binIdx(binIdxVal), iter(iterVal) {

  }

  Entry entry; //value
  int32_t binIdx; //bin in grid
  typename std::list<SmartEntry>::iterator iter; //position in bin

};


struct GridDef {

  GridDef(double minMeters, double maxMeters, double deltaBinMeters) :
    _minMeters(minMeters), _maxMeters(maxMeters), _deltaBinMeters(deltaBinMeters) {

  }

  GridDef() :
    _minMeters(0.0), _maxMeters(0.0), _deltaBinMeters(1.0) {

  }

  double _minMeters;
  double _maxMeters;
  double _deltaBinMeters;

};


struct XyzBinCoords {
  int32_t  xBin;
  int32_t  yBin;
  int32_t  zBin;
};

struct XyzPosCoords {
  double xMeters;
  double yMeters;
  double zMeters;
};

      } // namespace util
    } // namespace topics
  } // namespace msp
} // namespace cruRrp

#endif /* GRIDTYPES_H_ */

#ifndef BINNER_H_
#define BINNER_H_

#include <cmath>
#include "GridTypes.h"

namespace cruRrp {
  namespace msp {
    namespace topics {
      namespace util {

class Binner {
public:

  Binner() {

    resize({0.0, 0.0, 1.0});

  }

  explicit Binner(const GridDef& gridDef) {

    resize(gridDef);

  }

  void resize(const GridDef& gridDef) {

    _min = gridDef._minMeters;

    _max = gridDef._maxMeters;

    _delta = gridDef._deltaBinMeters;

    //TODO: throw if |_delta| < EPSILON

    _numBins = std::ceil((_max - _min) / _delta);

    _maxBinIdx = _numBins - 1;

    _firstBinEnd = _min + _delta;

    _lastBinStart = _max - _delta;

  }

  int32_t operator()(double value) const {

    int32_t bin{};

    if(value < _firstBinEnd) {

      bin = 0;

    }
    else if(value >= _lastBinStart) {

      bin = _maxBinIdx;

    }
    else {

      bin = static_cast<int32_t>((value - _min)/_delta);

    }

    return bin;

  }

  int32_t getNumBins() const {

    return _numBins;

  }

private:
  double _min;
  double _max;
  int32_t _numBins;
  int32_t _maxBinIdx;
  double _delta;
  double _firstBinEnd;
  double _lastBinStart;
};

      } // namespace util
    } // namespace topics
  } // namespace msp
} // namespace cruRrp

#endif /* BINNER_H_ */

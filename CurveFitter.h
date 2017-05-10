/*
 *  CurveFitter.h
 *
 *  Created on: Apr 22, 2017
 *      Author: adasi_000
 */

#ifndef CURVEFITTER_H_
#define CURVEFITTER_H_

#include <stdexcept>
#include <vector>
#include <cstdint>
#include <initializer_list>

//first == x coord, second == y coord
//first == line slope, second == y intercept
using DataPt = std::pair<double, double>;

class CurveFitter {
public:
  CurveFitter();
  CurveFitter(std::initializer_list<DataPt> inList);
  DataPt addDataPts(std::initializer_list<DataPt> inList);
  void clear();

private:
  std::vector<DataPt> _dataPts{};
  double _slope{};
  double _yIntercept{};
  int32_t _numPts{};

  DataPt fitLineToToData();
};

class VCurveFitterException : public std::runtime_error {
public:
  explicit VCurveFitterException (const char* what) :
    std::runtime_error(what) {

  }
};

#endif /* CURVEFITTER_H_ */

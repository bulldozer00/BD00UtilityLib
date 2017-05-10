/*
 *  CurveFitter.cpp
 *
 *  Created on: Apr 22, 2017
 *      Author: adasi_000
 */

#include "CurveFitter.h"

CurveFitter::CurveFitter() {
  fitLineToToData();
}

CurveFitter::CurveFitter(std::initializer_list<DataPt> inList) {
  addDataPts(inList);
}

DataPt CurveFitter::addDataPts(std::initializer_list<DataPt> inList) {
  for(const auto& el :  inList) {
    _dataPts.emplace_back(el);
    ++_numPts;
  }
  return fitLineToToData();
}

DataPt CurveFitter::fitLineToToData() {

  if (_numPts == 0)
    throw VCurveFitterException("No Data Points!");

  double sumX{};
  double sumY{};
  double sumX2{};
  double sumXY{};

  for(const auto& dp : _dataPts) {
    sumX += dp.first;
    sumY += dp.second;
    sumX2 += dp.first * dp.first;
    sumXY += dp.first * dp.second;
  }

  _slope = ((_numPts * sumXY) -  (sumX * sumY))
      / ((_numPts * sumX2) - (sumX * sumX));

  _yIntercept = (sumY - (_slope * sumX)) / _numPts;

  return {_slope, _yIntercept};

}

void CurveFitter::clear() {
  _dataPts.clear();
  _slope = 0.0;
  _yIntercept = 0.0;
  _numPts = 0;

}


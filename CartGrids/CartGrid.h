#ifndef CARTGRID_H_
#define CARTGRID_H_

#include "Binner.h"
#include "GridTypes.h"
#include <vector>
#include <set>
#include <mutex>

namespace cruRrp {
  namespace msp {
    namespace topics {
      namespace util {

//--------------------------------------------------------------------------------
//! @class CartGrid
//! @brief Implements a 3 dimensional cartesian grid
//! @tparam Entry the type of object stored in a bin's Container
//--------------------------------------------------------------------------------
template<typename Entry>
class CartGrid {

public:

  //--------------------------------------------------------------------------------
  //! @brief Constructs CartGrid object
  //! @param[in] xGridDef defines the extent and resolution of the X dimension
  //! @param[in] yGridDef defines the extent and resolution of the Y dimension
  //! @param[in] zGridDef defines the extent and resolution of the Z dimension
  //--------------------------------------------------------------------------------
  CartGrid(const GridDef& xGridDef, const GridDef& yGridDef, const GridDef& zGridDef);

  //--------------------------------------------------------------------------------
  //! @brief Adds an Entry to the grid based on a user-supplied position
  //! @param[in] pos The position of the entry in cartesian coords
  //! @param[in] entry A handle to an entry to be copied into the grid
  //! @return The bin number in which the Entry was inserted
  //--------------------------------------------------------------------------------
  int32_t addEntry(const XyzPosCoords& pos, const Entry& entry);

  //--------------------------------------------------------------------------------
  //! @brief Returns A container of merged Entries surrounding a reference
  //! cartesian position supplied by the user
  //! @param[in] refPos defines the cartesian position of the reference point
  //! @param[in] searchExtent defines the search extent around the reference point
  //! @return A container of merged Entries
  //--------------------------------------------------------------------------------
  const std::vector<Entry>& getEntriesBySearchExtent(const XyzPosCoords& refPos, const XyzPosCoords& searchExtent);

  //--------------------------------------------------------------------------------
  //! @brief Returns A handle to the Container of Entries residing in the bin
  //! associated with the cartesian coordinates supplied by the user
  //! @param[in] pos defines the cartesian position of the grid entry of interest
  //! @return A handle to the Container of Entries
  //--------------------------------------------------------------------------------
  const std::vector<Entry>& getEntriesByPos(const XyzPosCoords& pos) const;

  //--------------------------------------------------------------------------------
  //! @brief Returns the total number of bins in the grid
  //! @return The number of bins in the grid
  //--------------------------------------------------------------------------------
  int32_t getNumBins() const;

  //--------------------------------------------------------------------------------
  //! @brief Converts a cartesian position trio (x,y,z) into a single bin number
  //! @param[in] pos defines the cartesian position to be converted
  //! @return The bin number associated with the position trio
  //--------------------------------------------------------------------------------
  int32_t positionToBin(const XyzPosCoords& pos) const;

  //--------------------------------------------------------------------------------
  //! @brief Converts a bin trio (x,y,z) into a single bin number
  //! @param[in] gridPos defines the grid position to be converted
  //! @return The bin number associated with the bin trio
  //--------------------------------------------------------------------------------
  int32_t binToVecIdx(const XyzBinCoords& gridPos) const;

  //--------------------------------------------------------------------------------
  //! @brief Clears/flushes out all the entries and resizes the grid
  //! @param[in] xGridDef defines the extent and resolution of the new X dimension
  //! @param[in] yGridDef defines the extent and resolution of the new Y dimension
  //! @param[in] zGridDef defines the extent and resolution of the new Z dimension
  //! @return none
  //--------------------------------------------------------------------------------
  void resize(const GridDef& xGridDef, const GridDef& yGridDef, const GridDef& zGridDef);

  //--------------------------------------------------------------------------------
  //! @brief returns the total number of entries residing in the grid
  //! @return The total number of entries in the grid
  //-------------------------------------------------------------------------------
  int32_t getNumTotalEntries() const;

  //--------------------------------------------------------------------------------
  //! @brief Clears out all the entries in the Container of each of the occupied bins
  //! @return none
  //--------------------------------------------------------------------------------
  void clearAllEntries();

private:

  //! The grid extents for the X-dimension
  GridDef _xGridDef;

  //! The functor for converting an X cartesian coordinate into an X bin index
  Binner _xBinner;

  //! The number of bins covering the X dimension
  int32_t _numXbins;

  //! The grid extents for the Y-dimension
  GridDef _yGridDef;

  //! The functor for converting an Y cartesian coordinate into an Y bin index
  Binner _yBinner;

  //! The number of bins covering the Y dimension
  int32_t _numYbins;

  //! The grid extents for the Z-dimension
  GridDef _zGridDef;

  //! The functor for converting an Z cartesian coordinate into an Z bin index
  Binner _zBinner;

  //! The number of bins covering the Z dimension
  int32_t _numZbins;

  //! The total number of bins in the 3D grid
  int32_t _numTotalBins;

  //! @brief The internal representation. It represents a 3D grid flattened
  //! onto a linear std::vector stored in contiguous memory. Each bin
  //! in the grid houses a container of 0 or more Entry objects
  std::vector<std::vector<Entry>> _entries;

  //! A list of all the bin Ids that contain one or more Entries
  std::set<int32_t> _occupiedBins;

  //! Holds the search results
  std::vector<Entry> _searchResults;

};

      } // namespace util
    } // namespace topics
  } // namespace msp
} // namespace cruRrp




#endif /* CARTGRID_H_ */

//
//  Filename         : FastGrid.h
//  Author(s)        : Stephane Grabli
//  Purpose          : Class to define a cell grid surrounding the
//                     bounding box of the scene
//  Date of creation : 30/07/2002
//
///////////////////////////////////////////////////////////////////////////////


//
//  Copyright (C) : Please refer to the COPYRIGHT file distributed 
//   with this source distribution. 
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef  FASTGRID_H
# define FASTGRID_H

# include <set>
# include "Grid.h"
# include <cassert>
/*! Class to define a regular grid used for ray
 *  casting computations 
 *  We don't use a hashtable here. The grid is 
 *  explicitly stored for faster computations.
 *  However, this might result in significant
 *  increase in memory usage (compared to the regular grid)
 */

class LIB_GEOMETRY_EXPORT FastGrid : public Grid
{
 public:

  FastGrid() : Grid() {
    _cells = NULL;
    _cells_size = 0;
  }
  
  virtual ~FastGrid() {
    clear();
  }

  /*! clears the grid
   *  Deletes all the cells, clears the hashtable,
   *  resets size, size of cell, number of cells.
   */
  virtual void clear();
  
  /*! Sets the different parameters of the grid
   *    orig
   *      The grid origin
   *    size
   *      The grid's dimensions
   *    nb
   *      The number of cells of the grid
   */
  virtual void configure(const Vec3r& orig, const Vec3r& size, unsigned nb);
  
  /*! returns the cell whose coordinates are pased as argument */
  Cell* getCell(const Vec3u& p) ;
 
  /*! Fills the case p with the cell iCell */
  virtual void fillCell(const Vec3u& p, Cell& cell);

  // added by aaron for visiting all non-empty cells
  // warning: no error-checking (e.g., assertions) here.  iterators may become invalid if
  // any changes are made to the grid.
  class FGiterator // : public Grid::iterator
  {
  public:
    FGiterator(const set<Vec3u>::iterator &);
    FGiterator(const FGiterator &);
    void operator ++();
    bool operator !=(const FGiterator &) const;
    Vec3u operator *() const;
    virtual Vec3u dereference () const;
  private:
    set<Vec3u>::iterator _setit;
  };

  //  Grid::iterator begin();
  //  Grid::iterator end();

  FGiterator beginFG();
  FGiterator endFG();
  int numNonempty() const { return _nonempty_cells.size(); };

protected:

  Cell**	_cells;
  unsigned	_cells_size;
  
  // added by Aaron in order to keep track of non-empty cells
  set<Vec3u>    _nonempty_cells;
};

#endif // FASTGRID_H

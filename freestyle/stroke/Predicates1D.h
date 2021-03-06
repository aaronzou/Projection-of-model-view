//
//  Filename         : Predicates1D.h
//  Author(s)        : Stephane Grabli, Emmanuel Turquin
//  Purpose          : Class gathering stroke creation algorithms
//  Date of creation : 01/07/2003
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

#ifndef  PREDICATES1D_H
# define PREDICATES1D_H

# include <string>
# include "../system/TimeStamp.h"
# include "../view_map/Interface1D.h"
# include "../view_map/Functions1D.h"
# include "AdvancedFunctions1D.h"

//
// UnaryPredicate1D (base class for predicates in 1D)
//
///////////////////////////////////////////////////////////
/*! Base class for Unary Predicates that work
 *  on Interface1D.
 *  A UnaryPredicate1D is a functor that evaluates
 *  a condition on a Interface1D and returns
 *  true or false depending on whether this condition is
 *  satisfied or not.
 *  The UnaryPredicate1D is used by calling its () operator.
 *  Any inherited class must overload the () operator.
 */
class UnaryPredicate1D
{
public:
  /*! Default constructor. */
  UnaryPredicate1D() {}
  /*! Destructor. */
  virtual ~UnaryPredicate1D() {}
  /*! Returns the string of the name
   *  of the UnaryPredicate1D.
   */
  virtual string getName() const {
    return "UnaryPredicate1D";
  }
  /*! The () operator. Must be overload
   *  by inherited classes.
   *  \param inter
   *    The Interface1D on  which we wish to evaluate
   *    the predicate.
   *  \return true if the condition is satisfied,
   *    false otherwise.
   */
  virtual bool operator()(Interface1D& inter) {
    cerr << "Warning: operator() not implemented" << endl;
    return false;
  }
};


//
// BinaryPredicate1D (base class for predicates in 1D)
//
///////////////////////////////////////////////////////////
/*! Base class for Binary Predicates working on Interface1D.
 *  A BinaryPredicate1D is typically an ordering relation
 *  between two Interface1D.
 *  It evaluates a relation between 2 Interface1D and
 *  returns true or false.
 *  It is used by calling the () operator.
 */
class BinaryPredicate1D
{
public:
  /*! Default constructor. */
  BinaryPredicate1D() {}
  /*! Destructor. */
  virtual ~BinaryPredicate1D() {}
  /*! Returns the string of the name of the
   * binary predicate.
   */
  virtual string getName() const {
    return "BinaryPredicate1D";
  }
  /*! The () operator. Must be overload by inherited classes.
   *  It evaluates a relation between 2 Interface1D.
   *  \param inter1
   *    The first Interface1D.
   *  \param inter2
   *    The second Interface1D.
   *  \return true or false.
   */
  virtual bool operator()(Interface1D& inter1, Interface1D& inter2) {
    cerr << "Warning: operator() not implemented" << endl;
    return false;
  }
};


//
// Predicates definitions
//
///////////////////////////////////////////////////////////

namespace Predicates1D {

  // TrueUP1D
  /*! Returns true */
  class TrueUP1D : public UnaryPredicate1D
  {
  public:
    /*! Constructor */
    TrueUP1D() {}
    /*! Returns the string "TrueUP1D"*/
    string getName() const {
      return "TrueUP1D";
    }
    /*! the () operator */
    bool operator()(Interface1D&) {
      return true;
    }
  };

  // FalseUP1D
  /*! Returns false */
  class FalseUP1D : public UnaryPredicate1D
  {
  public:
    /*! Constructor */
    FalseUP1D() {}
    /*! Returns the string "FalseUP1D"*/
    string getName() const {
      return "FalseUP1D";
    }
    /*! the () operator */
    bool operator()(Interface1D&) {
      return false;
    }
  };

  // QuantitativeInvisibilityUP1D
  /*! Returns true if the Quantitative Invisibility evaluated
   *  at an Interface1D, using the QuantitativeInvisibilityF1D
   *  functor, equals a certain user-defined value.
   */
  class QuantitativeInvisibilityUP1D : public UnaryPredicate1D
  {
  public:
    /*! Builds the Predicate.
     *  \param qi
     *    The Quantitative Invisibility you want
     *    the Interface1D to have
     */
    QuantitativeInvisibilityUP1D(unsigned qi = 0) : _qi(qi) {}
    /*! Returns the string "QuantitativeInvisibilityUP1D"*/
    string getName() const {
      return "QuantitativeInvisibilityUP1D";
    }
    /*! the () operator */
    bool operator()(Interface1D& inter) {
      Functions1D::QuantitativeInvisibilityF1D func;
      return (func(inter) == _qi);
    }
  private:
    unsigned _qi;
  };

  // ContourUP1D
  /*! Returns true if the Interface1D is a contour.
   *  An Interface1D is a contour if it is borded
   *  by a different shape on each of its sides.
   */
  class ContourUP1D : public UnaryPredicate1D
  {
  private:
    Functions1D::CurveNatureF1D _getNature;
  public:
    /*! Returns the string "ContourUP1D"*/
    string getName() const {
      return "ContourUP1D";
    }
    /*! The () operator. */
    bool operator()(Interface1D& inter) {
      if((_getNature(inter) & Nature::SILHOUETTE) || (_getNature(inter) & Nature::BORDER)){
        Interface0DIterator it=inter.verticesBegin();
        for(; !it.isEnd(); ++it){
          if(Functions0D::getOccludeeF0D(it) != Functions0D::getShapeF0D(it))
            return true;
        }
      }
      return false;
    }
  };

  // ExternalContourUP1D
  /*! Returns true if the Interface1D is an external contour.
   *  An Interface1D is an external contour if it is borded
   *  by no shape on one of its sides.
   */
  class ExternalContourUP1D : public UnaryPredicate1D
  {
  private:
    Functions1D::CurveNatureF1D _getNature;
  public:
    /*! Returns the string "ExternalContourUP1D"*/
    string getName() const {
      return "ExternalContourUP1D";
    }
    /*! The () operator. */
    bool operator()(Interface1D& inter) {
      if((_getNature(inter) & Nature::SILHOUETTE) || (_getNature(inter) & Nature::BORDER)){
        set<ViewShape*> occluded;
        Functions1D::getOccludeeF1D(inter, occluded);
        for(set<ViewShape*>::iterator os=occluded.begin(), osend=occluded.end();
            os!=osend;
            ++os){
          if((*os) == 0)
            return true;
        }
      }
      return false;
    }
  };

  // EqualToTimeStampUP1D
  /*! Returns true if the Interface1D's time stamp
   *  is equal to a certain user-defined value.
   */
  class EqualToTimeStampUP1D : public UnaryPredicate1D
  {
  protected:
    unsigned _timeStamp; 
  public:
    EqualToTimeStampUP1D(unsigned ts) : UnaryPredicate1D(){
      _timeStamp = ts;
    }
    /*! Returns the string "EqualToTimeStampUP1D"*/
    string getName() const {
      return "EqualToTimeStampUP1D";
    }
    /*! The () operator. */
    bool operator()(Interface1D& inter) {
      return (inter.getTimeStamp() == _timeStamp);
    }
  };

  // EqualToChainingTimeStampUP1D
  /*! Returns true if the Interface1D's time stamp
   *  is equal to a certain user-defined value.
   */
  class EqualToChainingTimeStampUP1D : public UnaryPredicate1D
  {
  protected:
    unsigned _timeStamp; 
  public:
    EqualToChainingTimeStampUP1D(unsigned ts) : UnaryPredicate1D(){
      _timeStamp = ts;
    }
    /*! Returns the string "EqualToChainingTimeStampUP1D"*/
    string getName() const {
      return "EqualToChainingTimeStampUP1D";
    }
    /*! The () operator. */
    bool operator()(Interface1D& inter) {
      ViewEdge* edge = dynamic_cast<ViewEdge*>(&inter);
      if (!edge)
	return false;
      return (edge->getChainingTimeStamp() >= _timeStamp);
    }
  };

  // ShapeUP1D
  /*! Returns true if the shape to which the Interface1D
   *  belongs to has the same Id as the one specified by the
   *  user.
   */
  class ShapeUP1D: public UnaryPredicate1D
  {
  private:
    Id _id;
  public:
    /*! Builds the Predicate.
     *  \param idFirst
     *    The first Id component.
     *  \param idSecond
     *    The second Id component.
     */
    ShapeUP1D(unsigned idFirst, unsigned idSecond=0) 
      : UnaryPredicate1D(){
      _id = Id(idFirst, idSecond);
    }
    /*! Returns the string "ShapeUP1D"*/
    string getName() const {
      return "ShapeUP1D";
    }
    /*! The () operator. */
    bool operator()(Interface1D& inter) {
      set<ViewShape*> shapes;
      Functions1D::getShapeF1D(inter, shapes);
      for(set<ViewShape*>::iterator s=shapes.begin(), send=shapes.end();
      s!=send;
      ++s){
        if((*s)->getId() == _id)
          return true;
      }
      return false;
    }
  };
  
  //
  //   Binary Predicates definitions
  //
  ///////////////////////////////////////////////////////////

  // TrueBP1D
  /*! Returns true. */
  class TrueBP1D : public BinaryPredicate1D
  {
  public:
    /*! Returns the string "TrueBP1D"*/
    string getName() const {
      return "TrueBP1D";
    }
    /*! The () operator. */
    bool operator()(Interface1D& i1, Interface1D& i2) {
      return true;
    }
  };

  // FalseBP1D
    /*! Returns false. */
  class FalseBP1D : public BinaryPredicate1D
  {
  public:
    /*! Returns the string "FalseBP1D"*/
    string getName() const {
      return "FalseBP1D";
    }
    /*! The () operator. */
    bool operator()(Interface1D& i1, Interface1D& i2) {
      return false;
    }
  };
  
  // Length2DBP1D
  /*! Returns true if the 2D length of the Interface1D i1
   *  is less than the 2D length of the Interface1D i2.
   */
  class Length2DBP1D : public BinaryPredicate1D
  {
  public:
    /*! Returns the string "Length2DBP1D"*/
    string getName() const {
      return "Length2DBP1D";
    }
    /*! The () operator. */
    bool operator()(Interface1D& i1, Interface1D& i2) {
      return (i1.getLength2D() > i2.getLength2D());
    }
  };

  // SameShapeIdBP1D
  /*! Returns true if the Interface1D i1 and i2 belong
   *  to the same shape.
   */
  class SameShapeIdBP1D : public BinaryPredicate1D
  {
  public:
    /*! Returns the string "SameShapeIdBP1D"*/
    string getName() const {
      return "SameShapeIdBP1D";
    }
    /*! The () operator. */
    bool operator()(Interface1D& i1, Interface1D& i2) {
      set<ViewShape*> shapes1;
      Functions1D::getShapeF1D(i1, shapes1);
      set<ViewShape*> shapes2;
      Functions1D::getShapeF1D(i2, shapes2);
      // FIXME:// n2 algo, can do better...
      for(set<ViewShape*>::iterator s=shapes1.begin(), send=shapes1.end();
          s!=send;
          ++s){
        Id current = (*s)->getId();
        for(set<ViewShape*>::iterator s2=shapes2.begin(), s2end=shapes2.end();
          s2!=s2end;
          ++s2){
          if((*s2)->getId() == current)
            return true;
        }
      }
      return false;
    }
  };

  // ViewMapGradientNormBP1D
  /*! Returns true if the evaluation of the 
   *  Gradient norm Function is higher for Interface1D i1 
   *  than for i2.
   */
  class ViewMapGradientNormBP1D : public BinaryPredicate1D
  {
  private:
    Functions1D::GetViewMapGradientNormF1D _func;
  public:
    ViewMapGradientNormBP1D(int level, IntegrationType iType=MEAN, float sampling=2.0) 
      : BinaryPredicate1D(), _func(level, iType, sampling) {
    }
    /*! Returns the string "ViewMapGradientNormBP1D"*/
    string getName() const {
      return "ViewMapGradientNormBP1D";
    }
    /*! The () operator. */
    bool operator()(Interface1D& i1, Interface1D& i2) {
      return (_func(i1) > _func(i2));
    }
  };
} // end of namespace Predicates1D

#endif // PREDICATES1D_H

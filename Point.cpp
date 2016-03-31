//
// Created by Owner on 2/15/2016.
//

#include <cmath>
#include "Point.h"
#include "Exceptions.h"
#include <fstream>
#include <sstream>
#include <string>
using std::string;
using std::stringstream;

#include <cstdlib>

namespace Clustering {

    unsigned int Point::__idGen = 0; // id generator
    const char Point::POINT_VALUE_DELIM = ','; // for use in Point::operator>>

// rewindIDGen function
// Used to rewind the Point __idGen(erator) when a DimensionalityMismatchEx is caught in Cluster::operator>> when
//          a point cannot read itself. Ensures that the Point(s) are sequential and there are no gaps in the
//          sequence due to data file reading problems.
// Precondition: A DimensionalityMismatchEx is thrown in Cluster::operator>>
// Postcondition: the __idGen has 1 subtracted from it.
    void Point::rewindIdGen() {
       __idGen = --__idGen;
    }

//Constructor #1
//Precondition: User has entered amount of dimensions for their points
//PostCondition: *__values (array) is initialized to 0.0
    Point::Point(unsigned int dimensions) {
      try {
           if (dimensions == 0){
                  throw ZeroDimensionsEx();
                  }

            __dim = dimensions;
            __values = new double[__dim];
            __id = __idGen++;

            //Initialize m_Value array to 0.0
            for (int index = 0; index < __dim; index++) {
                __values[index] = 0.0;
            }
    } catch (ZeroDimensionsEx &ex){
        throw ex;}
      }

//copy constructor
    Point::Point(const Point &rightSide) {

        __dim = rightSide.__dim;
        __values = new double[__dim];

        for (int index = 0; index < __dim; index++) {
            __values[index] = rightSide.__values[index];
        }
        __id = rightSide.__id;
    }


//Assignment Operator
    Point &Point::operator=(const Point &rightSide) {

        if (*this == rightSide)
        { return *this; }

        try {
            if (__dim != rightSide.__dim) {
                throw DimensionalityMismatchEx(rightSide.__dim, rightSide.__id);
            }

            delete[] __values;

            __dim = rightSide.__dim;
            __values = new double[__dim];
            for (int index = 0; index < __dim; index++) {
                __values[index] = rightSide.__values[index];
            }
            this->__id = rightSide.__id;
        }catch (DimensionalityMismatchEx &ex){
                throw ex;
            }
            return *this;
        }

// Destuctor
    Point::~Point() {
        delete [] __values;
    }



//accesors

// setValue function
// sets the value of a point
// Precondition:
// Postcondition: the values (point) of an index is set
    void Point::setValue(unsigned int index, double axisPoint) {
        try {
        if (index < 0 || index >= this->__dim){
        throw OutOfBoundsEx(this->__dim, this->__id);}
            __values[index] = axisPoint;
        } catch (OutOfBoundsEx &ex){
            throw ex;
        }
    }


// getValue function
// returns the point value
// Precondition:
// Postcondition: the values of the point are returned
    double Point::getValue(unsigned int index) const {
        try{
            if (index < 0 || index >= this->__dim){
                throw OutOfBoundsEx(this->__dim, this->__id);}
        }
        catch (OutOfBoundsEx &ex){
            throw ex;
        }
        return __values[index];

    }


// getId function
// returns the ID of the point
// Precondition:
// Postcondition: the ID of the point is returned
    int Point::getId() const {
        return __id;
    }

// getDims function
// returns the dimensions of the point
// Precondition:
// Postcondition: the dimensions of the point are returned
   unsigned int Point::getDims() const {
        return __dim;
    }


// Functions

// distanceTo function
    double Point::distanceTo(const Point &array) const {
        double sum = 0;
        double distance = 0;
        try {
            if (__dim != array.__dim) {
                // return 0;
                throw (DimensionalityMismatchEx(this->__dim, this->__id));
            }
            else {
                  for (int index = 0; index < __dim; index++) {
                        distance = (pow(__values[index] - array.__values[index], 2));
                        sum += distance;
                  }
                return sqrt(sum);
                }
            } catch (DimensionalityMismatchEx &ex) {
                 throw ex;
                }
    }



// Overloaded operators
//Members

//overloaded *=
    Point &Point::operator*=(double d) {
        for (int index = 0; index < this->__dim; index++){
            this->__values[index] = this->__values[index] * d;
        }
        return *this;
    }


// Overloaded /=
    Point &Point::operator/=(double d) {
        for (int index = 0; index < this->__dim; index++){
            this->__values[index] = this->__values[index] / d;
        }
        return *this;
    }


// overloaded operator *
    const Point Point::operator*(double d) const {

        for (int index = 0; index < this->__dim; index++){
            this->__values[index] = this->__values[index] * d;
        }
        return *this;
    }

//Overloaded operator /
    const Point Point::operator/(double d) const {
        for (int index = 0; index < this->__dim; index++){
            this->__values[index] = this->__values[index] / d;
        }
        return *this;
    }

// Overloaded operator []
    double &Point::operator[](unsigned int index) {
        try {
            if (index < 0 || index >= this->__dim) {
                throw OutOfBoundsEx(this->__dim, index);
            }
        }
        catch (OutOfBoundsEx &ex) {
            throw ex;
        }
            return __values[index];

    }

// Overloaded operator [] const
// Returned the point at the requested index   // needed by KMeans class
// Precondition:
// Postcondition: the point at the requested index is returned
    const double &Point::operator[](unsigned int index) const {
        return __values[index];
    }


// friend functions

//Overloaded +=
    Point &operator+=(Point &leftSide, const Point &rightSide) {
        try {
               if (leftSide.__dim != rightSide.__dim) {
                    throw DimensionalityMismatchEx(leftSide.__dim, leftSide.__id);
                  }

             int dim = leftSide.__dim;
             for (int index = 0; index < dim; index++) {
                 leftSide.__values[index] += rightSide.__values[index];
                  }
           } catch (DimensionalityMismatchEx &ex){
                  throw ex;
                }
      return leftSide;
    }

// Overloaded -=
    Point &operator-=(Point &leftSide, const Point &rightSide) {
        try{
            if (leftSide.__dim != rightSide.__dim) {
                throw DimensionalityMismatchEx(leftSide.__dim, leftSide.__id);
               }

        int dim = leftSide.__dim;
        for (int index = 0; index < dim; index++) {
            leftSide.__values[index] -= rightSide.__values[index];
              }
          } catch (DimensionalityMismatchEx &ex){
             throw ex;
               }
        return leftSide;
    }

// Overloaded +
    const Point operator+(const Point &leftSide, const Point &rightSide) {
        try {
            if (leftSide.__dim != rightSide.__dim){
                throw DimensionalityMismatchEx(leftSide.__dim, leftSide.__id);
                  }
            } catch (DimensionalityMismatchEx &ex){
                  throw ex;
                }

        Point temp(leftSide.__dim);

        for (int index = 0; index < temp.__dim; index++) {
            temp.__values[index] = leftSide.__values[index] + rightSide.__values[index];
        }

        return temp;
    }

// Overloaded -
    const Point operator-(const Point &leftSide, const Point &rightSide) {
        try {
            if (leftSide.__dim != rightSide.__dim){
                throw DimensionalityMismatchEx(leftSide.__dim, leftSide.__id);
              }
             } catch (DimensionalityMismatchEx &ex){
                       throw ex;
               }

        Point temp(leftSide.__dim);

        for (int index = 0; index < temp.__dim; index++) {
            temp.__values[index] = leftSide.__values[index] - rightSide.__values[index];
        }
        return temp;
    }


// Overloaded ==
    bool operator==(const Point &leftSide, const Point &rightSide) {
        // check that the points are not the same points or their dimensions are different
//        if (&leftSide == &rightSide || leftSide.__dim != rightSide.__dim) {
//            return false;
//        }
        bool equal = true;
        try {
            if ( leftSide.__dim != rightSide.__dim) {
               throw DimensionalityMismatchEx(rightSide.__dim, rightSide.__id);
            }

            for (int index = 0; index < rightSide.__dim; index++) {
                if (leftSide.__values[index] != rightSide.__values[index]) {
                    equal = false;
                    break;
                }
            }
          }catch (DimensionalityMismatchEx &ex){
               throw ex;
              }

        return equal && (leftSide.__id == rightSide.__id);
    }

// Overloaded !=
    bool operator!=(const Point &leftSide, const Point &rightSide) {
        return !(leftSide == rightSide);
    }



// Overloaded <
    bool operator<(const Point &leftSide, const Point &rightSide) {
        //check that the points are not the same points or their dimensions are different
//        if (&leftSide == &rightSide || leftSide.__dim != rightSide.__dim) {
//            return false;
//        }
        bool less_Than = false;
        try{
        if (leftSide.__dim != rightSide.__dim) {
            throw DimensionalityMismatchEx(rightSide.__dim, rightSide.__id);
        }

        for (int index = 0; index < rightSide.__dim; index++) {
            if (leftSide.__values[index] > rightSide.__values[index]) {
                break;
            }
            else if (leftSide.__values[index] < rightSide.__values[index]) {
                less_Than = true;
                break;
            }
        }
    }catch (DimensionalityMismatchEx &ex){
            throw ex;
        }
        return less_Than;
    }

// Overloaded >
    bool operator>(const Point &leftSide, const Point &rightSide) {
        // check that the points are not the same points or their dimensions are different
        return rightSide < leftSide;
    }

// Overloaded <=
    bool operator<=(const Point &leftSide, const Point &rightSide) {
        return !(leftSide > rightSide);
    }

// Overloaded >=
    bool operator>=(const Point &leftSide, const Point &rightSide) {
        return !(leftSide < rightSide);
    }

// Overloaded <<
    std::ostream &operator<<(std::ostream &ostream, const Point &point) {
        ostream <<  point.__values[0];
        for (int  index = 1; index < point.__dim ; index++) {
            ostream << ", " << point.__values[index];
        }
        return ostream;
    }

// Overloaded >>
    std::istream &operator>>(std::istream &istream, Point &point) {
        double* newPoint = new double[point.__dim];
        char pointValue[50];
        unsigned int dimensions = 0;
        if (istream.good()){
            for(int index = 0; index < point.__dim; index++)
            {istream.getline(pointValue, 50, ',');
                newPoint[index] = atof( pointValue);
                dimensions++;
            }
        }
        try{
         if (point.__dim != dimensions){
             point.rewindIdGen();
             throw DimensionalityMismatchEx(point.__dim, point.__id);
         }
        }catch (DimensionalityMismatchEx &ex){
            throw ex;
        }
      point.__values = newPoint;

        return istream;
    }







//        // this works except for dim mismatch
//        std::string number;
//        double value = 0;
//        size_t index = 0;
//        double input = 0;
//
//        while (std::getline(istream, number, POINT_VALUE_DELIM)){
//                 stringstream sso;
//                 sso << number;
//                 sso >> input;
//                 point.__values[index++] = input;
//        }
//        try{
//         if (point.__dim != index){
//             throw DimensionalityMismatchEx(point.__id ,index);
//         }
//        }catch (DimensionalityMismatchEx &ex){
//            throw ex;
//        }
//        return istream;
//    }





//        double* newPoint = new double[point.__dim];
//        char pointValue[50];
//        unsigned int dimensions = 0;
//        if (istream.good()){
//            for(int index = 0; index < point.__dim; index++)
//            {istream.getline(pointValue, 50, ',');
//                newPoint[index] = atof( pointValue);
//                dimensions++;
//            }
//        }
//        try{
//         if (point.__dim != dimensions){
//             throw DimensionalityMismatchEx(point.__dim, point.__id);
//         }
//        }catch (DimensionalityMismatchEx &ex){
//            throw ex;
//        }
//
////        else { std::cerr << "The file not open" << std::endl;}
////        point.__values = newPoint;
//
//        return istream;
//    }




} //end of Clustering namespace bracket



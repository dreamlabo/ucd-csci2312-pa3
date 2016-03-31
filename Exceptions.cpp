#include "Exceptions.h"
#include "Point.h"
#include "Cluster.h"


namespace Clustering {

//***********************************
// Class        OutOfBoundsEx       *
// **********************************
//  OutOfBoundsEx Constructor
   OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r): __current(c), __rhs(r) {
        __name = "OutOfBoundsEx";
    }

// getCurrent Function (OutOfBoundsEx)
// returns __current
// Precondition:
// Postcondition: __current is returned
    unsigned int OutOfBoundsEx::getCurrent() const {
        return __current;
    }

// getRhs Function (OutOfBoundsEx)
// returns __rhs
// Precondition:
// Postcondition: __rhs is returned
    int OutOfBoundsEx::getRhs() const {
        return 0;
    }

// getName Function (OutOfBoundsEx)
// returns __name
// Precondition:
// Postcondition: __name is returned
    std::string OutOfBoundsEx::getName() const {
        return __name;
    }

//overloaded operator << (OutOfBoundsEx)
    std::ostream &operator<<(std::ostream &os, const OutOfBoundsEx &ex) {
        os << ex.__name << ", " << ex.__current << " ID:" << ex.__rhs << std::endl;
        return os;
    }




//**************************************
// Class   DimensionalityMismatchEx    *
// *************************************
//  DimensionalityMismatchEx Constructor
    DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r)
            : __current(c),
              __rhs(r),
              __name("DimensionalityMismatchEx")
    { }

// getCurrent Function (DimensionalityMismatchE)
// returns __current
// Precondition:
// Postcondition: __current is returned
    unsigned int DimensionalityMismatchEx::getCurrent() const {
        return __current;
    }

// getCurrent Function (DimensionalityMismatchE)
// returns __rhs
// Precondition:
// Postcondition: __rhs is returned
    unsigned int DimensionalityMismatchEx::getRhs() const {
        return __rhs;
    }

// getCurrent Function (DimensionalityMismatchE)
// returns __name
// Precondition:
// Postcondition: __name is returned
    std::string DimensionalityMismatchEx::getName() const {
        return __name;
    }

//overloaded operator << (OutOfBoundsEx)
    std::ostream &operator<<(std::ostream &os, const DimensionalityMismatchEx &ex) {
        os << ex.__name << " Dimensions: " << ex.__current << " ID: " << ex.__rhs << std::endl;
        return os;
    }


//**************************************
// Class       ZeroClustersEx          *
// *************************************
//  ZeroClustersEx Constructor
    ZeroClustersEx::ZeroClustersEx() : __name("ZeroClustersEx") {
    }

// getName Function (ZeroClustersEx)
// returns __name
// Precondition:
// Postcondition: __name is returned
    std::string ZeroClustersEx::getName() const {
        return __name;
    }

    //overloaded << operator (ZeroClustersEx)
    std::ostream &operator<<(std::ostream &os, const ZeroClustersEx &ex) {
        os << ex.__name <<  std::endl;
        return os;
    }


//**************************************
// Class       DataFileOpenEx          *
// *************************************
//   DataFileOpenEx   constructor
    Clustering::DataFileOpenEx::DataFileOpenEx(std::string filename)
       : __filename(filename),
         __name("DataFileOpenEx")
    {}

// getFilename Function (ZeroClustersEx)
// returns __fileName
// Precondition:
// Postcondition: __fileName is returned
    std::string DataFileOpenEx::getFilename() const {
        return __filename;
    }

// getName Function (ZeroClustersEx)
// returns __name
// Precondition:
// Postcondition: __name is returned
    std::string DataFileOpenEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const DataFileOpenEx &ex) {
        os << ex.__name << " Filename: " << ex.__filename <<  std::endl;
        return os;
    }


    //**************************************
// Class      ZeroDimensionsEx         *
// *************************************
//  ZeroDimensionsEx  Constructor
    ZeroDimensionsEx::ZeroDimensionsEx() : __name("ZeroDimensionsEx") {
    }

// getName Function (ZeroDimensionsEx)
// returns __name
// Precondition:
// Postcondition: __name is returned
    std::string ZeroDimensionsEx::getName() const {
        return __name;
    }

//overloaded operator<< (ZeroDimensionsEx)
    std::ostream &operator<<(std::ostream &os, const ZeroDimensionsEx &ex) {
        os << ex.__name <<  std::endl;
        return os;
    }



//**************************************
// Class      EmptyClusterEx           *
// *************************************
//  EmptyClusterEx Constructor
    EmptyClusterEx::EmptyClusterEx():__name("EmptyClusterEx") {
    }

// getName Function (EmptyClusterEx)
// returns __name
// Precondition:
// Postcondition: __name is returned
    std::string EmptyClusterEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const EmptyClusterEx &ex) {
        os << ex.__name <<  std::endl;
        return os;
    }
}




//
// Created by Todd Labo on 2/15/2016.
//


#include <limits>
#include <cassert>
#include <string>
#include "Point.h"
#include "Cluster.h"
#include <cstdlib>    //for "atof" in operator >>
#include <fstream>
#include <sstream>
#include <w32api/msclus.h>
#include "Exceptions.h"
using std::stringstream;
using std::string;
namespace Clustering {

    const char POINT_CLUSTER_ID_DELIM = ':';
    unsigned int Cluster::__idGenerator = 0;

    LNode::LNode(const Point &p, LNodePtr n) : point(p), next(n) {
    }


//***********************************
// Class       Centroid             *
// **********************************
// Centroid constructor
    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c) :
            __dimensions(d),
            __p(d),
            __valid(false),
            __c(c){}

// get function (Centroid)
// Returns the centroid
// Precondition:
// Postcondition: the centroid is returned.
    const Point Cluster::Centroid::get() const {
        return __p;
    }

// set (Centriod)
// sets the centroid point
// Precondition:
// Postcondition: the centroid has been set, and __valid is set to true.
    void Cluster::Centroid::set(const Point &p) {
        __p = p;
        __valid = true;
    }

// isValid function (Centriod)
// sets __isValid
// Precondition:
// Postcondition: the __valid is set to true
    bool Cluster::Centroid::isValid() const {
        return __valid;
    }

// setValid (Centriod)
// sets the centroid  __valid
// Precondition:
// Postcondition: __valid is set to true or false.
    void Cluster::Centroid::setValid(bool valid) {
        __valid = valid;
    }

// compute Function
// computes the Centroid of a cluster
// Precondition:
// Postcondition: the centroid has been computed, __valid is set to true
    void Cluster::Centroid::compute() {
        if (__c.__size == 0 ){
                assert(__c.__points == nullptr);
                __valid = true;
                toInfinity();
            }

        else {
          LNodePtr current =__c.__points;
          Point p(__c.getDimensionality());
          unsigned int sizeCheck = 0;

           while (current != nullptr){
             p += current->point/__c.getSize();
             current = current->next;
             sizeCheck++;
             }
          set(p);
         }
    }


// equal Function
//
// Precondition:
// Postcondition:

    bool Cluster::Centroid::equal(const Point &point) const {

        bool isEqual = true;
        for (unsigned int index = 0; index < point.getDims(); index++){
            if (this->__p[index] != point[index]){
                isEqual = false;
                if (isEqual == false){
                    return false;
                }
            }
        }
        return isEqual;
    }


// toInfinity Function
//
// Precondition:
// Postcondition:
    void Cluster::Centroid::toInfinity() {
        {
            for ( size_t index = 0; index < __p.getDims(); index++){
                __p[index] = std::numeric_limits<double>::max();
            }
        }
    }

//***************************************************************************************

//***********************************
// Class        Cluster             *
// **********************************

// Cluster Constructor
    Cluster::Cluster(unsigned int d) :
            __size(0),
            __points(nullptr),
            __dimensionality(d),
            __id (__idGenerator++),
            centroid(d,*this)

    { try
        {
            if (__dimensionality == 0){
                throw ZeroDimensionsEx();
            }

        } catch (ZeroDimensionsEx &ex){
        throw ex;}
    }

////Copy Constructor
    Cluster::Cluster(const Cluster &rightSide): centroid(rightSide.__dimensionality, *this) {

        __size = 0;
        __dimensionality = rightSide.__dimensionality;
        __points = nullptr;
        __id = rightSide.__id;

        if (rightSide.__points == nullptr) {
            assert(rightSide.__size == 0);
            __points = nullptr;
        }
        else {
            LNodePtr current = rightSide.__points;

            while (current != nullptr) {
                add(current->point);
                current = current->next;
            }
        }
    }


// Assignment operator=
    Cluster &Cluster::operator=(const Cluster &rightSide) {
        if (this == &rightSide) {//prevent self assignment
            return *this;
        }

        try{
            if (this->__dimensionality != rightSide.__dimensionality){
                throw DimensionalityMismatchEx(rightSide.__dimensionality, rightSide.__id);
            }
        }catch (DimensionalityMismatchEx &ex){
            throw ex;
        }
        LNodePtr nodePtr = __points; //to traverse the list
        LNodePtr nextNode; //to point to next node

        while (nodePtr != nullptr) { //delete cluster first
            nextNode = nodePtr->next; //save pointer to next node
            delete nodePtr;
            nodePtr = nextNode; //position ptr at the next node
            __size--;
        }

        //Initialize Cluster variables
        __size = 0;
        __points = nullptr;
        __dimensionality = rightSide.__dimensionality;
        __id = rightSide.__id;

        LNodePtr rightSidePtr = rightSide.__points;
        for (int i = 0; i < rightSide.getSize(); i++) {
            add(rightSidePtr->point);
            rightSidePtr = rightSidePtr->next;
        }

        return *this;
    }


// destructor (Cluster)
    Cluster::~Cluster() {
        LNodePtr currentNode = __points;
        LNodePtr nextNode;

        while (currentNode != nullptr) {
            nextNode = currentNode->next;
            delete currentNode;
            currentNode = nextNode;
        }
        __size = 0;
        __points = nullptr;
    }

// getSize function
// returns the size of the cluster
// Precondition:
// Postcondition: the size of the cluster is returned
    unsigned int Cluster::getSize() const {
        return __size;
    }

// getDimensionality function (Cluster)
// Returns the __dimensionality of the cluster
// Precondition:
// Postcondition: The __dimensionality of the cluster is returned
    unsigned int Cluster::getDimensionality() const {
        return __dimensionality;
    }


// getId function (Cluster)
// Returns the __Id of the cluster.
// Precondition:
// Postcondition: the __Id of the Cluster is returned.
    unsigned int Cluster::getId() const {
        return __id;
    }


// add function
// adds a point to linked list in lexographical order
// Precondition:
// Postcondition: a point is added to the linked list, retaining the lexigraphical order
    void Cluster::add(const Point &addPoint) {
        LNodePtr newNode = new LNode(addPoint, nullptr);

        //when adding a new point to a cluster, the centroid of the cluster is no longer valid
        centroid.setValid(false);

        // If there are no nodes in the list
        // Make newNode the first node.
        if (!__points) {  // insert into empty list
            __points = newNode;
        }
        else if (addPoint < __points->point) { //insert at beginning of the list
            newNode->next = __points;
            __points = newNode;
        }
        else {
            LNodePtr currentNode = __points->next, previousNode = __points; //set pointer to current and previous nodes

            while (true) {
                if (currentNode == nullptr || currentNode->point > addPoint) {
                    newNode->next = currentNode;
                    previousNode->next = newNode;
                    break;
                }
                else
                    previousNode = currentNode;
                currentNode = currentNode->next;
            }
        }
        __size++;
    }


// remove function
// removes a point from the cluster
// Precondition:
// Postcondition: a point is removed from the cluster
    const Point &Cluster::remove(const Point &removePoint) {

        LNodePtr nodePtr; //To traverse the list
        LNodePtr previousNode; //To point to the previous node

        //when removing a new point to a cluster, the centroid of the cluster is no longer valid
        centroid.setValid(false);

        //If list is empty
        if (!__points)
            return removePoint;

        //If point to be removed is at head of list
        if (__points->point == removePoint) {
            nodePtr = __points->next;
            delete __points;
            __points = nodePtr;
        }
        else {
            nodePtr = __points;
            while (nodePtr != nullptr && nodePtr->point != removePoint) {
                previousNode = nodePtr;
                nodePtr = nodePtr->next;
            }
            if (nodePtr) {
                previousNode->next = nodePtr->next;
                delete nodePtr;
            }
        }

        __size--;
        return removePoint;
    }

// contains Function
// returns true/false whether a point is found within a cluster
// Precondition:
// Postcondition: a bool value (true/false) is returned
    bool Cluster::contains(const Point &point) const {
        LNodePtr cursor = __points; // to traverse the list
        bool contains = false;

        // Traverse the list, checking to see if the list contains the requested point
        while (cursor != nullptr) {
            if (cursor->point == point) {
                contains = true;
                break;
            }
            cursor = cursor->next; // move curser
        }
        return contains;
    }

// pickCentroids function
    void Cluster::pickCentroids(unsigned int k, Point **pointArray) {
            LNodePtr current = __points;
            Point centroid(__dimensionality);
            for (size_t index = 0; index < __dimensionality; index++){
                centroid[index] = std::numeric_limits<double>::max();
            }

           if (k < __size) {
               for (size_t index = 0; index < k; index++) {
                   *pointArray[index] = current->point;
                   current = current->next;
               }
           }
            else {
                for (size_t index = 0; index < __size; index++) {
                    *pointArray[index] = current->point;
                    current = current->next;
                }
                for (size_t index = __size; index < k; index++){
                    *pointArray[index] = centroid;
                }
            }
    }





// Overloaded operators

// Overloaded [] operator
// returns a reference to the point at the requested index
// Precondition
// Postcondition: a reference to the point at the requested index is returned
    const Point &Cluster::operator[](unsigned int index) const {
        try {
            if (__size == 0) {
                throw EmptyClusterEx();
            }
        }catch (EmptyClusterEx &ex){
            throw ex;
        }
        try {
            if (index >= __size || index < 0 ) {
                throw OutOfBoundsEx(__size, this->__id);
            }
        } catch (OutOfBoundsEx &ex) {
            throw ex;
             }

            // assert (index < __size);
            LNodePtr cursor = __points;

            for (unsigned int i = 0; i < index; i++) {
                //assert(cursor);
                cursor = cursor->next;
            }
            return cursor->point;
        }


// Members: Compound assignment (Point argument)

// operator +=  (Point argument)
// Adds a point to an existing cluster.
// Precondition:
// Postcondition: A new point is added to an existing Cluster.
    Cluster &Cluster::operator+=(const Point &point) {
        try{
            if (point.getDims()!= this->__dimensionality){
                throw DimensionalityMismatchEx(point.getDims(), this->__id);
            }
        }catch (DimensionalityMismatchEx &ex){
            throw ex;
        }

        add(point);
        return *this;
    }

// operator -=  (Point argument)
// Removes a point from existing cluster.
// Precondition:
// Postcondition: A new point is removed from an existing Cluster.
    Cluster &Cluster::operator-=(const Point &point) {
        try{
            if (point.getDims()!= this->__dimensionality){
                throw DimensionalityMismatchEx(point.getDims(), this->__id);
            }
        }catch (DimensionalityMismatchEx &ex){
            throw ex;
        }
        remove(point);
        return *this;
    }


// Members: Compound assignment (Cluster argument)
// operator +=   (Cluster)
// creates a union of two cluster
    Cluster &Cluster::operator+=(const Cluster &cluster) {
        LNodePtr cursor = cluster.__points; // to traverse the list
        while (cursor != nullptr) {
            if (!contains(cursor->point)) {
                add(cursor->point);
            }
            cursor = cursor->next;
        }
        return *this;
    }

// operator -=   (Cluster)
// creates a new cluster created from the (asymmetric) difference of two cluster
// Precondition:
// Postcondition: a new cluster created and returned from the (asymmetric) difference of two cluster.
    Cluster &Cluster::operator-=(const Cluster &cluster) {
        // Cluster tempCluster;
        LNodePtr cursor = cluster.__points;

        while (cursor != nullptr) {
            if (contains(cursor->point)) {
                remove(cursor->point);
            }
            cursor = cursor->next;
        }
        return *this;
    }


//friends IO


// Overloaded << operator
    std::ostream &operator<<(std::ostream &ostream, const Cluster &cluster) {
         assert(cluster.__points != nullptr);

         LNodePtr cursor = cluster.__points;

                while (cursor != nullptr) {
                    ostream << cursor->point << ' ' << POINT_CLUSTER_ID_DELIM << ' ' << cluster.__id << std::endl;
                    cursor = cursor->next;
                }
                return ostream;
    }


// Overloaded >> operator
// Reads cluster from a file
// Precondition:
// Postcondition: A point is read from a file, and a new point is added to a cluster.
    std::istream &operator>>(std::istream &istream, Cluster &cluster) {
        Point readIn(cluster.__dimensionality);
        try {
            std::string input;
            while (getline(istream, input)) {
                std::stringstream sso(input);

                sso >> readIn;
                cluster.add(readIn);
            }

            }catch (DimensionalityMismatchEx & ex)
            {
                throw ex;
                //readIn.rewindIdGen();
            }
        return istream;
        }




// Friends: Comparison


// Overloaded == operator
    bool operator==(const Cluster &lhs, const Cluster &rhs) {
        //Check to see if clusters are of same size, if not return false
        //assert (lhs.__size == rhs.__size);
        // check that the points are not the same points or their dimensions are different
        if (lhs.__size != rhs.__size) {
            return false;
        }

        LNodePtr targetRight = rhs.__points; //set rhs curser to head
        LNodePtr targetLeft = lhs.__points;  //set lhs curser head

        //  while the end of the list hasn't been reached
        while ( targetLeft != nullptr  ) {
            // check to see if nodes don't match, if they don't, return false
            if (targetLeft->point != targetRight->point) {
                return false;
            }
            else  // otherwise move to the  next node
                targetRight = targetRight->next;
            targetLeft = targetLeft->next;
        } //return to beg of while loop

        //if we reach here, all points are the same and we return true
        return true;
    }


// Overloaded != operator
    bool operator!=(const Cluster &lhs, const Cluster &rhs) {
        //Check to see if clusters are of same size, if not return false
        //assert (lhs.__size == rhs.__size);
        // check that the points are not the same points or their dimensions are different
//        if (lhs.__size != rhs.__size) {
//            return false;
//        }
        try {
             if (lhs.__dimensionality != rhs.__dimensionality){
                 throw (DimensionalityMismatchEx(rhs.__dimensionality,rhs.__id));
             }
            }catch (DimensionalityMismatchEx &ex){
                 throw ex;
                }
        LNodePtr targetRight = rhs.__points; //set rhs curser to head
        LNodePtr targetLeft = lhs.__points;  //set lhs curser head

        //  while the end of the list hasn't been reached
        while ( targetLeft != nullptr && targetRight!= nullptr  ) {
            // check to see if nodes don't match, if they don't, return false
            if (targetLeft->point == targetRight->point) {
                return false;
            }
            else  // otherwise move to the  next node
                targetRight = targetRight->next;
            targetLeft = targetLeft->next;
        } //return to beg of while loop

        //if we reach here, all points are the same and we return true
        return true;
    }


// Friends: Arithmetic (Cluster and Point)

// operator +   (Cluster,Point)
// A new cluster is created by adding a point to an existing cluster. (using copy constuctor and add function of cluster.cpp)
// Precondition:
// Postcondition: a new cluster has been created by adding a point from to an existing cluster.
    const Cluster operator+(const Cluster &cluster, const Point &pointToAdd) {
        Cluster tempCluster(cluster);
        tempCluster += pointToAdd;

        return tempCluster;
    }

// operator -   (Cluster,Point)
// A new cluster is created by removing a point from an existing cluster.  (using copy constuctor and remove function of cluster.cpp)
// Precondition:
// Postcondition: a new cluster has been created by removing a point from an existing cluster.
    const Cluster operator-(const Cluster &cluster, const Point &pointToSubtract) {
        Cluster tempCluster(cluster);
        tempCluster -= pointToSubtract;

        return tempCluster;
    }


    // Friends: Arithmetic (two Clusters)

// operator + (cluster,cluster)
// union of two clusters.
// Precondition:
// Postcondition: The union of two clusters is returned
    const Cluster operator+(const Cluster &leftCluster, const Cluster &rightCluster) {
        Cluster plusCluster(leftCluster);


        // LNodePtr  cursorLeft = leftCluster.__points;
        LNodePtr cursorRight = rightCluster.__points;
        while (cursorRight != nullptr){
            if(!plusCluster.contains(cursorRight->point)){
                plusCluster.add(cursorRight->point);
            }
            cursorRight = cursorRight->next;
        }
        return plusCluster;
    }

// operator - (cluster,cluster)
// (asymmetric) difference of two clusters.
// Precondition:
// Postcondition: The (asymmetric) difference of two clusters is returned
    const Cluster operator-(const Cluster &leftCluster, const Cluster &rightCluster) {
        // Cluster newCluster;
        Cluster tempCluster(leftCluster);

        LNodePtr cursorRight = rightCluster.__points;
        while (cursorRight != nullptr){
            if(tempCluster.contains(cursorRight->point)){
                tempCluster.remove(cursorRight->point);
                cursorRight = cursorRight->next;
            } else{
                cursorRight = cursorRight->next;
            }
        }
        return tempCluster;
    }



    //***********************************
// Class           Move             *
// **********************************
// move constructor
    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) : __p(p), __from(from), __to(to){
    }

// perform Function (Move Class)
// removes a point from 1 cluster and adds the point to a different cluster
//        and invalidates the centriods of both clusters
// Precondition:
// Postcondition: a point is moved from one cluster to another. The centriods of both clusters is set to false
    void Cluster::Move::perform() {
        __to.add(__from.remove(__p));
        __to.centroid.setValid(false);
        __from.centroid.setValid(false);
    }






}// end of namespace Clustering (don't remove } )



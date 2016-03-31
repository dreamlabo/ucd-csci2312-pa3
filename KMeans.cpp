//
// Created by Owner on 3/21/2016.
//

#include "KMeans.h"
#include "Exceptions.h"
 namespace Clustering {


// constructor
     KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter)
             : __dimensionality(dim),
               __k(k),
               __iFileName(filename),
               __maxIter(maxIter) {
         __numIter = 0;
         __numNonempty = 0;
         __numMovesLastIter = 0;

//           Cluster **__clusters;
//           Point **__initCentroids;
         try {
             if (k == 0) {
                 throw ZeroClustersEx();
             }
         } catch (ZeroClustersEx &ex ){
             throw ex;
         }
     }


// destructor
     KMeans::~KMeans() {
// needs code
     }


// getMaxIter function
// returns __maxIter
// Precondition:
// Postcondition: __maxIter is returned
     unsigned int KMeans::getMaxIter() {
         return __maxIter;
     }


// getNumIter function
// returns __numIter
// Precondition:
// Postcondition: __numIter is returned
     unsigned int KMeans::getNumIters() {
         return __numIter;
     }

// getNumNonemptyClusters function
// returns __numNonempty
// Precondition:
// Postcondition: __numNonempty is returned
     unsigned int KMeans::getNumNonemptyClusters() {
         return __numNonempty;
     }


// getNumMovesLastIter function
// returns __numMovesLastIter
// Precondition:
// Postcondition: __numMovesLastIter is returned
     unsigned int KMeans::getNumMovesLastIter() {
         return __numMovesLastIter;
     }


     Cluster &KMeans::operator[](unsigned int u) {
         //need code
         // return <#initializer#>;
     }


     const Cluster &KMeans::operator[](unsigned int u) const {
         //need code
         // return <#initializer#>;
     }



    void KMeans::run() {
        //need code

    }

    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans) {
        return os;
    }

}// end Clustering
















#include "lloyd.h"
#include "mathUtils.h"

// Returns index of closest centroid point, after checking all points
int lloyd_method(std::vector<PointPtr> centroidPoints, PointPtr point, int dimension) {
    double min_dist = INT32_MAX;
    double cur_dist = 0;
    int index = 0;

    for(int i=0;i<centroidPoints.size();i++) {
        cur_dist = euclideanDistance(point, centroidPoints[i], dimension);
        if(cur_dist < min_dist){
            min_dist = cur_dist;
            index = i;
        }
    }
    return index;
}
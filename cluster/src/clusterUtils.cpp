#include "clusterUtils.h"

double silhouette_calculator(PointPtr point, std::vector<Cluster> clusters, int dimension)
{

    //find 2 closest clusters
    std::vector<int> closestClusters = get_2_closest_clusters(point, clusters, dimension);

    //find ai
    ClusterPtr cluster = &clusters[closestClusters[0]];
    double distanceSum = 0;
    for (int i = 0; i < cluster->size; i++)
    {
        distanceSum += euclideanDistance(point, cluster->points[i], dimension);
    }
    double a = distanceSum / (cluster->size - 1); // -1 beacause point belongs in cluster

    //find bi
    cluster = &clusters[closestClusters[1]];
    distanceSum = 0;
    for (int i = 0; i < cluster->size; i++)
    {
        distanceSum += euclideanDistance(point, cluster->points[i], dimension);
    }
    double b = distanceSum / cluster->size; // -1 beacause point belongs in cluster

    return (b - a) / (std::max(a, b));
}

std::vector<int> get_2_closest_clusters(PointPtr point, std::vector<Cluster> clusters, int dimension)
{

    std::vector<int> closestClusters;
    closestClusters.resize(2);
    for (int i = 0; i < 2; i++)
        closestClusters[i] = 0;

    std::vector<double> closestDist;
    closestDist.resize(2);
    for (int i = 0; i < 2; i++)
        closestDist[i] = INT32_MAX;

    double min_dist = INT32_MAX;
    double cur_dist = 0.0;

    // get all centroid points ( we could add this as a parameter)
    std::vector<PointPtr> centroidPoints;
    for (int i = 0; i < clusters.size(); i++)
    {
        centroidPoints.push_back(clusters[i].centroidPoint);
    }

    for (int i = 0; i < centroidPoints.size(); i++)
    {

        cur_dist = euclideanDistance(point, centroidPoints[i], dimension);
        if (cur_dist < closestDist[1])
        {
            closestDist[1] = cur_dist;
            closestClusters[1] = i;
        }
        // sort 1&2
        if (closestDist[1] < closestDist[0])
        {
            closestDist[1] = closestDist[0];
            closestDist[0] = cur_dist; // cur_dist holds temp
            closestClusters[1] = closestClusters[0];
            closestClusters[0] = i; // i holds cur_cluster
        }
    }

    return closestClusters;
}
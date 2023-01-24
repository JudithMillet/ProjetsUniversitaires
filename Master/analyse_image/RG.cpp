#include <stdio.h>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <vector>
#include <algorithm>
#include <list>
#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <iostream>
#include <map>
#include "tools/command_line.h"

using namespace std::chrono;
using namespace cv;

Mat image;
Mat dst;

std::map<int, std::list<int>> graph_adjacency;
std::vector<int> rag;
std::mutex m;

//////////////////////////////////////////////////////////////////////////////////////////


//desc: get the four (or less) neighbors of a point 
//args: Point p
//return: Point vector
std::vector<Point> getNeighbors(Point p)
{
    std::vector<Point> neighbors;
    int x = p.x;
    int y = p.y;

    if (y > 0)
        neighbors.push_back(Point(x, y - 1));

    if (y < image.rows - 1)
        neighbors.push_back(Point(x, y + 1));

    if (x > 0)
        neighbors.push_back(Point(x - 1, y));

    if (x < image.cols - 1)
        neighbors.push_back(Point(x + 1, y));

    return neighbors;
}

//desc: check if two points are simmilars
//args: Point parentPoint, Point visitedPoint, unsigned int threshold
//return: boolean
bool isSimilar(Point parentPoint, Point visitedPoint, unsigned int threshold)
{
    Vec3b parent = image.at<Vec3b>(parentPoint);
    Vec3b visited = image.at<Vec3b>(visitedPoint);

    float dist = sqrt(pow(parent[0] - visited[0], 2) + pow(parent[1] - visited[1], 2) + pow(parent[2] - visited[2], 2));
    return dist < threshold;
}

Vec3b avgRegions(int id) 
{
    Vec3b colorAvg;

    int x, y, compte = 0;
    Vec3b p;
    float b, g, r;
    b = g = r = 0;

    for (int i=0 ; i<rag.size() ; i++)
    {
        if(rag[i] == id)
        {
            y = i / image.cols;
            x = i - y * image.cols;
            p = image.at<Vec3b>(Point(x, y));
            b += p[0];
            g += p[1];
            r += p[2];
            compte++;
        }
    }
    colorAvg[0] = b/compte;
    colorAvg[1] = g/compte;
    colorAvg[2] = r/compte;

    return colorAvg;
}

bool isSimilarAvg(int parentID, int visitedID, unsigned int threshold)
{
    
    Vec3b parent = avgRegions(parentID);
    Vec3b visited = avgRegions(visitedID);

    float dist = sqrt(pow(parent[0] - visited[0], 2) + pow(parent[1] - visited[1], 2) + pow(parent[2] - visited[2], 2));
    return dist < threshold+5;
}


//desc: generates a new random color
//args: Vec3b newColor
void randomColor(Vec3b &newColor)
{
    newColor[0] = rand() % 255;
    newColor[1] = rand() % 255;
    newColor[2] = rand() % 255;
}


//desc: returns nb random pixel in four corners of the image 
//args: Int nb
//return: Point vector
std::vector<Point> randomSeeds(int nb = 10)
{
    std::vector<Point> v;
    Vec3b newColor;

    // Seeds organized depending on location
    for (int j = 0; j < nb / 4; j++) {
      Point p = Point(rand() % (image.cols / 2), rand() % (image.rows / 2));
      v.push_back(p);
    }

    for (int j = 0; j < nb / 4; j++) {
      Point p = Point(image.cols / 2 + rand() % (image.cols / 2),
                      image.rows / 2 + rand() % (image.rows / 2));
      v.push_back(p);
    }

    for (int j = 0; j < nb / 4; j++) {
      Point p = Point(image.cols / 2 + rand() % (image.cols / 2),
                      rand() % (image.rows / 2));
      v.push_back(p);
    }

    for (int j = 0; j < nb / 4; j++) {
      Point p = Point(rand() % (image.cols / 2),
                      image.rows / 2 + rand() % (image.rows / 2));
      v.push_back(p);
    }

    // Seeds randomly organized
    // for (int i = 0; i < nb; i++)
    // {
    //     int x = rand() % image.cols;
    //     int y = rand() % image.rows;
    //     int pos = image.cols * y + x;
    //     int compte = 0;
    //     while (rag[pos] != -1 || compte < 150)
    //     {
    //         x = rand() % image.cols;
    //         y = rand() % image.rows;
    //         pos = image.cols * y + x;
    //         compte++;
    //     }
    //     Point p = Point(x, y);
    //     v.push_back(p);
    //     if (std::find(rag.begin(), rag.end(), -1) == rag.end())
    //     {
    //         std::cout << " STOP " << std::endl;
    //         break;
    //     }
    // }
    return v;
}

//////////////////////////////////////////////////////////////////////////////////////////


//desc: Main algorithme growing an entire region for one seed
//args: Point seed, int id of its region, unsigned int threshold
void regionGrowing(Point seed, int id, unsigned int threshold)
{
    Point point;

    std::vector<bool> visited(image.total(), false);
    std::vector<bool> neigVisited(image.total(), false);

    std::list<Point> pointList = {};
    pointList.push_front(seed);

    std::list<int> adjacencyList = {};
    m.lock();
    int seedPos = image.cols * seed.y + seed.x;
    if(rag[seedPos] == -1) {
        rag[seedPos] = id;
        m.unlock();

        while (!pointList.empty())
        {
            point = pointList.front();
            pointList.pop_front();

            int pos = image.cols * point.y + point.x;

            visited.at(pos) = true;
            neigVisited.at(pos) = true;

            std::vector<Point> neighbors = getNeighbors(point);

            for (Point neig : neighbors)
            {
                int posN = image.cols * neig.y + neig.x;
                m.lock();
                if (!visited.at(posN) && isSimilar(point, neig, threshold) && !neigVisited.at(posN) && rag[posN] == -1)
                {
                    pointList.push_front(neig);
                    neigVisited.at(posN) = true;
                    rag[posN] = id;
                }

                // construction adjacency list
                else if (rag[posN] != -1)
                {
                    int id_adjacency = rag[posN];

                    // doesn't exists in the list
                    if (std::find(adjacencyList.begin(), adjacencyList.end(), id_adjacency) == adjacencyList.end())
                    {
                        adjacencyList.push_back(id_adjacency);
                    }
                }
                m.unlock();
            }
        }
        graph_adjacency[id] = adjacencyList;
    }
    else 
    {
        m.unlock();
    }
}

//desc: calls the region growing algorithme for each threads
//args: Point vector of seeds, int idb begining of the regions ids, int ide end of the regions ids, unsigned int threshold
void regionGrowingThreads(std::vector<Point> seeds, int idb, int ide, unsigned int threshold)
{
    for (int i = idb; i < ide; i++)
        regionGrowing(seeds[i - idb], i, threshold);
}

//////////////////////////////////////////////////////////////////////////////////////////


//desc: calculates how many pixels are already assigned to a region
//return: number of assigned pixels
int coverageCount() 
{
    int coverage = 0;
    for (int i = 0; i < image.total(); i++) 
    {
        if (rag[i] != -1) 
        {
            coverage++;
        }
    }
    return coverage;
}


//desc: find a pixel assigned to a region
//agrs: int id of a region
//return: Coordinates (Point) of the pixel in the image
Point findPoint(int id)
{
    std::vector<int>::iterator it = std::find(rag.begin(), rag.end(), id);
    if (it != rag.end())
    {
        int index = std::distance(rag.begin(), it);

        int y = index / image.cols;
        int x = index - y * image.cols;

        return Point(x, y);
    }
    else
    {
        return Point();
    }
}

//desc: check if two regions are simmilars
//agrs: int r1 id of a region, int r2 id of another region, unsigned int threshold
//return: boolean
bool similarRegions(int r1, int r2, unsigned int threshold)
{
    Point pR1 = findPoint(r1);
    Point pR2 = findPoint(r2);
    return isSimilarAvg(r1, r2, threshold);
}

//desc: merge two regions into one if they are simmilars for each neighbors regions
//agrs: int nbSeeds number of seeds, unsigned int threshold
void mergeRegions(unsigned int nbSeeds, unsigned int threshold)
{
    std::vector<bool> visited(nbSeeds, false); // keep visited regions

    for (int id = 0; id < nbSeeds; id++)
    {
        for (int idAdj : graph_adjacency[id])
        {
            if (id != idAdj && !visited.at(idAdj) && similarRegions(id, idAdj, threshold))
            {
                // std::cout<<"ID "<<id<<" ID adj "<<idAdj<<std::endl;
                visited.at(idAdj) = true;

                std::replace(rag.begin(), rag.end(), idAdj, id);

                // merge current region and adjacent region lists into current region list
                for (int idAdjAdj : graph_adjacency[idAdj])
                {
                    if (std::find(graph_adjacency[id].begin(), graph_adjacency[id].end(), idAdjAdj) == graph_adjacency[id].end())
                    {
                        graph_adjacency[id].push_back(idAdjAdj);
                    }
                }

                // remove adjacent region list
                graph_adjacency[idAdj] = {};
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

//desc: assign a color to every pixel that is in a region
//agrs: int nb number of regions, bool outline
void colorRegions(unsigned int nb, bool outline = false)
{
    Vec3b newColor;
    Vec3b red;
    Vec3b black;

    black[0] = 0;
    black[1] = 0;
    black[2] = 0;

    red[0] = 0;
    red[1] = 0;
    red[2] = 255;

    Point p;
    std::vector<Vec3b> colors;
    int voisinX, voisinY;

    for (int i = 0; i < nb; i++)
    {
        randomColor(newColor);
        colors.push_back(newColor);
    }

    for (int x = 0; x < image.cols; x++)
    {
        for (int y = 0; y < image.rows; y++)
        {
            p = Point(x, y);
            int pos = image.cols * y + x;

            if (outline)
            {
                if (y < image.rows - 1)
                    voisinY = image.cols * (y + 1) + x;
                else
                    voisinY = -1;

                if (x < image.cols - 1)
                    voisinX = image.cols * y + (x + 1);
                else
                    voisinX = -1;
            }
            if (rag[pos] != -1)
            {
                if (!outline || voisinY == -1 || voisinX == -1 || (rag[pos] == rag[voisinY] && rag[pos] == rag[voisinX]))
                {
                    dst.at<Vec3b>(p) = colors[rag[pos]];
                }
                else
                {
                    dst.at<Vec3b>(p) = red;
                }
            } 
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

//desc: function called to do the segmentation without threads
//agrs: Point vector of seeds, int nbSeeds number of seeds, unsigned int threshold, bool outline
void segmentationWithoutThreads(std::vector<Point> seeds, unsigned int nbSeeds, unsigned int threshold, bool outline = false)
{
    for (int i = 0; i < seeds.size(); i++)
    {
        regionGrowing(seeds[i], i, threshold);
    }
}

void segmentationWithThreads(std::vector<Point> seeds, unsigned int nbSeeds, unsigned int threshold, bool outline = false)
{
    std::vector<std::thread> vec_th;
    int nb_seeds_thread = nbSeeds / 4;

    for (int i = 0; i < 4; i++)
    {
        std::vector<Point>::const_iterator first = seeds.begin() + (i * nb_seeds_thread);
        std::vector<Point>::const_iterator last = seeds.begin() + (i * nb_seeds_thread + nb_seeds_thread);
        std::vector<Point> newSeeds(first, last);
        vec_th.push_back(std::thread(regionGrowingThreads, newSeeds, i * nb_seeds_thread, i * nb_seeds_thread + nb_seeds_thread, threshold));
    }
    for (int i = 0; i < 4; i++)
        vec_th.at(i).join();
}

//////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    srand(time(NULL));
    if (argc < 2)
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    image = imread(argv[1], 1);
    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }

    unsigned int nbSeeds;
    unsigned int tolerance;
    unsigned int threshold;
    bool smoothing;
    bool threads;
    bool outline;

    process_command_line(argc, argv, nbSeeds, tolerance, threshold, smoothing, threads, outline);

    // init destination image
    dst = Mat(image.rows, image.cols, CV_8UC3);

    // preprocess in option
    if (smoothing)
    {
        GaussianBlur(image, image, Size(3, 3), 0);
        medianBlur(image, image, 3);
    }

    // init regions adjacency graph
    for (int i = 0; i < image.total(); i++)
    {
        rag.push_back(-1);
    }

    std::vector<Point> seeds;
    int nbSeedsTotal = 0;
    auto start = high_resolution_clock::now();
    //If not using threads
    if (!threads)
    {
        int coverage = 0;

        while (coverage < tolerance)
        {
            if (std::find(rag.begin(), rag.end(), -1) !=
                rag.end())
            {
                seeds = randomSeeds(nbSeeds);
                nbSeeds = seeds.size();
                nbSeedsTotal += nbSeeds;
                segmentationWithoutThreads(seeds, nbSeeds, threshold, outline);
                coverage = 100 * coverageCount() / image.total();
            }
        }
    }

    //If using threads
    else
    {
        int coverage = 0;

        while (coverage < tolerance)
        {
            if (std::find(rag.begin(), rag.end(), -1) !=
                rag.end())
            {
                seeds = randomSeeds(nbSeeds);
                nbSeeds = seeds.size();
                nbSeedsTotal += nbSeeds;
                segmentationWithThreads(seeds, nbSeeds, threshold, outline);
                coverage = 100 * coverageCount() / image.total();
            }
        }
        mergeRegions(nbSeedsTotal, threshold);
    }

    // Coloring all regions
    colorRegions(nbSeedsTotal, outline);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << "Time taken by function: "
         << duration.count() << " milliseconds" << std::endl;

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", dst);
    // imwrite(argv[2], dst);
    waitKey(0);
    destroyAllWindows();

    return 0;
}
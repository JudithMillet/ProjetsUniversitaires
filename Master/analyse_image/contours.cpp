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
#include "tools/command_line_contours.h"

#define PI 3.14159265
#define INFINITE_DIR 500

using namespace std::chrono;
using namespace cv;

Mat image;
Mat dstColor, dstBW, dstGS;
int prewitt[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
int sobel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int kirsch[3][3] = {{-3, -3, 5}, {-3, 0, 5}, {-3, -3, 5}};

int prewittNE[3][3] = {{0, 1, 1}, {-1, 0, 1}, {-1, -1, 0}};
int prewittSE[3][3] = {{-1, -1, 0}, {-1, 0, 1}, {0, 1, 1}};

int sobelNE[3][3] = {{0, 1, 2}, {-1, 0, 1}, {-2, -1, 0}};
int sobelSE[3][3] = {{-2, -1, 0}, {-1, 0, 1}, {0, 1, 2}};

int kirschNE[3][3] = {{-3, 5, 5}, {-3, 0, 5}, {-3, -3, -3}};
int kirschSE[3][3] = {{-3, -3, -3}, {-3, 0, 5}, {-3, 5, 5}};

//////////////////////////////////////////////////////////////////////////////////

void normalizeIntensity(std::vector<std::pair<float, double>> &buffer, float min, float max)
{
    for (int i = 0; i < image.rows * image.cols; i++)
    {
        buffer.at(i).first = (buffer.at(i).first - min) * 255 / (max - min);
    }
}

//////////////////////////////////////////////////////////////////////////////////

void convolution_bidir(unsigned int mask, std::vector<std::pair<float, double>> &buffer, unsigned int threshold, const bool &global)
{
    float Gx;
    float Gy;
    float val;
    float max = 0;
    float min = MAXFLOAT;
    for (int x = 0; x < image.cols; x++)
    {
        for (int y = 0; y < image.rows; y++)
        {

            Gx = 0;
            Gy = 0;
            if (x > 0 && x < image.cols - 1 && y > 0 && y < image.rows - 1)
            {

                for (int i = -1; i <= 1; i++)
                {
                    switch (mask)
                    {
                    case 0:
                        for (int j = -1; j <= 1; j++)
                        {
                            Gx += image.at<uchar>(y + i, x + j) * prewitt[i + 1][j + 1];
                            Gy += image.at<uchar>(y + i, x + j) * prewitt[j + 1][i + 1];
                        }
                        break;
                    case 1:
                        for (int j = -1; j <= 1; j++)
                        {
                            Gx += image.at<uchar>(y + i, x + j) * sobel[i + 1][j + 1];
                            Gy += image.at<uchar>(y + i, x + j) * sobel[j + 1][i + 1];
                        }
                        break;
                    case 2:
                        for (int j = -1; j <= 1; j++)
                        {
                            Gx += image.at<uchar>(y + i, x + j) * kirsch[i + 1][j + 1];
                            Gy += image.at<uchar>(y + i, x + j) * kirsch[j + 1][i + 1];
                        }
                        break;
                    }
                }
                val = int(sqrt(Gx * Gx + Gy * Gy));

                if (val < min)
                    min = val;
                if (val > max)
                    max = val;

                double dir = -1;
                if (Gx != 0)
                    dir = atan(Gy / Gx) * 180 / PI;

                if (dir < 0)
                    dir += 180;

                unsigned char v = val;

                buffer.push_back(std::make_pair(val, dir));
            }
            else
            {
                buffer.push_back(std::make_pair(0, -1));
            }
        }
    }
    normalizeIntensity(buffer, min, max);
}

void convolution_multi(unsigned int mask, std::vector<std::pair<float, double>> &buffer, unsigned int threshold, const bool &global)
{
    float Gx, Gy, Gz, Gw, Gmax, Gmax1, Gmax2;

    float max = 0;
    float min = MAXFLOAT;
    for (int x = 0; x < image.cols; x++)
    {
        for (int y = 0; y < image.rows; y++)
        {
            if (x > 0 && x < image.cols - 1 && y > 0 && y < image.rows - 1)
            {
                Gx = 0;
                Gy = 0;
                Gz = 0;
                Gw = 0;
                for (int i = -1; i <= 1; i++)
                {
                    switch (mask)
                    {
                    case 0:
                        for (int j = -1; j <= 1; j++)
                        {
                            Gx += image.at<uchar>(y + i, x + j) * prewitt[i + 1][j + 1];
                            Gy += image.at<uchar>(y + i, x + j) * prewitt[j + 1][i + 1];
                            Gz += image.at<uchar>(y + i, x + j) * prewittNE[i + 1][j + 1];
                            Gw += image.at<uchar>(y + i, x + j) * prewittSE[i + 1][j + 1];
                        }
                        break;
                    case 1:
                        for (int j = -1; j <= 1; j++)
                        {
                            Gx += image.at<uchar>(y + i, x + j) * sobel[i + 1][j + 1];
                            Gy += image.at<uchar>(y + i, x + j) * sobel[j + 1][i + 1];
                            Gz += image.at<uchar>(y + i, x + j) * sobelNE[i + 1][j + 1];
                            Gw += image.at<uchar>(y + i, x + j) * sobelSE[i + 1][j + 1];
                        }
                        break;
                    case 2:
                        for (int j = -1; j <= 1; j++)
                        {
                            Gx += image.at<uchar>(y + i, x + j) * kirsch[i + 1][j + 1];
                            Gy += image.at<uchar>(y + i, x + j) * kirsch[j + 1][i + 1];
                            Gz += image.at<uchar>(y + i, x + j) * kirschNE[i + 1][j + 1];
                            Gw += image.at<uchar>(y + i, x + j) * kirschSE[i + 1][j + 1];
                        }
                        break;
                    }
                }

                double dir = -1;

                Gmax1 = fmax(abs(Gx), abs(Gy));
                Gmax2 = fmax(abs(Gz), abs(Gw));
                Gmax = fmax(Gmax1, Gmax2);

                if (Gmax == abs(Gx))
                {
                    // East (horizontal)
                    if (Gx != 0)
                    {
                        dir = 0;
                    }
                    else
                        dir = INFINITE_DIR;
                }
                else if (Gmax == abs(Gy))
                {
                    // South (vertical)
                    if (Gy != 0)
                    {
                        dir = -PI / 2;
                    }
                    else
                        dir = INFINITE_DIR;
                }
                else if (Gmax == abs(Gz))
                {
                    // North east (diag)
                    if (Gz != 0)
                    {
                        dir = PI / 4;
                    }
                    else
                        dir = INFINITE_DIR;
                }
                else
                { // Gmax == abs(Gw)
                    // South East (diag)
                    if (Gw != 0)
                    {
                        dir = -PI / 4;
                    }
                    else
                        dir = INFINITE_DIR;
                }

                if (Gmax < min)
                    min = Gmax;
                if (Gmax > max)
                    max = Gmax;

                buffer.push_back(std::make_pair(Gmax, dir));
            }
            else
                buffer.push_back(std::make_pair(0, -1));
        }
    }
    normalizeIntensity(buffer, min, max);
}

//////////////////////////////////////////////////////////////////////////////////

int meanThreshold(std::vector<std::pair<float, double>> &buffer)
{
    unsigned long sum = 0;

    for (int i = 0; i < image.cols * image.rows; i++)
    {
        sum += buffer.at(i).first;
    }
    return sum / (image.cols * image.rows);
}

void fixedThreshold(std::vector<std::pair<float, double>> &buffer, unsigned int threshold)
{
    if (threshold == 0)
    {
        threshold = meanThreshold(buffer);
    }
    for (int i = 0; i < image.cols * image.rows; i++)
    {
        if (buffer.at(i).first < threshold)
        {
            buffer.at(i).second = -1;
        }
    }
}

void globalThreshold(std::vector<std::pair<float, double>> &buffer)
{
    fixedThreshold(buffer, meanThreshold(buffer));
}

void seuillagehysteresis(std::vector<std::pair<float, double>> &buffer, unsigned int threshold)
{
    int intens;
    int index;
    int intensneigh;
    float threshold_bas = 0;
    unsigned char ints;
    bool neighInThreshold = false;

    // If no fixed threshold in options : high threshold = mean and low threshold = mean of values < threshold
    if (threshold == 0)
    {
        int count = 0;
        float sumlow = 0;
        float sumup = 0;

        float mean = meanThreshold(buffer);
        for (int i = 0; i < image.cols * image.rows; i++)
        {
            if (buffer.at(i).first < mean)
            {
                count++;
                sumlow += buffer.at(i).first;
            }
            else
            {
                sumup += buffer.at(i).first;
            }
        }
        threshold = (sumup / ((image.cols * image.rows) - count));
        threshold_bas = mean;
    }
    // If fixed threshold in options : high threshold = fixed threshold and low threshold = 30% high threshold
    else
        threshold_bas = threshold * 0.3;

    std::cout << threshold_bas << " " << threshold << std::endl;

    for (int x = 0; x < image.cols; x++)
    {
        for (int y = 0; y < image.rows; y++)
        {
            index = x * image.rows + y;
            intens = buffer.at(index).first;
            if (intens < threshold)
            {
                if (intens > threshold_bas)
                {
                    for (int i = -1; i <= 1; i++)
                    {
                        for (int j = -1; j <= 1; j++)
                        {
                            if (!(i == 0 && j == 0) && (x + i) >= 0 && (y + j) >= 0 
                                && (x + i) < image.cols && (y + j) < image.rows)
                            {
                                intensneigh = buffer.at((x + i) * image.rows + y + j).first;
                                neighInThreshold = neighInThreshold || (intensneigh >= threshold);
                            }
                        }
                    }

                    if (!neighInThreshold)
                        buffer.at(index).second = -1;
                }
                else
                    buffer.at(index).second = -1;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////

void colorImage(const std::vector<std::pair<float, double>> &buffer, const bool &multi)
{
    unsigned char v;
    if (multi)
    {
        for (int x = 0; x < image.cols; x++)
        {
            for (int y = 0; y < image.rows; y++)
            {
                unsigned int index = y + x * image.rows;

                std::pair<float, double> result = buffer.at(index);
                float val = result.first;
                double dir = result.second;

                if (dir == -1 || val == 0)
                {
                    dstColor.at<Vec3b>((Point(x, y))) = {0, 0, 0};
                }
                else
                {
                    v = val;
                    dstBW.at<Vec3b>((Point(x, y))) = {255, 255, 255};
                    dstGS.at<Vec3b>((Point(x, y))) = {v, v, v};

                    if (dir == 0)
                    {
                        dstColor.at<Vec3b>((Point(x, y))) = {255, 0, 0};
                    }
                    else if (dir == -PI / 2)
                    {
                        dstColor.at<Vec3b>((Point(x, y))) = {0, 255, 0};
                    }
                    else if (dir == PI / 4)
                    {
                        dstColor.at<Vec3b>((Point(x, y))) = {0, 0, 255};
                    }
                    else if (dir == -PI / 4)
                    {
                        dstColor.at<Vec3b>((Point(x, y))) = {255, 255, 0};
                    }
                }
            }
        }
    }
    else
    {
        for (int x = 0; x < image.cols; x++)
        {
            for (int y = 0; y < image.rows; y++)
            {
                unsigned int index = y + x * image.rows;
                std::pair<float, double> result = buffer.at(index);
                float val = result.first;
                double dir = result.second;

                dstColor.at<Vec3b>((Point(x, y))) = {255, 255, 255};

                if (dir == -1 || val == 0)
                    dstColor.at<Vec3b>((Point(x, y))) = {0, 0, 0};

                else
                {
                    v = val;

                    dstBW.at<Vec3b>((Point(x, y))) = {255, 255, 255};
                    dstGS.at<Vec3b>((Point(x, y))) = {v, v, v};

                    if (dir < 60)
                        dstColor.at<Vec3b>((Point(x, y))) = {255, 0, 0};
                    else if (dir > 120)
                        dstColor.at<Vec3b>((Point(x, y))) = {0, 0, 255};
                    else
                        dstColor.at<Vec3b>((Point(x, y))) = {0, 255, 0};
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////

unsigned int checkIndex(unsigned int x, unsigned int y)
{
    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows)
        return y + x * image.rows;
    else
        return image.rows * image.cols - 1;
}

void refiningApply(std::vector<std::pair<float, double>> &buffer, const bool &multi)
{
    unsigned int index;
    unsigned int indexG = 0;
    unsigned int indexD = 0;

    std::pair<float, double> result;
    float val;
    double dir;
    for (int x = 1; x < image.cols - 1; x++)
    {
        for (int y = 1; y < image.rows - 1; y++)
        {
            index = y + x * image.rows;
            result = buffer.at(index);
            val = result.first;
            dir = result.second;

            if (val != 0)
            {
                if (multi)
                {
                    if (dir == -PI / 2)
                    { // vertical or diagonal
                        indexG = checkIndex(x, y - 1);
                        indexD = checkIndex(x, y + 1);
                    }
                    else
                    { // horizontal
                        indexG = checkIndex(x - 1, y);
                        indexD = checkIndex(x + 1, y);
                    }
                }
                else
                {
                    if (dir < 60 || dir > 120)
                    { // horizontal
                        indexG = checkIndex(x, y - 1);
                        indexD = checkIndex(x, y + 1);
                    }
                    else
                    { // vertical
                        indexG = checkIndex(x - 1, y);
                        indexD = checkIndex(x + 1, y);
                    }
                }
            }

            if (buffer.at(indexG).first != 0 || buffer.at(indexD).first != 0)
            {
                if (val < buffer.at(indexG).first || val < buffer.at(indexD).first)
                {
                    buffer.at(index).first = 0.f;
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////

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

    // Set parameters
    bool multi;
    bool global;
    bool hysteresis;
    bool refining;
    unsigned int mask;
    unsigned int threshold;

    process_command_line(argc, argv, multi, global, hysteresis, refining, mask, threshold);

    cvtColor(image, image, cv::COLOR_BGR2GRAY);

    dstColor = Mat(image.rows, image.cols, CV_8UC3);
    dstBW = Mat(image.rows, image.cols, CV_8UC3);
    dstGS = Mat(image.rows, image.cols, CV_8UC3);

    std::vector<std::pair<float, double>> buffer; // contains a module and an angle

    // Convolution parts
    if (multi)
    {
        convolution_multi(mask, buffer, threshold, global);
    }
    else
    {
        convolution_bidir(mask, buffer, threshold, global);
    }

    // Thresholding parts
    if (global)
    {
        globalThreshold(buffer);
    }
    else if (hysteresis)
    {
        seuillagehysteresis(buffer, threshold);
    }
    else
    {
        fixedThreshold(buffer, threshold);
    }

    // Refining part
    if (refining)
    {
        refiningApply(buffer, multi);
    }

    // Coloring part
    colorImage(buffer, multi);

    namedWindow("Display Colors", WINDOW_AUTOSIZE);
    imshow("Display Colors", dstColor);

    namedWindow("Display BW", WINDOW_AUTOSIZE);
    imshow("Display BW", dstBW);

    namedWindow("Display GS", WINDOW_AUTOSIZE);
    imshow("Display GS", dstGS);

    // waitKey(0);

    std::vector<Mat> bgr_planes;
    split(image, bgr_planes);
    int histSize = 255;
    float range[] = {0, 255}; // the upper boundary is exclusive
    const float *histRange[] = {range};
    bool uniform = true, accumulate = false;
    Mat b_hist, g_hist, r_hist;

    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    for (int i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
             Scalar(255, 0, 0), 2, 8, 0);
    }
    // imshow("Source image", image);
    // imshow("calcHist Demo", histImage);
    waitKey(0);

    destroyAllWindows();

    return 0;
}
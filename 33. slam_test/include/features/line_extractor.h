/*
 * @Author: Liu Weilong
 * @Date: 2021-02-01 16:36:25
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-07 16:15:14
 * @FilePath: /3rd-test-learning/33. slam_test/include/features/line_extractor.h
 * @Description: 
 * 
 *              为了防止 opencv 3.4 的liscense 的问题 和 opencv3.3 没有 param 选项的问题，最后出此下策
 *              将LSD集成到代码内部
 * 
 * 
 * 
 */
#include <iostream>
#include <string>

#include "Eigen/Eigen"
#include "opencv2/core/core.hpp"

#include "line.h"

#include "opencv2/core/core.hpp"
#include "opencv2/line_descriptor/descriptor.hpp"

using namespace std;
using namespace cv;
using namespace cv::line_descriptor;


class CV_EXPORTS LSDDetectorC : public Algorithm
{
public:


LSDDetectorC()
{
}
;

struct LSDOptions{
    int    refine;
    double scale;
    double sigma_scale;
    double quant;
    double ang_th;
    double log_eps;
    double density_th;
    int    n_bins;
    double min_length;
} options;

/** @brief Creates ad LSDDetectorC object, using smart pointers.
 */
static Ptr<LSDDetectorC> createLSDDetectorC();

/** @brief Detect lines inside an image.

@param image input image
@param keypoints vector that will store extracted lines for one or more images
@param scale scale factor used in pyramids generation
@param numOctaves number of octaves inside pyramid
@param mask mask matrix to detect only KeyLines of interest
 */
void detect( const Mat& image, CV_OUT std::vector<KeyLine>& keylines, int scale, int numOctaves, const Mat& mask = Mat() );
void detect( const Mat& image, CV_OUT std::vector<KeyLine>& keylines, int scale, int numOctaves, LSDOptions opts, const Mat& mask = Mat() );
void detectFast( const Mat& image, CV_OUT std::vector<KeyLine>& keylines, int scale, int numOctaves, LSDOptions opts, const Mat& mask = Mat() );


/** @overload
@param images input images
@param keylines set of vectors that will store extracted lines for one or more images
@param scale scale factor used in pyramids generation
@param numOctaves number of octaves inside pyramid
@param masks vector of mask matrices to detect only KeyLines of interest from each input image
*/
void detect( const std::vector<Mat>& images, std::vector<std::vector<KeyLine> >& keylines, int scale, int numOctaves,
const std::vector<Mat>& masks = std::vector<Mat>() ) const;


private:
/* compute Gaussian pyramid of input image */
void computeGaussianPyramid( const Mat& image, int numOctaves, int scale );

/* implementation of line detection */
void detectImpl( const Mat& imageSrc, std::vector<KeyLine>& keylines, int numOctaves, int scale, const Mat& mask ) const;
void detectImpl( const Mat& imageSrc, std::vector<KeyLine>& keylines, int numOctaves, int scale, LSDOptions opts, const Mat& mask ) const;
void detectImplFast( const Mat& imageSrc, std::vector<KeyLine>& keylines, int numOctaves, int scale, LSDOptions opts, const Mat& mask ) const;

/* matrices for Gaussian pyramids */
std::vector<cv::Mat> gaussianPyrs;
};



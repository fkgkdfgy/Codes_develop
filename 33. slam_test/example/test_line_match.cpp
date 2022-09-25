/*
 * @Author: Liu Weilong
 * @Date: 2021-02-01 16:08:26
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-07 17:04:37
 * @FilePath: /3rd-test-learning/33. slam_test/example/test_line_match.cpp
 * @Description: 
 */

#include <iostream>
#include <string>

#include "camera_model.h"
#include "data_loader.h"
#include "preprocess.h"
#include "tic_toc.hpp"
#include "display.h"
#include "line_extractor.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/line_descriptor/descriptor.hpp"
#include "opencv2/features2d.hpp"

using namespace std;
using namespace cv;
using namespace cv::line_descriptor;


int main()
{
    DataLoader dl("./config/all_path.yaml");
    MyneyePreprocessor mp("./config/undistort.yml");
    CameraModelPinhole cm("./config/undistort.yml");
    cv::Mat img_l_raw,img_r_raw,img_l,img_r;
    TicToc tictoc_lsd("LSD"),tictoc_lbd("LBD"); 
    // Ptr<FastLineDetector> fld_ = createFastLineDetector(60,1.414213562f,50,50,3,false);
    
    LSDDetectorC::LSDOptions opts;
    opts.scale        = 0.5;   //0.8   	The scale of the image that will be used to find the lines. Range (0..1].
    opts.sigma_scale  = 0.6;	//0.6  	Sigma for Gaussian filter. It is computed as sigma = _sigma_scale/_scale.
    opts.quant        = 2.0;	//2.0   Bound to the quantization error on the gradient norm
    opts.ang_th       = 10;	//22.5	Gradient angle tolerance in degrees
    opts.log_eps      = 1.0;	//0		Detection threshold: -log10(NFA) > log_eps. Used only when advance refinement is chosen
    opts.density_th   = 0.6;	//0.7	Minimal density of aligned region points in the enclosing rectangle.
    opts.n_bins       = 1024;	//1024 	Number of bins in pseudo-ordering of gradient modulus.
    double min_line_length = 0.125;  // Line segments shorter than that are rejected
    
    Ptr<LSDDetectorC> lsd_ = LSDDetectorC::createLSDDetectorC();
    
    Ptr<BinaryDescriptor> lbd_ = BinaryDescriptor::createBinaryDescriptor();

    Ptr<BinaryDescriptorMatcher> bdm_ = BinaryDescriptorMatcher::createBinaryDescriptorMatcher();
    while(dl.LoadImg(img_l_raw,img_r_raw))
    {

        // 去畸变
        mp.UnDistort(img_l_raw,img_r_raw,img_l,img_r);

        // resize 一下
        Mat img_line_l = img_l.rowRange(img_l.rows/6,img_l.rows/6*5);
        Mat img_line_r = img_r.rowRange(img_r.rows/6,img_r.rows/6*5);
        // LSD提取
        tictoc_lsd.Tic();
        std::vector<cv::line_descriptor::KeyLine> lsd_l,lsd_r,keylsd_l,keylsd_r;
        lsd_->detect(img_l,lsd_l,2,2,opts);
        lsd_->detect(img_r,lsd_r,2,2,opts);
        tictoc_lsd.Toc();
        
        // 第一次筛选

        for ( int i = 0; i < (int) lsd_l.size(); i++ )
        {
            if( lsd_l[i].octave == 0 && lsd_l[i].lineLength >= 60 && abs(lsd_l[i].angle)<1.6&&abs(lsd_l[i].angle)>1.4)
            {
                keylsd_l.push_back( lsd_l[i] );
            }
        }

        for ( int i = 0; i < (int) lsd_r.size(); i++ )
        {
            if( lsd_r[i].octave == 0 && lsd_r[i].lineLength >= 60&& abs(lsd_r[i].angle)<1.6&&abs(lsd_r[i].angle)>1.4)
            {
                keylsd_r.push_back( lsd_r[i] );
            }
        }

        // LBD
        tictoc_lbd.Tic();
        Mat keylbd_descr_r, keylbd_descr_l;
        Ptr<BinaryDescriptor> bd_ = BinaryDescriptor::createBinaryDescriptor(  );
        bd_->compute( img_line_r, keylsd_r, keylbd_descr_r );
        bd_->compute( img_line_l, keylsd_l, keylbd_descr_l );
        tictoc_lbd.Toc();
        
        // Match
        // std::vector<DMatch> lsd_matches;
        // bdm_->match(keylbd_descr_r, keylbd_descr_l, lsd_matches);


        // 生成plucker 进行二次筛选
        
        // 去非竖直线

        // 线段显示(image+pangolin)
        DrawLine(img_l,keylsd_l);
        cv::imshow("Left Camera",img_l);
        cv::waitKey(0);
    }
    
    
}
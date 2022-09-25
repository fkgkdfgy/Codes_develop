/*
 * @Author: Liu Weilong 
 * @Date: 2020-09-01 15:05:28 
 * @Last Modified by: Liu Weilong
 * @Last Modified time: 2020-09-01 15:35:29
 */


#include <iostream>
#include "glog/logging.h"

int main(int argc, char ** argv)
{
	FLAGS_alsologtostderr = 1;
	google::InitGoogleLogging(argv[0]); // 括号内是程序名 如果没有这一条 log 文件的标记会出问题log_info_19700101-080000 1970年的log
	// 大于glog_info 等级的log 都会被放入 log 文件当中

	// LOG 使用
	bool logIfFlag = true;
    google::SetLogDestination(google::GLOG_INFO,"/home/sunny/workspace_private/3rd-test-learning/1. glog/log/"); 
	LOG(INFO) << "I am INFO!";
	LOG(WARNING) << "I am WARNING!";
	// LOG(ERROR) << "I am ERROR!";
	//LOG(FATAL) << "I am FATAL!";
	
	LOG_IF(INFO,logIfFlag)<<"HHHH"<<std::endl;

	//LOG_EVERY_N

	// CHECK 使用
	
	CHECK(false)<<"there is something wrong with xxx"<<std::endl;
    
	return 0;
}

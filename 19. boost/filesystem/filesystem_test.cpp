/*
 * @Author: Liu Weilong
 * @Date: 2020-10-25 17:15:21
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-26 15:08:47
 * @Description:  学习如何使用filesystem 进行文件夹的操作
 * 
 *                get_filenames 主要用于统计路径下的文件数 和文件名
 */

#include <boost/filesystem.hpp> 
#include <iostream> 
#include <string>
#include <fstream> 
#include <vector>

namespace fs = boost::filesystem;  

     
int get_filenames(const std::string& dir, std::vector<std::string>& filenames)   //获取文件名和个数
{  
    fs::path path(dir);  
    if (!fs::exists(path))  //判断文件路径是否为空
    {  
        return -1;  
    }  
    
    fs::directory_iterator end_iter;  
    for (fs::directory_iterator iter(path); iter!=end_iter; ++iter)  
    {
        // 打印文件名
        if (fs::is_regular_file(iter->status()))  
        {  
            filenames.push_back(iter->path().string()); 
            std::cout<<iter->path().filename()<<std::endl;
            // filename() 的文件名没有之前的 长路经 只有例如 filesystem.cpp 这种文件名字
            std::cout<<iter->path().stem()<<std::endl;
            // stem() 输出单一文件名，例如 filesystem_test
            std::cout<<iter->path().extension()<<std::endl;
            // extension() 输出后缀名 例如 .cpp
        }  
      
        if (fs::is_directory(iter->status()))  
        {  
            get_filenames(iter->path().string(), filenames);//是目录则递归  
        }  
    }  
     



    return filenames.size();  //返回文件大小
}  
 
int main(int argc,char *argv[])
{
    std::vector<std::string> vName;
    get_filenames(argv[1],vName);
    return 0;
}

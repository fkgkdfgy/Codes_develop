/*
 * @Author: Liu Weilong
 * @Date: 2021-05-01 22:27:16
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-02 11:06:10
 * @Description: 
 * 
 *  这个文件 主要用于存储和测试各种好用的 io 函数 
 *
 */


#include <boost/filesystem.hpp>
#include <algorithm>

namespace fs = boost::filesystem;
int get_filenames(const std::string& dir, std::vector<std::string>& filenames)
{
	fs::path path(dir);
	if (!fs::exists(path))
	{
		return -1;
	}
 
	fs::directory_iterator end_iter;
	for (fs::directory_iterator iter(path); iter!=end_iter; ++iter)
	{
		if (fs::is_regular_file(iter->status()))
		{
			filenames.push_back(iter->path().string());
		}
 
		if (fs::is_directory(iter->status()))
		{
			get_filenames(iter->path().string(), filenames);
		}
	}
 
	return filenames.size();
}


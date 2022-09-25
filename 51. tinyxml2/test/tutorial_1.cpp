/*
 * @Author: Liu Weilong
 * @Date: 2021-06-30 10:26:51
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-30 14:29:46
 * @Description: 
 * 
 * tutorial 1: xml 加载 + 内部存储逻辑
 * 内部存储逻辑
 * 
 * eg. 
 * <sdf version = "1.6"><data> 123123123</data> </sdf>
 * 
 * sdf 是elememt 的名字
 * version 是sdf 的一个 attribute 
 * 1.6 version的value
 * data 也是一个element ,它是sdf下面的child
 * 123123123 是一个txt, 他也是sdf下面的child
 * 
 * 
 */

#include <iostream>
#include "src/tinyxml2.h"

using namespace tinyxml2;


int main()
{
    XMLDocument doc;
    doc.LoadFile("/home/lwl/workspace/3rd-test-learning/51. tinyxml2/test/model.sdf");

    // 这里的element 就是sdf
    XMLElement* element = doc.FirstChildElement();
    
    


    
}
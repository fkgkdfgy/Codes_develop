/*
 * @Author: Liu Weilong
 * @Date: 2021-06-30 15:30:55
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-30 15:40:03
 * @Description: 
 * 
 * 配套 tinyxml2 使用的小函数
 */

#include <vector>
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

XMLElement * FindElement(XMLElement * parent,
                         const char * name ,
                         const char * attribute,const char * value)
{
    XMLElement* userNode=root->FirstChildElement(name);
    while(userNode!=NULL)
    {
        if(userNode->Attribute(attribute)==value)
            break;
        userNode=userNode->NextSiblingElement();//下一个兄弟节点
    }
    return userNode;
}




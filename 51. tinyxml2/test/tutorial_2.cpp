/*
 * @Author: Liu Weilong
 * @Date: 2021-06-30 10:26:51
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-30 19:06:58
 * @Description: 
 * 
 * tutorial 1: xml 内部的添加删除修改
 * 
 */
/** 目前的版本
 <?xml version='1.0'?>
<sdf version='1.6'>
  <model name='wall'>
    <pose frame=''>-3.14877 -4.56357 0 0 -0 0</pose>
    <link name='Wall_0'>
      <collision name='Wall_0_Collision'>
        <geometry>
          <box>
            <size>5.75 0.15 2.5</size>
          </box>
        </geometry>
        <pose frame=''>0 0 1.25 0 -0 0</pose>
      </collision>
      <visual name='Wall_0_Visual'>
        <pose frame=''>0 0 1.25 0 -0 0</pose>
        <geometry>
          <box>
            <size>5.75 0.15 2.5</size>
          </box>
        </geometry>
        <material>
          <script>
            <uri>file://media/materials/scripts/gazebo.material</uri>
            <name>Gazebo/Grey</name>
          </script>
          <ambient>1 1 1 1</ambient>
        </material>
      </visual>
      <pose frame=''>-0.851226 1.85357 0 0 -0 0</pose>
    </link>
    <static>1</static>
  </model>
</sdf>
 */
#include <iostream>
#include "src/tinyxml2.h"

using namespace tinyxml2;
using namespace std;


int main()
{
    XMLDocument doc;
    doc.LoadFile("/home/lwl/workspace/3rd-test-learning/51. tinyxml2/test/model.sdf");
    
    // 这里的element 就是sdf
    XMLElement* sdf = doc.FirstChildElement();
    // 得到model
    XMLElement* model = sdf->FirstChildElement();
    // 得到link Wall_0
    XMLElement* wall_0 = model->FirstChildElement("link");
    // 复制一份得到 Wall_1
    XMLElement* wall_1 = wall_0->DeepClone(&doc)->ToElement();
    wall_1->SetAttribute("name","Wall_1");
    XMLElement * wall_1_collision = wall_1->FirstChildElement("collision");
    if(wall_1_collision == NULL){
        cout<<"error no collision"<<endl;
    }
    wall_1_collision->SetAttribute("name","Wall_1_Collision");
    XMLElement * box = wall_1_collision->FirstChildElement()->FirstChildElement()->FirstChildElement();
    box->SetText("1000 1000 1000");
    model->InsertAfterChild(wall_0,wall_1);
    doc.SaveFile("/home/lwl/workspace/3rd-test-learning/51. tinyxml2/test/result.sdf");

    


    
}
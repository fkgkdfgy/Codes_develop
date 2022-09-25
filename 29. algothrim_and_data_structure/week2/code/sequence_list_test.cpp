/*
 * @Author: Liu Weilong
 * @Date: 2021-01-21 22:32:39
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-02-23 20:33:24
 * @Description: 
 */
#include "sequence_list.h"


void showItem(const SequenceList<Item> & sl)
{
    const Item * i_p = sl.data_c();
    for(int i=0;i<sl.Length();i++)
    {
        cout<<"the idx is "<< i_p[i].a<<endl;
    }
}

void showItemWithGetElem(const SequenceList<Item> & sl)
{   
    for(int i=0;i<sl.Length();i++)
    {
        Item temp;
        sl.GetElem(i,temp);
        cout << "the idx : " << temp.a << endl;
    }
}

void sum(Item & f)
{
    f.a +=1;
}

int main()
{
    // 
    SequenceList<Item> sl(10);
    for(int i=0;i<12;i++)
    {
        Item temp(i);
        sl.InsertElem(0,temp);
    }
    // showItem(sl);
    // showItemWithGetElem(sl);
    // 
    for(int i=0;i<12;i++)
    {
        size_t idx = 20;
        bool flag = sl.LocateElem(i,idx);
        cout<<"find?: " <<flag<<" , idx: "<<idx<<endl;
    }
    
    // 
    for(int i=0;i<10;i++)
    {
        Item t1,t2;
        bool flag1 = sl.NextElem(i,t1);
        bool flag2 = sl.PriorElem(i,t2);
        cout<<"front?: "<< flag1 <<", the front: "<< t1.a <<endl;
        cout<<"later?: "<< flag2 <<", the later: "<< t2.a <<endl;
    }
    
    // 
    
    sl.Traverse(sum);

    showItem(sl);
}
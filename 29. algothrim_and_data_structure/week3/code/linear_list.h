/*
 * @Author: Liu Weilong
 * @Date: 2021-01-20 21:42:24
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-02-23 21:05:27
 * @Description: 
 */
#include <iostream>
#include <functional>
using namespace std;

class Item
{
    public:
    Item():a(9999),b(9999){}
    Item(int a_t,int b_t=1000):a(a_t),b(b_t){}

    bool operator==(const Item & item)
    {
        if(a==item.a&&b ==item.b)
        return true;
        return false;
    }
    int a,b;
};


class Node
{
    public:
    Item content_;
    Node * pnext_;
};

typedef Node* Linklist;

template<typename T>
class LinearList
{
    public:
    LinearList(size_t capacity):capacity_(capacity){}
    virtual bool Init() =0 ;
    virtual bool Destroy() =0;
    virtual bool ClearList() {length_=0;}
    virtual bool Empty()const {if(length_==0)return true;return false;}
    virtual size_t Length() const {return length_;}
    virtual bool GetElem(size_t idx, T &e) const=0;
    virtual bool LocateElem(const T & e,size_t & idx)const =0;
    virtual bool PriorElem(const T & cur_e, T & pre_e)const =0;
    virtual bool NextElem(const T & cur_e, T & next_e) const=0;
    virtual bool InsertElem(size_t idx, const T & e) =0;
    virtual bool DeleteElem(size_t idx, T & e) =0;
    virtual bool Traverse(std::function<void(T&)> f) =0;
    bool CheckIdx(size_t idx) const {if(idx<=length_-1)return true; return false;}
    virtual ~LinearList(){}
    size_t capacity_;
    size_t length_;
};
/*
 * @Author: Liu Weilong
 * @Date: 2021-02-23 20:45:16
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-02-24 21:54:43
 * @Description: 
 */
#include "linear_list.h"
#include <functional>

#define DEPRECATED cerr<<"this function is deprecated!!"<<endl;\
return false;




class PointerList: public LinearList<Item>
{
    public:
    PointerList();
    virtual bool Init()  override ;
    virtual bool Destroy()  override;
    virtual bool ClearList()  override;
    virtual bool GetElem(size_t idx, Item &e, Node* & p ) const ;
    virtual bool GetElem(size_t idx, Item & e)const override{DEPRECATED}
    virtual bool LocateElem(const Item & e, size_t & idx, Node * & p)const ;
    virtual bool LocateElem(const Item & e ,size_t & idx) const override{DEPRECATED}
    virtual Node *Locate(size_t idx);
    virtual bool PriorElem(const Item & cur_e, Item & pre_e)const  override{};
    virtual bool NextElem(const Item & cur_e, Item & next_e) const override{};
    virtual bool InsertElem(size_t idx, const Item & e)  override;
    virtual bool DeleteElem(size_t idx, Item & e)  override;
    virtual bool Traverse(std::function<void(Item&)> f)  override;
    virtual bool Empty() const override;
    virtual size_t Length() const override;
    
    Linklist L_;
    
};

PointerList::PointerList():LinearList<Item>(0){Init();}
bool PointerList::Init(){L_ = new Node;L_->pnext_ = nullptr;return true;}
bool PointerList::Empty()const{if(L_->pnext_ == nullptr)return true; return false;}
bool PointerList::Destroy()
{
    Node * next;
    while(L_)
    {
        next = L_;
        L_ = L_->pnext_;
        delete next;
    }
}
bool PointerList::ClearList()
{
    Node* origin_L = L_;
    L_ = L_->pnext_;
    Node * next;
    while(L_)
    {
        next = L_;
        L_ = L_->pnext_;
        delete next;
    }
    L_ = origin_L;
}
size_t PointerList::Length()const 
{
    const Node * next = L_;
    size_t count = 0;
    while(next)
    {
        count++;
        next = next->pnext_;
    }
    return count;
}
bool PointerList::GetElem(size_t idx, Item &e, Node *& p) const
{
    if(Empty())
    return false;
    if(idx>Length()-1)
    return false;
    Node * next = L_->pnext_;
    size_t count = 0;
    while(count < idx)
    {
        next = next->pnext_;
        count++;
    }
    next = next->pnext_;
    e = next->content_;
    p = next;
    return true;
}
bool PointerList::LocateElem(const Item & e, size_t & idx, Node * & p)const
{
    Node * next = L_->pnext_;
    size_t count = 0;
    while(next)
    {
        if(next->content_ == e)
        {
            idx = count;
            p = next;
            return true;
        }
        next = next->pnext_;
        count++;
    }
    return false;
}
Node * PointerList::Locate(size_t idx)
{
    if(idx==-1)
    return L_;
    if(idx>Length()-1)
    return nullptr;
    Node * next = L_->pnext_;
    size_t count = 0;
    while(count!=idx)
    {
        next = next->pnext_;
        count++;
    }
    return next;
}
bool PointerList::InsertElem(size_t idx, const Item & e)
{
    if(Empty())return false;
    if((idx-1)>Length()-2)
    return false;
    Node * p;
    Item temp;
    p = Locate(idx-1);
    Node * new_p = new Node;
    new_p->content_ = e;
    new_p->pnext_ = p->pnext_;
    p->pnext_ = new_p;
    return true;
}
bool PointerList::DeleteElem(size_t idx, Item & e)
{
    if(Empty())return false;
    if((idx-1)>Length()-2)
    return false;
    Node * p,*t;
    Item temp;
    p = Locate(idx-1);
    t = p->pnext_;
    p->pnext_ = p->pnext_->pnext_;
    e = t->content_;
    delete t;
    return true;
}

bool PointerList::Traverse(std::function<void(Item&)> f){}
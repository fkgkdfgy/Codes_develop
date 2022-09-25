/*
 * @Author: Liu Weilong
 * @Date: 2021-01-20 22:07:19
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-02-23 20:47:02
 * @Description: 
 */
#include "linear_list.h"
#include "assert.h"


template <typename T1>
class SequenceList:public LinearList<T1>
{
    public:
    SequenceList(size_t capacity);
    virtual bool Init()  override ;
    virtual bool Destroy()  override;
    virtual bool ClearList()  override;
    virtual bool GetElem(size_t idx, T1 &e) const override;
    virtual bool LocateElem(const T1 & e,size_t & idx)const  override;
    virtual bool PriorElem(const T1 & cur_e, T1 & pre_e)const  override;
    virtual bool NextElem(const T1 & cur_e, T1 & next_e) const override;
    virtual bool InsertElem(size_t idx, const T1 & e)  override;
    virtual bool DeleteElem(size_t idx, T1 & e)  override;
    virtual bool Traverse(std::function<void(T1&)> f)  override;
    const T1 * data_c() const{return pool_;}
    T1 * data() {return pool_;}
    T1 * pool_;
};


template<typename T1>
SequenceList<T1>::SequenceList(size_t capacity):LinearList<T1>(capacity){Init();}

template<typename T1>
bool SequenceList<T1>::Init()
{
    pool_ = new T1[this->capacity_]();
    this->length_ = 0;
    cout<<"the SequenceList init "<<endl;
}

template<typename T1>
bool SequenceList<T1>::Destroy()
{
    delete [] pool_;
    this->capacity_ = 0;
    this->length_ = 0;
    return true;
}

template<typename T1>
bool SequenceList<T1>::ClearList() 
{
    LinearList<T1>::ClearList();
    Destroy();
    pool_ = new T1[this->capacity_]();
    this->length_ = 0;
    return true;
}

template<typename T1>
bool SequenceList<T1>::GetElem(size_t idx, T1 &e) const
{
    if(!this->CheckIdx(idx))
    return false;
    e = pool_[idx];
    return true;
}

template<typename T1>
bool SequenceList<T1>::LocateElem(const T1 & e,size_t & idx)const
{ 
    for(size_t i=0;i<this->length_;i++)
    {
        if(pool_[i]==e)
        {
            idx = i;
            return true;
        }
    }
    return false;
}

template<typename T1>
bool SequenceList<T1>::PriorElem(const T1 & cur_e, T1 & pre_e)const
{
    size_t idx =0;
    bool ok = LocateElem(cur_e,idx);
    if(idx==0||!ok)
    return false;
    GetElem(idx-1,pre_e);
    return true;
}

template<typename T1>
bool SequenceList<T1>::NextElem(const T1 & cur_e, T1 & next_e) const
{
    size_t idx =0;
    bool ok = LocateElem(cur_e,idx);
    if(idx==(this->length_-1)||!ok)
    return false;
    GetElem(idx+1,next_e);
    return true;
}

template<typename T1>
bool SequenceList<T1>::InsertElem(size_t idx, const T1 & e) 
{
    if(!this->CheckIdx(idx)&& this->length_!=0)
    return false;
    assert(this->length_<=this->capacity_);
    if(this->length_==this->capacity_)
    return false;
    this->length_++;
    for(size_t i = this->length_-1;i!=idx;i--)
    {
        pool_[i] = pool_[i-1];
    }
    pool_[idx] = e;
    return true;
}

template<typename T1>
bool SequenceList<T1>::DeleteElem(size_t idx, T1 & e)
{
    if(!this->CheckIdx(idx))
    return false;

    e=pool_[idx];
    
    for(size_t i = idx;i<(this->length_-1);i++)
    {
        pool_[i] = pool_[i+1];
    }
    this->length_--;
    return true;    
}

template<typename T1>
bool SequenceList<T1>::Traverse(std::function<void(T1&)> f)
{
    for(size_t i=0;i<this->length_;i++)
    {
        f(pool_[i]);
    }
}
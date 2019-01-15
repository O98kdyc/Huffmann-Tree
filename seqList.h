#ifndef SEQLIST_H_INCLUDED
#define SEQLIST_H_INCLUDED

#include <iostream>
#include <math.h>


using namespace std;

class OutOfBound{};
class IllegalSize{};

template <class elemType>
class seqList
{
private:
    elemType *data;
    int currentLength;
    //int maxSize;
    void doubleSpace();
    bool checkIndex(int idx);
    void reduceSpace(double precentage=0.25,double reduceRatio=0.5);
public:
    int maxSize;
    seqList(int initSize = 10);
    ~seqList()  {delete [] data;}
    void clear()  {currentLength = 0;}
    int length() const  {return currentLength;}
    void insert(int i, const elemType &x);
    void remove(int i);
    int search(const elemType &x) const ;
    elemType visit(int i); //此处之后本有const，加上后无法运行
    void traverse() const ;
    void halfRemove();
    int lastIndexOf(const elemType &x) const;
};

template<class elemType>
int seqList<elemType>::search (const elemType&x) const
{

  int num = 0;
  for (num = 0; num < currentLength; ++ num)
  if  (data[num] == x) break;
  if (num == currentLength) num = -1;
  return num;
}


template<class elemType>
int seqList<elemType>::lastIndexOf (const elemType&x) const
{
  int num = 0;
  for (num = currentLength-1; num >=0; -- num)
  if  (data[num] == x) break;
  if (num == -1) num = -1;
  return num;
}

template <class elemType>
void seqList<elemType>::doubleSpace()
{
  elemType *tmp = data;
  maxSize *= 2;
  data = new elemType[maxSize];
  for (int i = 0; i < currentLength; ++i)
          data[i] = tmp[i];
  delete [] tmp;
}

template <class elemType>
void seqList<elemType>::insert(int i, const elemType &x)
{
  if (checkIndex(i)) throw OutOfBound();
  if (currentLength == maxSize) doubleSpace();
  for (int j = currentLength; j > i; j--)  data[j] = data[j-1];
  data[i] = x;
  ++currentLength;
}

template <class elemType>
void seqList<elemType>::remove(int i)
{
 if (checkIndex(i)||i==currentLength)
           throw OutOfBound();
  for (int j = i; j < currentLength - 1; j++)  data[j] = data[j+1] ;
  --currentLength;
  reduceSpace(0.25,0.5);
  //cout<<endl<<currentLength<<endl;
}

template <class elemType>
seqList<elemType>::seqList(int initSize)
{
  if (initSize <= 0) throw IllegalSize();
  data = new elemType[initSize];
  maxSize = initSize;
  currentLength = 0;
}

template <class elemType>
elemType seqList<elemType>::visit(int i)
{
   if(!checkIndex(i)&&i!=currentLength) return data[i];
   else throw OutOfBound();
}

template <class elemType>
void seqList<elemType>::traverse() const
{
    cout<<endl;
    for (int i=0;i<currentLength;++i) //<<data[i];
         cout<<data[i]<<" ";
}

template <class elemType>
void seqList<elemType>::halfRemove()
{
    int i=0,m=currentLength;
    if(currentLength%2==0)
    {
    for(i=0;i<m/2;i++)
    {
        data[i]=data[2*i];
    }
    currentLength=currentLength/2;
    };
    if(m%2==1)
    {
    for(i=0;i<m/2+0.5;i++)
    {
        data[i]=data[2*i];
    }
    currentLength=(currentLength+1)/2;
    };
}

template <class elemType>
void seqList<elemType>::reduceSpace(double precentage,double reduceRatio)
{
    elemType *tmp = data;
    if (currentLength<precentage*maxSize) maxSize=round(maxSize*reduceRatio);
    data = new elemType[maxSize];
    for (int i = 0; i < currentLength; ++i) data[i] = tmp[i];
    delete [] tmp;
}

template <class elemType>
bool seqList<elemType>::checkIndex(int idx)
{
    if(idx>=0&&idx<=currentLength) return 0;
    if(idx>currentLength||idx<0) return 1;
}


#endif // SEQLIST_H_INCLUDED

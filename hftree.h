#ifndef HFTREE_H_INCLUDED
#define HFTREE_H_INCLUDED
#include <fstream>
#include "seqList.h"
using namespace std;

template <class Type>
class hfTree{
private:
    struct Node
    {  //数组中的元素类型
        Type data;  //结点值
        int weight;  //结点的权值
        int parent, left, right;  //父结点及左右儿子的下标地址
    };

    Node *elem;
    //int length;
    void calculateWeight(char *filename);

public:
    int length;
    void getCode();
    void enCode (char *filename);
    void deCode(char *filename);
    struct hfCode{  //保存哈夫曼编码的类型
        Type data;  //待编码的字符
        string code;  //对应的哈夫曼编码
    };

    hfTree(const Type *v, const int *w, int size);
    hfTree(char *filename);
    void getCode(hfCode result[]);
    ~hfTree(){delete []elem;};
};

template <class Type>
hfTree<Type>::hfTree(const Type *v, const int *w, int size) {
    const int MAX_INT = 32767;
    int min1,min2;  //最小树 次小树的权值
    int x,y;  //次小树 最小树的下标

    length = 2*size;
    elem = new Node[length];
    //置初值
    for (int i = size; i < length; ++i) {
        elem[i].weight = w[i-size];
        elem[i].data = v[i-size];
        elem[i].parent = elem[i].left = elem[i].right = 0;
    }

    //归并森林中的树
    for (int i = size-1; i > 0; --i) {
        min1 = min2 = MAX_INT;
        x = y = 0;
        for (int j = i+1; j < length; ++j)   //找到权值最小的树
            if(elem[j].parent == 0)
                if (elem[j].weight < min1) {  //元素j最小
                    min2 = min1;
                    min1 = elem[j].weight;
                    x = y;
                    y = j;
                }
                else if (elem[j].weight < min2) {  //元素次小
                    min2 = elem[j].weight;
                    x = j;
                }
            //cout<<min1<<" "<<min2<<endl;
            elem[i].weight = min1 + min2;
            elem[i].left = x;
            elem[i].right = y;
            elem[i].parent = 0;
            elem[x].parent = i;
            elem[y].parent = i;
        }


    //for(int i=0;i<length;i++) cout<<elem[i].parent<<" ";
    //cout<<endl;
    //for(int i=0;i<length;i++) cout<<elem[i].weight<<" ";
}

template <class Type>
void hfTree<Type>::getCode(hfTree<Type>::hfCode result[]) {
    int size = length/2;
    int p,s;  //s是追溯过程中正在处理的结点，p是s的父结点下标

    for (int i = size; i < length; ++i) {
        result[i - size].data = elem[i].data;
        result[i - size].code = "";
        p = elem[i].parent;
        s = i;
        while(p){  //向根追溯
            if(elem[p].left == s)
                result[i - size].code = '0' + result[i - size].code;
            else result[i - size].code = '1' + result[i - size].code;
            s = p;
            p = elem[p].parent;
        }
    }

}

template <class Type>
void hfTree<Type>::calculateWeight(char *filename)
{
    ifstream fin(filename);
    if(!fin) {cout<<"cannot open input file\n";return;}
    seqList<Type> v;
    seqList<int> u;
    Type x;
    //fin>>x;
    x=fin.get();
    //cout<<x<<endl;
    v.insert(0,x);
    u.insert(0,1);
    while(!fin.eof())
    {
        //cout<<x<<endl;
        //cout<<1<<endl;
        x=fin.get();
        if(v.search(x)==-1) {v.insert(v.length(),x);u.insert(u.length(),1);}
        else {u.insert(v.search(x),u.visit(v.search(x))+1);u.remove(v.search(x)+1);}
    }
    //v.traverse();
    //u.traverse();
    length=2*v.length();
    elem=new Node[length];

    for(int i=v.length();i<length;++i)
    {
        elem[i].weight=u.visit(i-v.length());
        elem[i].data=v.visit(i-v.length());
        elem[i].parent=elem[i].left=elem[i].right=0;
    }
}

template <class Type>
hfTree<Type>::hfTree(char *filename)
{
    calculateWeight(filename);
    int size=length/2;
    const int MAX_INT = 32767;
    int min1,min2;  //最小树 次小树的权值
    int x,y;  //次小树 最小树的下标
    for (int i = size-1; i > 0; --i) {
        min1 = min2 = MAX_INT;
        x = y = 0;
        for (int j = i+1; j < length; ++j)   //找到权值最小的树
            if(elem[j].parent == 0)
                if (elem[j].weight < min1) {  //元素j最小
                    min2 = min1;
                    min1 = elem[j].weight;
                    x = y;
                    y = j;
                }
                else if (elem[j].weight < min2) {  //元素次小
                    min2 = elem[j].weight;
                    x = j;
                }

            elem[i].weight = min1 + min2;
            elem[i].left = x;
            elem[i].right = y;
            elem[i].parent = 0;
            elem[x].parent = i;
            elem[y].parent = i;
        }
   }

template <class Type>
void hfTree<Type>::getCode()
{
    int siz=length/2;
    hfTree<char>::hfCode result[siz];
    getCode(result);
    for (int i = 0; i < siz; ++i)
        cout<<result[i].data<<' '<<result[i].code<<endl;
}

template <class Type>
void hfTree<Type>::enCode(char *filename)
{
    int siz=length/2;
    hfTree<char>::hfCode result[siz];
    getCode(result);
    ifstream fin(filename);
    ofstream fout("huffmann.txt");
    if(!fin) {cout<<"cannot open input file\n";return;}
    if(!fout) {cout<<"cannot create file\n";return;}
    Type x;
    while(!fin.eof())
    {
        x=fin.get();
        for(int i=0;i<siz;i++)
        {
            if(result[i].data==x) fout<<result[i].code;
        }
    }
}

template <class Type>
void hfTree<Type>::deCode(char *filename)
{
    int siz=length/2;
    hfTree<char>::hfCode result[siz];
    getCode(result);
    ifstream fin(filename);
    ofstream fout("result.txt");
    if(!fin) {cout<<"cannot open input file\n";return;}
    if(!fout) {cout<<"cannot create file\n";return;}
    Type x;
    string s="";
    while(!fin.eof())
    {
        x=fin.get();
        s=s+x;
        //cout<<s<<" ";
        for(int i=0;i<siz;i++)
        {
            if(result[i].code==s)
            {
                fout<<result[i].data;
                s="";
            }
        }
    }
}

#endif // HFTREE_H_INCLUDED

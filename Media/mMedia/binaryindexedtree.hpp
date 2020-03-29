#ifndef BINARYINDEXEDTREE_H
#define BINARYINDEXEDTREE_H

#include <vector>
#include <initializer_list>


//单点修改，区间查询
template<class T>
class BTTree
{
public:
    BTTree(const std::initializer_list<T> &init);
    BTTree(const std::vector<T> &init);
    int getLen();

    void updata(T v,int x);
    T getSum(int x);

private:
    std::vector<T> values;
    int len;
};


template<class T>
BTTree<T>::BTTree(const std::initializer_list<T> &init)
    :values(init.size()+1),len{init.size()}
{
    int i=0;
    for(auto val: init){
        updata(val, ++i);
    }
}

template<class T>
BTTree<T>::BTTree(const std::vector<T> &init)
    :values(init.size()+1),len(init.size())
{
    int i=0;
    for(auto val: init){
        updata(val, ++i);
    }
}

template<class T>
int BTTree<T>::getLen()
{
    return len;
}

template<class T>
void BTTree<T>::updata(T v,int x)
{
//    while (x < len) {
//        values[x] += v;
//        x += x&-x;
//    }
    for(;x<=len; x += x&-x)  values[x] += v;
}

template<class T>
T BTTree<T>::getSum(int x)
{
    T ans = T();
    for(; x>0; x -= x&-x)
        ans += values[x];
    return ans;
}


#endif // BINARYINDEXEDTREE_H

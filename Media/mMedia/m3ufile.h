#ifndef M3UFILE_H
#define M3UFILE_H

#include <map>
#include "binaryindexedtree.hpp"

class m3uFile
{
public:
    m3uFile(std::string file);

    int length();
    int getHeadValue(std::string str);
    double getExtinfSum(int n);
    double getExtinf(int n);

private:
    std::map<std::string, int> _head;
    BTTree<double> * _body;
};

#endif // M3UFILE_H

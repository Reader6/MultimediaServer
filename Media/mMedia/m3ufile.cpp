#include "m3ufile.h"
#include <fstream>

using std::ifstream;
using std::string;
using std::map;
using std::vector;

m3uFile::m3uFile(std::string file)
{
    ifstream iss(file);

    string temp;
    iss >> temp;

    if(temp != "#EXTM3U")
        throw "m3u8文件头部出错";


    vector<double> body;
    string key,value;
    while (iss >> temp) {

        if(temp == "#EXT-X-ENDLIST")//结束
            break;

        string::size_type pos = temp.find(':');
        if(pos == string::npos){
            throw "未能找到分割符':',文件分析失败";
        }

        key = temp.substr(0, pos);
        if(key != "#EXTINF"){
            value = temp.substr(pos+1, temp.length()-1);
            _head[key] = atoi(value.c_str());
        }
        else {
            value = temp.substr(pos+1, temp.length()-2);
            body.push_back(atof(value.c_str()));

            iss >> temp;//跳过一行
        }
    }

    _body = new BTTree<double>(body);
}

int m3uFile::length()
{
    return _body->getLen();
}

int m3uFile::getHeadValue(std::string str)
{
    auto iter = _head.find(str);
    if(iter == _head.end())
        throw string("不存在该键值" + str);
    return iter->second;
}

double m3uFile::getExtinfSum(int n)
{
    //n [0, len)
    if(n >= _body->getLen() || n < 0){
        throw "索引出错";
    }
    return _body->getSum(n+1);//[1,len+1]
}

double m3uFile::getExtinf(int n)
{
    if(n <= 0){
        throw "索引出错";
    }

    return getExtinfSum(n) - getExtinfSum(n-1);
}

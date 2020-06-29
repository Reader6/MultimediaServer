#include "mediasourse.h"


#include <unistd.h>
#include <QDebug>
#include <QDir>

#include "filedownload.h"
#include "downthread.h"

MediaSourse::MediaSourse(QObject *parent)
    :QObject(parent),
      available{false}, file{nullptr}, alreadyTs{nullptr},len{-1}{}

bool MediaSourse::loadSourse(QString url)
{
    if(available == true){
        qDebug() << "当前源已加载";
        return false;
    }

    if(url.isEmpty()){
        qDebug() << "参数为空";
        return false;
    }

    sourse = url;
    QString soursefile = url + "/index.m3u8";
    if(!FileDownload::get_instance().downLoad(soursefile, true)){
        qDebug() << "加载源文件"<<soursefile<<"失败";
        return false;
    }

    QString s = FileDownload::localaddr + soursefile;
    file = new m3uFile(s.toStdString());
    alreadyTs = new QVector<bool>(file->length());
    len = alreadyTs->length();
    for (int i=0; i<len; i++) {
        (*alreadyTs)[i] = isTsFileOk(i);
    }
    available = true;
    return true;
}

int MediaSourse::loadTsFile(int index, bool b)
{
    if(!available){
        qDebug() << "当前源不可用";
        return -1;
    }

    if(index < 0) return -1;
    if(index > len-1) return -1;

    if((*alreadyTs).at(index)){
        qDebug() << index << ".ts，已存在";
        return 0;
    }

    QString url = QString(sourse + "/index%1.ts").arg(index);
    if(!FileDownload::get_instance().downLoad(url, b)){
        qDebug() << "加载文件"<<url<<"失败";
        return 1;
    }

    (*alreadyTs)[index] = true;
    return 0;

}

int MediaSourse::getDuration(int index)
{
    if(!available){
        qDebug() << "当前源不可用";
        return -1;
    }
    return static_cast<int>(file->getExtinfSum(index) * 1000);
}

QString MediaSourse::absDir()
{
    QDir dir = FileDownload::localaddr;
    return dir.absolutePath();
}

void MediaSourse::clear()
{
    available = false;
    len = -1;
    sourse.clear();
    if( file != nullptr){
        delete file;
    }
    if( alreadyTs != nullptr){
        delete alreadyTs;
    }
}

int MediaSourse::length() const
{
    if(available) return len;
    return -1;
}

int MediaSourse::poslocation(int postion)
{
    //(,]
    if(postion < 0) return -1;
    double sec = static_cast<double>(postion/1000);

    int left = 0;
    int right = file->length()-1;//[left:0 right:len-1]
//    for(int i=0; i<right; i++)
//        if(x < postime[i])
//            return i;
    if(sec > file->getExtinfSum(right))
        return -1;

    while(left != right){
        int m = (left+right)/2;
        if(sec < file->getExtinfSum(m)){//[0,m]
            right = m;
        }else {//[m+1 , right]
            left = m+1;
        }
    }
    return left;
    //return -1;
}

bool MediaSourse::isTsFileOk(int i)
{
    if((*alreadyTs).at(i)) return true;

    QString path = QString(sourse + "/index%1.ts").arg(i);
    if(access(path.toStdString().c_str(),F_OK) == 0){
        return true;
    }
    return false;
}

MediaSourse::~MediaSourse()
{
    clear();
}

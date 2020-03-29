#include "mediasourse.h"
#include "workthread.h"
#include "curltool.h"

#include <unistd.h>
#include <QDebug>
#include <QDir>

const QString media_addr("http://127.0.0.1/");

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
    WorkThread work(media_addr + url + "/index.m3u8");
    connect(&work,SIGNAL(finished()),&work,SLOT(deleteLater()));
    work.start();
    if(work.wait(5000) == false){//5秒
        qDebug() << "加载m3u : " + url;
        return false;
    }
    file = new m3uFile(CurlDownload::rootDir+ QString(sourse+"/index.m3u8").toStdString() );
    alreadyTs = new QVector<bool>(file->length());
    len = alreadyTs->length();
    for (int i=0; i<len; i++) {
        (*alreadyTs)[i] = isTsFileOk(i);
    }
    available = true;
    return true;
}

int MediaSourse::loadTsFile(int index)
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

    QString url = QString(media_addr + sourse + "/index%1.ts").arg(index);
    WorkThread work(url);
    work.start();
    if(work.wait(30000) == false){//30秒
        qDebug() << "加载超时 : " + url;
        return -2;
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
    //index [0, len)
    return static_cast<int>(file->getExtinfSum(index) * 1000);
}

QString MediaSourse::absDir()
{
    return QDir("./").absolutePath();
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

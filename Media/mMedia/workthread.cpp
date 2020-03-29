#include "workthread.h"
#include "curltool.h"

#include <QMutex>
#include <QDebug>

//CurlDownload &curlSing = CurlDownload::get_instance();

WorkThread::WorkThread(QString url)
    :_url{url}
{}

void WorkThread::run()
{
    CurlDownload &curlSing = CurlDownload::get_instance();
    curlSing.downLoad(string(_url.toStdString()).c_str());
    qDebug() << "任务完成" << _url;
}

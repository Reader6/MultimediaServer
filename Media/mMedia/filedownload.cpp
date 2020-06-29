#include "filedownload.h"

#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QUrl>
#include "downthread.h"

QString FileDownload::addrport = "127.0.0.1:8080/";
QString FileDownload::localaddr = "./mediaData/";

FileDownload::FileDownload()
{
    rootDir = new QDir();
    if(!rootDir->exists(localaddr)){
        if(!rootDir->mkdir(localaddr)){
            qDebug() << "创建根目录失败";
            return;
        }
    }
    rootDir->cd(localaddr);
}

bool FileDownload::downLoad(QString  &url, bool wait)
{
    QDir *fileDir = new QDir(*rootDir);

    QStringList list = url.split("/");
    if(!list.isEmpty()) list.removeLast();

    QStringListIterator it(list);

    while (it.hasNext()){
        QString name = it.next();
        if(!fileDir->exists(name)){
            if(!fileDir->mkdir(name)){
                qDebug() << "创建目录失败失败";
                return false;
            }
        }
        fileDir->cd(name);
    }

    QString  str(rootDir->absolutePath() + "/" + url);
    QString httputl("http://"+ addrport + url);

    DownThread *th = new DownThread(str, httputl);
    if(wait) {
        QTimer timer;
        timer.setSingleShot(true);
        QEventLoop loop;
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        connect(th, SIGNAL(finished()), &loop, SLOT(quit()));
        timer.start(10000);   // 10 secs. timeout
        th->downLoad();
        loop.exec();


        if(timer.isActive()) {
            timer.stop();
        } else {
            qDebug() << "超时";
            disconnect(th, SIGNAL(finished()), &loop, SLOT(quit()));
            delete th;
            return false;
        }
    }
    else {
        th->downLoad();
        return true;
    }

    return true;
}

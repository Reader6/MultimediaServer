#include "downthread.h"

int DownThread::downLoad()
{
    startRequest(url);
    return 0;
}

void DownThread::httpFinished()
{
    emit finished();
    file->flush();
    file->close();
    reply->deleteLater();
    reply = nullptr;
    delete file;
}

void DownThread::httpReadyRead()
{
    if(file) file->write(reply->readAll());

}

void DownThread::startRequest(QUrl url)
{
    /*
    mutex.lock();
    reply = manager->get(QNetworkRequest(url));
//    connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
//    connect(reply,SIGNAL(finished()),this,SLOT(httpFinished()));

    QTimer timer;
    timer.setSingleShot(true);

    QEventLoop loop;
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(2000);   // 10 secs. timeout
    loop.exec();

    if(timer.isActive()) {
        timer.stop();
    } else {
        qDebug() << "超时";
        disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        mutex.unlock();
        reply->abort();
        reply->close();
        reply->deleteLater();
    }*/

    reply = manager->get(QNetworkRequest(url));

    connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
    connect(reply,SIGNAL(finished()),this,SLOT(httpFinished()));

}

DownThread::DownThread(QString &dir, QString  &httpurl, QObject *parent)
    :QObject(parent)
{
    manager = new QNetworkAccessManager();
    file = new QFile(dir);
    if(!file->open(QIODevice::WriteOnly)) {
        qDebug()<<"file open error";
        delete file;
        file = nullptr;
        throw QString("打开文件失败");
    }
    url = httpurl;
}


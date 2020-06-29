#ifndef DOWNTHREAD_H
#define DOWNTHREAD_H

#include <QFile>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QObject>
#include <QThread>

class DownThread : public QObject
{
    Q_OBJECT //使用信号与槽函数
public:
    explicit DownThread(QString &dir, QString  &httpurl, QObject *parent = nullptr);
    int downLoad();
//声明信号
signals:
    void finished();

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;
    QString url;
private slots:
    void httpFinished();
    void httpReadyRead();
private:
    void startRequest(QUrl url);
};


#endif // DOWNTHREAD_H

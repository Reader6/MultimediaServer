#ifndef FILEDOWNLOAD_H
#define FILEDOWNLOAD_H

#include <QObject>

class QDir;
class QUrl;
class QFile;
class QNetworkReply;
class QNetworkAccessManager;

class FileDownload : public QObject
{
    Q_OBJECT
public:
    virtual ~FileDownload(){}
    FileDownload(const FileDownload&)=delete;
    FileDownload& operator=(const FileDownload&)=delete;

    static FileDownload& get_instance(){
        static FileDownload instance;
        return instance;
    }
    static void setAddrPort(QString addr, int port){
        QString newAddrPort= QString("%1:%2").arg(addr).arg(port);
        addrport = newAddrPort;
    }
    static void setLocaladdr(QString addr){
        addrport = addr;
    }

    bool downLoad(QString &url,  bool wait = false);
    static QString addrport;
    static QString localaddr;
private:
    QDir *rootDir;
private:
    FileDownload();
};

#endif // FILEDOWNLOAD_H

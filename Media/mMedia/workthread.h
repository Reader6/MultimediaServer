#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>


class WorkThread : public QThread
{
public:
    WorkThread(QString url);

    void run() override;

private:
    QString _url;
};

#endif // WORKTHREAD_H

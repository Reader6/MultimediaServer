#ifndef MEDIASOURSE_H
#define MEDIASOURSE_H

#include <QObject>

#include <m3ufile.h>
#include <QVector>

class MediaSourse : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(int length READ length)

    explicit MediaSourse(QObject *parent = nullptr);

    Q_INVOKABLE bool loadSourse(QString url);
    Q_INVOKABLE int loadTsFile(int index, bool b = false);
    Q_INVOKABLE int poslocation(int postion);

    Q_INVOKABLE int getDuration(int index);
    Q_INVOKABLE QString absDir();
    Q_INVOKABLE void clear();
    Q_INVOKABLE bool isTsFileOk(int i);

    int length() const;

    ~MediaSourse();
private:
    bool available;
    QString sourse;
    m3uFile *file;
    QVector<bool> *alreadyTs;
    int len;
};

#endif // MEDIASOURSE_H

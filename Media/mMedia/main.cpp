#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "mediasourse.h"


bool checkPermission();

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    MediaSourse *mediaSourse = new MediaSourse;
    engine.rootContext()->setContextProperty("mediaSourse", mediaSourse);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}


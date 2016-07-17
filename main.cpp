#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "tk15.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<tk15>("Gyco", 1, 0, "TK15");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

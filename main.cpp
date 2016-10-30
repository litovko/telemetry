#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "tk15.h"
#include <QDebug>
#include <QTime>


#define giko_name "HYCO"
#define giko_program "TK-15 telemetry"

static QFile logfile;
static QTextStream out(&logfile);
static bool recordinglog=false;

extern void toggle_log(bool recordlog) {
    if (!recordlog) {
        if(logfile.isOpen()) {
            logfile.write("Close\n");
            logfile.flush();
            logfile.close();
        }
        recordinglog=false;
        return;
    }
    if (!logfile.isOpen()) {
        logfile.setFileName("hyco_tk-15_log_"+QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss-zzz.log"));
        logfile.open(QIODevice::WriteOnly | QIODevice::Text);
        logfile.write("Open\n");
    }
    recordinglog=true;
}
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "D:%s %s (%s:%u, %s)\n",QTime::currentTime().toString("hh:mm:ss:zzz ").toLocal8Bit().data(), localMsg.constData(), context.file, context.line, context.function);
        out<<"Debug:"<<QTime::currentTime().toString("hh:mm:ss:zzz ").toUtf8().data()<<" "<<localMsg.constData()<<"("<<context.file<<":"<<context.line<<", "<<context.function<<")\n";
        break;
    case QtWarningMsg:
        fprintf(stderr, "W:%s %s (%s:%u, %s)\n",QTime::currentTime().toString("hh:mm:ss:zzz ").toLocal8Bit().data(), localMsg.constData(), context.file, context.line, context.function);
        out<<"Warning:"<<QTime::currentTime().toString("hh:mm:ss:zzz ").toLocal8Bit().data()<<" "<<localMsg.constData()<<"("<<context.file<<":"<<context.line<<", "<<context.function<<")\n";
        break;
    case QtCriticalMsg:
        fprintf(stderr, "C:%s %s (%s:%u, %s)\n",QTime::currentTime().toString("hh:mm:ss:zzz ").toLocal8Bit().data(), localMsg.constData(), context.file, context.line, context.function);
        out<<"Critical:"<<QTime::currentTime().toString("hh:mm:ss:zzz ").toLocal8Bit().data()<<" "<<localMsg.constData()<<"("<<context.file<<":"<<context.line<<", "<<context.function<<")\n";
        break;
    case QtFatalMsg:
        fprintf(stderr, "F:%s %s (%s:%u, %s)\n",QTime::currentTime().toString("hh:mm:ss:zzz ").toLocal8Bit().data(), localMsg.constData(), context.file, context.line, context.function);
        out<<"FATAL:"<<QTime::currentTime().toString("hh:mm:ss:zzz ").toLocal8Bit().data()<<" "<<localMsg.constData()<<"("<<context.file<<":"<<context.line<<", "<<context.function<<")\n";
        abort();
    }
    if(logfile.isOpen()) logfile.flush();
    if(logfile.isOpen()) logfile.flush();
}

int main(int argc, char *argv[])
{
    int ret;
    qInstallMessageHandler(myMessageOutput);
    toggle_log(true);
    setlocale(LC_ALL, ""); // избавляемся от кракозябров в консоли
    qDebug()<<QTime::currentTime().toString("hh:mm:ss:zzz ")<<"Старт"<<giko_name<<"  "<<giko_program;
    QGuiApplication app(argc, argv);
    app.setOrganizationName("HYCO");
    app.setOrganizationDomain("hyco.ru");
    app.setApplicationName("TK15");
    qmlRegisterType<tk15>("Gyco", 1, 0, "TK15");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    try {
        ret = app.exec();
    }
    catch (const std::bad_alloc &) {
        // clean up here, e.g. save the session
        // and close all config files.
        if(logfile.isOpen()) {
            logfile.flush();
            logfile.close();
        }

        return 0; // exit the application
    }
    qDebug()<<QTime::currentTime().toString("hh:mm:ss:zzz ")<<"Стоп"<<giko_name<<"  "<<giko_program;
    toggle_log(false);
    return ret;
}

#ifndef TK15_H
#define TK15_H

#include <QQuickItem>
#include "qstring.h"
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>

class tk15 : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QStringList list READ list NOTIFY listChanged)

    Q_PROPERTY(bool client_connected READ client_connected NOTIFY client_connectedChanged)

    Q_PROPERTY(int timer_connect_interval READ timer_connect_interval WRITE setTimer_connect_interval NOTIFY timer_connect_intervalChanged)
//    Овершот 1 - лампочка
    Q_PROPERTY(bool overshort_1 READ overshort_1 NOTIFY overshort_1Changed)
//    Овершот 2 - лампочка
    Q_PROPERTY(bool overshort_2 READ overshort_2 NOTIFY overshort_2Changed)
//    Температура
    Q_PROPERTY(double temperature READ temperature NOTIFY temperatureChanged)
//    Давление
    Q_PROPERTY(double pressure READ pressure NOTIFY pressureChanged)
//    Угол наклона 1
    Q_PROPERTY(double angle1 READ angle1 NOTIFY angle1Changed)
//    Угол наклона 2
    Q_PROPERTY(double angle2 READ angle2 NOTIFY angle2Changed)
//    Коррекция угла наклона 1
    Q_PROPERTY(double angle1k READ angle1k NOTIFY angle1kChanged)
//    Коррекция угла наклона 2
    Q_PROPERTY(double angle2k READ angle2k NOTIFY angle2kChanged)
//    Датчик тока 1
    Q_PROPERTY(double current1 READ current1 NOTIFY current1Changed)
//    Датчик тока 2
    Q_PROPERTY(double current2 READ current2 NOTIFY current2Changed)
//    Датчик тока 3
    Q_PROPERTY(double current3 READ current3 NOTIFY current3Changed)
//    Датчик напряжения
    Q_PROPERTY(double voltage READ voltage NOTIFY voltageChanged)

public:
    tk15();
    ~tk15();

    QString address() const;
    void setAddress(const QString &address);

    int port() const;
    void setPort(int port);

    QString data() const;
    void setData(const QString &data);

    QStringList list() const;

    bool client_connected() const;
    void setClient_connected(bool client_connected);

    int timer_connect_interval() const;
    void setTimer_connect_interval(int timer_connect_interval);

    double temperature() const;
    void setTemperature(double temperature);

    double pressure() const;
    void setPressure(double pressure);

    double angle1() const;
    void setAngle1(double angle1);

    double angle2() const;
    void setAngle2(double angle2);

    double current1() const;
    void setCurrent1(double current1);

    double current2() const;
    void setCurrent2(double current2);

    double current3() const;
    void setCurrent3(double current3);

    double voltage() const;
    void setVoltage(double voltage);

    bool overshort_1() const;
    void setOvershort_1(bool overshort_1);

    bool overshort_2() const;
    void setOvershort_2(bool overshort_2);

    double current1k() const;
    void setCurrent1k(double current1k);

    double current2k() const;
    void setCurrent2k(double current2k);

    double current3k() const;
    void setCurrent3k(double current3k);

    double voltagek() const;
    void setVoltagek(double voltagek);

    double pressurek() const;
    void setPressurek(double pressurek);

    double angle1k() const;
    void setAngle1k(double angle1k);

    double angle2k() const;
    void setAngle2k(double angle2k);

    bool tcp() const;
    void setTcp(bool tcp);

signals:
    void addressChanged();
    void portChanged();
    void dataChanged();
    void listChanged();
    void temperatureChanged();
    void pressureChanged();
    void angle1Changed();
    void angle2Changed();
    void angle1kChanged();
    void angle2kChanged();
    void current1Changed();
    void current2Changed();
    void current3Changed();
    void voltageChanged();
    void overshort_1Changed();
    void overshort_2Changed();

    void client_connectedChanged();

    void timer_connect_intervalChanged();

public slots:
    void clientConnected();  // слот для обработки события присоединения клиента к серверу.
    void clientDisconnected();
    void displayError(QAbstractSocket::SocketError socketError);
    void readData(); //раскладываем полученные от сервера данные по параметрам
    void start_client();

    //UDP
    void onClientReadyRead();
private:
    //TCP
    QString m_address="localhost";
    int m_port=9999;

    QTcpSocket tcpClient;
    bool m_client_connected = false;
    bool m_tcp=true;
    //UDP
    QUdpSocket *m_client;
    int m_udpcount=0;
    //DATA UDP
    QString m_data="null";
    //SETTINGS
    void saveSettings();
    void readSettings();
    QStringList m_list;
    void fill_list();
    //TIMING
    QTimer timer_connect;
    int m_timer_connect_interval=20000;
    //DATA
    QByteArray Data="";
    double m_temperature=14;

    double m_pressure=109;
    double m_pressurek=1.5;
    double m_angle1=1;
    double m_angle2=3;
    double m_angle1k=0;
    double m_angle2k=0;
    double m_current1=20;
    double m_current2=17;
    double m_current3=19;
    double m_voltage=408;
    double m_current1k=1;
    double m_current2k=1;
    double m_current3k=1;
    double m_voltagek=1;
    bool m_overshort_1=false;
    bool m_overshort_2=false;
};

#endif // TK15_H

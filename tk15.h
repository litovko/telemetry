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
    Q_PROPERTY(long count READ count WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(QString data READ data WRITE setData NOTIFY dataChanged)
    //Q_PROPERTY(QStringList list READ list NOTIFY listChanged)
    Q_PROPERTY(bool tcp READ tcp NOTIFY tcpChanged)
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

    //QStringList list() const;

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

    long count() const;
    void setCount(long count);

signals:
    void addressChanged();
    void portChanged();
    void dataChanged();
    void tcpChanged();
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
    void countChanged();

public slots:
    void clientConnected();  // слот для обработки события присоединения клиента к серверу.
    void clientDisconnected();
    void displayError(QAbstractSocket::SocketError socketError);
    void readData(); //раскладываем полученные от сервера данные по параметрам
    void start_client();

    //UDP
    void onClientReadyRead();

    void fill_list();
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
    //QStringList m_list;

    //TIMING
    QTimer timer_connect;
    QTimer timer_showdata;
    long m_count=0;
    int m_timer_connect_interval=20000;
    //DATA
    QByteArray Data="";
    uint16_t CRC16(QByteArray data);
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

    const uint8_t crc8_tabl[256]={
    0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
    157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
    35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
    190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
    70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
    219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
    101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
    248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
    140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
    17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
    175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
    50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
    202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
    87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
    233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
    116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53};
    uint8_t CRC8(QByteArray data);
};

#endif // TK15_H

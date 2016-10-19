#include "tk15.h"
#include <QTcpSocket>
#include <QSettings>
#include <QDebug>
#include <QTimer>

tk15::tk15()
{
    qDebug()<<"Конструктор класса ТК15";
    readSettings();
    connect(&tcpClient, SIGNAL(connected()),this, SLOT(clientConnected())); // Клиент приконнектилася к указанному адресу по указанному порту.
    connect(&tcpClient, SIGNAL(disconnected()),this, SLOT(clientDisconnected())); // Клиент отвалился
    connect(&tcpClient, SIGNAL(readyRead()),this, SLOT(readData()));
    connect(&tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(&timer_connect, SIGNAL(timeout()), this, SLOT(start_client()));



    if(m_tcp) {
        timer_connect.start(m_timer_connect_interval);
        start_client();
    }
    else {
        m_client = new QUdpSocket();
        m_client->bind(QHostAddress(address()), port());
        qDebug()<<"Bind address:"<<address()<<" Bind port: "<<port();
        //UDP
        connect(m_client, SIGNAL(readyRead()), SLOT(onClientReadyRead()));
    }


}

tk15::~tk15()
{
    saveSettings();
    qDebug()<<"Деструктор класса ТК15";
    if(!m_tcp) delete m_client;
}


QString tk15::address() const
{
    return m_address;
}

void tk15::setAddress(const QString &address)
{
    m_address = address;
    emit addressChanged();
}

int tk15::port() const
{
    return m_port;
}

void tk15::setPort(int port)
{
    m_port = port;
    emit portChanged();
}



bool tk15::client_connected() const
{
    return m_client_connected;
}

void tk15::setClient_connected(bool client_connected)
{
    if (m_client_connected == client_connected) return;
    m_client_connected = client_connected;
    emit client_connectedChanged();
}

void tk15::saveSettings()
{
    QSettings settings("HYCO", "TK15");
    settings.setValue("Address",m_address);
    settings.setValue("Port",m_port);
    settings.setValue("TCP",m_tcp);
    settings.setValue("K_pressure",m_pressurek);
    settings.setValue("K_current1",m_current1k);
    settings.setValue("K_current2",m_current2k);
    settings.setValue("K_current3",m_current3k);
    settings.setValue("K_voltage",m_voltagek);
    settings.setValue("K_angle1k",m_angle1k);
    settings.setValue("K_angle2k",m_angle2k);


}

void tk15::readSettings()
{
    QSettings settings("HYCO", "TK15");
    setAddress(settings.value("Address","192.168.1.168").toString());
    setPort(settings.value("Port", 9999).toInt());
    setTcp(settings.value("TCP", true).toBool());
    setPressurek(settings.value("K_pressure",1).toDouble());
    setCurrent1k(settings.value("K_current1",1).toDouble());
    setCurrent2k(settings.value("K_current2",1).toDouble());
    setCurrent3k(settings.value("K_current3",1).toDouble());
    setVoltagek(settings.value( "K_voltage", 1).toDouble());
    setAngle1k(settings.value( "K_angle1k", 0).toDouble());
    setAngle2k(settings.value( "K_angle2k", 0).toDouble());
    qDebug()<<"tcp:"<<tcp()<<"v:"<<voltagek()<<" p:"<<pressurek()<<" c1:"<<current1k()<<" c2:"<<current2k()<<" c2:"<<current2k();
}

int tk15::timer_connect_interval() const
{
    return m_timer_connect_interval;
}

void tk15::setTimer_connect_interval(int timer_connect_interval)
{
    if(m_timer_connect_interval == timer_connect_interval) return;
    m_timer_connect_interval = timer_connect_interval;
    emit timer_connect_intervalChanged();
}

QStringList tk15::list() const
{
    return m_list;
}


QString tk15::data() const
{
    return m_data;
}

void tk15::setData(const QString &data)
{
    m_data = data;
    emit dataChanged();
}

void tk15::start_client()
{
    if (m_client_connected) return;
    //bytesWritten = 0;
    qDebug()<<"Telemetry Start client >>>"<<m_address<<"poprt"<<::QString().number(m_port);
    tcpClient.connectToHost(m_address, m_port);
}

void tk15::clientConnected()
{
    qDebug()<<"Telemetry Client connected to address >>>"+this->address()+" port:"+ ::QString().number(m_port);
    setClient_connected(true);
    fill_list();

}

void tk15::clientDisconnected()
{
    qDebug()<<" TK15 Telemetry Client disconnected ";
    setClient_connected(false);
    fill_list();
}

void tk15::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)   //litovko надо уточнить зачем эта проверка
        return;
    qDebug()<<"TK15 Network error >>>"+tcpClient.errorString();

    fill_list();
    tcpClient.close();

}

void tk15::onClientReadyRead()
{
    QHostAddress sender;
    quint16 senderPort;
    QByteArray datagram;
    datagram.resize(m_client->pendingDatagramSize());
    m_client->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    //setData(datagram);
    Data.append(datagram);
    qDebug()<<"UDP Datagram["<<datagram<<"] from addr:"<<sender<<"port:"<<senderPort;
    readData();
    m_udpcount+=1;
}

bool tk15::tcp() const
{
    return m_tcp;
}

void tk15::setTcp(bool tcp)
{
    m_tcp = tcp;
}
/*
Передача цифровых датчиков
№	Содержание	Описание	Примечание
байта	НЕХ
1	55	Стартовый
2	31	Код
3	хх	Байт 1	Содержание
4	хх	Байт 2	байтов на листе
5	хх	Байт 3	 "Протокол"
6	хх	Байт 4	строки
7	хх	Байт 5	 284 - 295
8	хх	Байт 6
9	хх	Байт 7
10	хх	Байт 8
11	хх	Байт 9
12	00	Резерв
13	00	Резерв
14	00	Резерв
15	CRC16	Контрольная
16	CRC16	сумма


Передача аналоговых датчиков
№	Содержание	Описание	Примечание
байта	НЕХ
1	55	Стартовый
2	32	Код
3	хх	Байт 1	Содержание
4	хх	Байт 2	байтов на листе
5	хх	Байт 3	 "Протокол"
6	хх	Байт 4	строки
7	хх	Байт 5	 360 - 370
8	хх	Байт 6
9	хх	Байт 7
10	хх	Байт 8
11	хх	Байт 9
12	хх	Байт 10
13	хх	Байт 11
14	хх	Байт 12
15	хх	Байт 13
16	хх	Байт 14
17	хх	Байт 15
18	00	Резерв
19	CRC16	Контрольная
20	CRC16	сумма
*/
void tk15::readData()
{
    const int len_digital = 16;
    const int len_analog  = 20;
    const char type_digital = 0x31;
    const char type_analog = 0x32;
    QByteArray Datagramma="";
    quint16 crc=0;
    quint16 crc0=0;
    char d_type=0;

    if (m_tcp) {
        Datagramma = tcpClient.readAll();
        Data.append(Datagramma);
    }

    qDebug()<<"TK15 telemetry data read :"<<Data.toHex()<<Data.length();
    if (Data.length()<2) return;
    int i=Data.indexOf(0x55);

    while (i>=0)
    {
        d_type=Data.at(i+1);
        qDebug()<<"i="<<i<<"t:"<<int(d_type);
        if (d_type==type_digital) Datagramma=Data.mid(i, len_digital);
        if (d_type==type_analog) Datagramma=Data.mid(i, len_analog);
        crc = qChecksum(Datagramma.data(), Datagramma.length()-2);
        qDebug()<<"Datagramma:"<<Datagramma.mid(0,Datagramma.length()-2).toHex();
        qDebug()<<"len:" <<Datagramma.length();
        qDebug()<<Datagramma[Datagramma.length()-1]*1;
//        unsigned char c1=Datagramma[Datagramma.length()-1];
//        unsigned char c2=Datagramma[Datagramma.length()-2];
//        crc0=c1*256+c2; qDebug()<<"c1:"<<c1<<" c2:"<<c2;
        crc0=(unsigned char)Datagramma[Datagramma.length()-1]*256+(unsigned char)Datagramma[Datagramma.length()-2]*1;
        QByteArray b=""; b.append(Datagramma[Datagramma.length()-1]).append(Datagramma[Datagramma.length()-2]);
        qDebug()<<"crc:"<<b.toHex();
        qDebug()<<"TK15 telemetry crc0 :"<<::QString().number(crc0);
        qDebug()<<"TK15 telemetry data SRC :"<<::QString().number(crc);
        m_list.append("Данные:"+Datagramma.toHex());
        m_list.append("CRC:"+::QString().number(crc)+"<>"+::QString().number(crc0));
        if(d_type==type_digital) {
            Data=Data.mid(i+len_digital, Data.length());
            setOvershort_1(Datagramma.at(2)^1);
            setOvershort_2(Datagramma.at(2)^2);

            setTemperature(Datagramma.at(3)|Datagramma.at(4)<<8);

            setAngle1(-angle1k()+(Datagramma.at(5)|Datagramma.at(6)<<8));
            setAngle2(-angle2k()+(Datagramma.at(7)|Datagramma.at(8)<<8));
        }
        if(d_type==type_analog) {
            Data=Data.mid(i+len_analog, Data.length());
            setVoltage(voltagek()*(Datagramma.at(2)|Datagramma.at(3)<<8));

            setCurrent1(current1k()*(Datagramma.at(4)|Datagramma.at(5)<<8));
            setCurrent2(current2k()*(Datagramma.at(6)|Datagramma.at(7)<<8));
            setCurrent3(current3k()*(Datagramma.at(8)|Datagramma.at(9)<<8));

            setPressure(pressurek()*(Datagramma.at(10)|Datagramma.at(11)<<8));
        }
        qDebug()<<"truncated:"<<Data.toHex();


        i=Data.indexOf(0x55);
    }
    fill_list();

}

void tk15::fill_list()
{
    m_list.clear();
    //m_list.append("Адрес:"+m_address.toLatin1());
    m_list.append("Адрес:"+m_address);
    m_list.append("Порт:"+::QString().number(m_port));
    m_list.append("Коррекция угла 1:"+QString().number(m_angle1k));
    m_list.append("Коррекция угла 2:"+QString().number(m_angle2k));
//    m_list.append("Угол 1:"+QString().number(m_angle1));
//    m_list.append("Угол 2:"+QString().number(m_angle2));
//    m_list.append("Давление:"+QString().number(m_pressure));
//    m_list.append("Ток1:"+QString().number(m_current1));
//    m_list.append("Ток2:"+QString().number(m_current2));
//    m_list.append("Ток3:"+QString().number(m_current3));
//    m_list.append("Напряжение:"+QString().number(m_voltage));
//    m_list.append("Температура:"+QString().number(m_temperature));
//    m_list.append("Овершот1:"+QString().number(m_overshort_1));
//    m_list.append("Овершот2:"+QString().number(m_overshort_2));
    m_list.append(":"); m_list.append(":"); m_list.append(":"); m_list.append(":"); m_list.append(":");
    if(m_tcp) {
      if (client_connected()) m_list.append("Соединение установлено");
      if (!client_connected()) m_list.append("Соединение разорвано");
    }
    else m_list.append("UDP:"+QString().number(m_udpcount));
    emit listChanged();
}

double tk15::angle2k() const
{
    return m_angle2k;
}

void tk15::setAngle2k(double angle2k)
{
    m_angle2k = angle2k;
    emit angle2kChanged();
}

double tk15::angle1k() const
{
    return m_angle1k;

}

void tk15::setAngle1k(double angle1k)
{
    m_angle1k = angle1k;
    emit angle1kChanged();
}

double tk15::pressurek() const
{
    return m_pressurek;
}

void tk15::setPressurek(double pressurek)
{
    m_pressurek = pressurek;
    emit pressureChanged();
}

double tk15::voltagek() const
{
    return m_voltagek;
}

void tk15::setVoltagek(double voltagek)
{
    m_voltagek = voltagek;
    emit voltageChanged();
}

double tk15::current3k() const
{
    return m_current3k;
}

void tk15::setCurrent3k(double current3k)
{
    m_current3k = current3k;
}

double tk15::current2k() const
{
    return m_current2k;
}

void tk15::setCurrent2k(double current2k)
{
    m_current2k = current2k;
}

double tk15::current1k() const
{
    return m_current1k;
}

void tk15::setCurrent1k(double current1k)
{
    m_current1k = current1k;
}

bool tk15::overshort_2() const
{
    return m_overshort_2;
}

void tk15::setOvershort_2(bool overshort_2)
{
    m_overshort_2 = overshort_2;
    emit overshort_2Changed();
}

bool tk15::overshort_1() const
{
    return m_overshort_1;
}

void tk15::setOvershort_1(bool overshort_1)
{
    m_overshort_1 = overshort_1;
    emit overshort_1Changed();
}

double tk15::voltage() const
{
    return m_voltage;
}

void tk15::setVoltage(double voltage)
{
    m_voltage = voltage;
    emit voltageChanged();
}

double tk15::current3() const
{
    return m_current3;
}

void tk15::setCurrent3(double current3)
{
    m_current3 = current3;
    emit current3Changed();
}

double tk15::current2() const
{
    return m_current2;
}

void tk15::setCurrent2(double current2)
{
    m_current2 = current2;
    emit current2Changed();
}

double tk15::current1() const
{
    return m_current1;
}

void tk15::setCurrent1(double current1)
{
    m_current1 = current1;
    emit current1Changed();
}

double tk15::angle2() const
{
    return m_angle2;
}

void tk15::setAngle2(double angle2)
{
    m_angle2 = angle2;
    emit angle2Changed();
}

double tk15::angle1() const
{
    return m_angle1;
}

void tk15::setAngle1(double angle1)
{
    m_angle1 = angle1;
    emit angle1Changed();
}

double tk15::pressure() const
{
    return m_pressure;

}

void tk15::setPressure(double pressure)
{
    m_pressure = pressure;
    emit pressureChanged();
}

double tk15::temperature() const
{
    return m_temperature;
}

void tk15::setTemperature(double temperature)
{
    m_temperature = temperature;
    emit temperatureChanged();
}

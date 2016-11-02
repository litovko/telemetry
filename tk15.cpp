#include "tk15.h"
#include <QTcpSocket>
#include <QSettings>
#include <QDebug>
#include <QTimer>
#include <QTMath>

extern void toggle_log(bool recordlog);

tk15::tk15()
{
    qDebug()<<"Конструктор класса ТК15";
    readSettings();
    connect(&tcpClient, SIGNAL(connected()),this, SLOT(clientConnected())); // Клиент приконнектилася к указанному адресу по указанному порту.
    connect(&tcpClient, SIGNAL(disconnected()),this, SLOT(clientDisconnected())); // Клиент отвалился
    connect(&tcpClient, SIGNAL(readyRead()),this, SLOT(readData()));
    connect(&tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(&timer_connect, SIGNAL(timeout()), this, SLOT(start_client()));
    connect(&timer_showdata, SIGNAL(timeout()), this, SLOT(fill_list()));

    timer_showdata.start(m_timer_showdata_interval);

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

    //qDebug()<<"START CRC8:"<<"U11234567890"<<CRC8("U11234567890");

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
    settings.setValue("Connect_interval", m_timer_connect_interval);
    settings.setValue("Show_interval", m_timer_showdata_interval);


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
    setTimer_connect_interval(settings.value( "Connect_interval", 20000).toInt());
    setTimer_showdata_interval(settings.value( "Show_interval", 500).toInt());
    qDebug()<<"tcp:"<<tcp()
            <<"v:"<<voltagek()
            <<" p:"<<pressurek()
            <<" c1:"<<current1k()<<" c2:"<<current2k()<<" c2:"<<current2k()
            <<"show:"<<m_timer_showdata_interval;
}

int tk15::timer_showdata_interval() const
{
    return m_timer_showdata_interval;
}

void tk15::setTimer_showdata_interval(int timer_showdata_interval)
{
    m_timer_showdata_interval = timer_showdata_interval;
}

long tk15::count() const
{
    return m_count;
}

void tk15::setCount(long count)
{
    m_count = count;
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

//QStringList tk15::list() const
//{
//    return m_list;
//}


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
    //fill_list();

}

void tk15::clientDisconnected()
{
    qDebug()<<" TK15 Telemetry Client disconnected ";
    setClient_connected(false);
    //fill_list();
}

void tk15::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)   //litovko надо уточнить зачем эта проверка
        return;
    qDebug()<<"TK15 Network error >>>"+tcpClient.errorString();

    //fill_list();
    tcpClient.close();

}

void tk15::onClientReadyRead()
{
    QHostAddress sender;
    quint16 senderPort;
    QByteArray datagram;
    try {
    datagram.resize(m_client->pendingDatagramSize());
    m_client->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    //setData(datagram);
    Data.append(datagram);
    setClient_connected(true);
    }
    catch (...) {
        qDebug()<<"ERROR READ DATA FROM UDP";
    }

    //qDebug()<<"UDP Datagram["<<datagram.toHex()<<"] from addr:"<<sender<<"port:"<<senderPort;
    try {
    readData();
    }
    catch (...) {
        qDebug()<<"ERROR READED DATA PROCESSING";
    }
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
3	хх	Байт 1	Овершоты - младшие биты 1 и 2
4	хх	Байт 2	Температура мл
5	хх	Байт 3	Температура ст
6	хх	Байт 4	угол ст (угол в радианах =ASIN(Data-2048)/512))
7	хх	Байт 5	угол мл
8	хх	Байт 6  угол ст
9	хх	Байт 7  угол мл
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
3	хх	Байт 1
4	хх	Байт 2	Температура мл
5	хх	Байт 3	Температура ст
6	хх	Байт 4	угол ст
7	хх	Байт 5	угол мл
8	хх	Байт 6  угол ст
9	хх	Байт 7  угол мл
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
    int l=0;
    double ang;

    if (m_tcp) {
        try {
            Datagramma = tcpClient.readAll();
        }
        catch (...) {
            qDebug()<< "ERROR tcpClient.readAll";
        }
        try {
            Data.append(Datagramma);
        }
        catch (...) {
            qDebug()<< "ERROR Data.append(Datagramma)";
        }

    }  
    Data=Data.right(2*len_analog+2*len_digital+10);

    int i=Data.indexOf(0x55);

    while (i>=0&&Data.length()>=len_digital) //(len_analog<len_digital?len_analog:len_digital)
    {
        qDebug()<<"LENGTH:"<<Data.length()<<"TK15 telemetry DATA:"<<Data.toHex();
        d_type=Data.at(i+1);


        if (d_type==type_digital) {
            Datagramma=Data.mid(i, len_digital);
            if (Datagramma.length()<len_digital) break;
        }
        else if (d_type==type_analog)  {
                Datagramma=Data.mid(i, len_analog);
                if (Datagramma.length()<len_digital) break;
             }
             else {
                Data=Data.mid(i+1);
                i=Data.indexOf(0x55);
                qDebug()<<"WRONG Datagramma type:"<<(int)d_type;
                continue;
             }

        qDebug()<<"i->55="<<i<<"type:"<<d_type<<"Datagramma:"<<Datagramma.toHex();

        crc = CRC8(Datagramma.left(Datagramma.length()-2));
        crc0=(unsigned char)Datagramma[Datagramma.length()-1];
        qDebug()<<"crc0 :"<<::QString().number(crc0)<<"SRC :"<<::QString().number(crc);
        //qDebug()<<"Datagramma->:"<<Datagramma.toHex();
        if (crc0!=crc&& d_type==type_digital)
        {
            Data=Data.mid(i+1);
            i=Data.indexOf(0x55);
            qDebug()<<"WRONG CRC:"<<"DTYPE:"<<(int)d_type<<"crc0 :"<<::QString().number(crc0)<<"SRC :"<<::QString().number(crc);;
            continue;
        }

        if(d_type==type_digital&&Datagramma.length()>=len_digital) {
            setOvershort_1(!(Datagramma.at(2)&1));
            setOvershort_2(!(Datagramma.at(2)&2));

            setTemperature(bytes2double(Datagramma.at(4), Datagramma.at(3))*0.065);  //коэффициент для цифрового датчика

            setAngle1(-angle1k()+qRadiansToDegrees(qAsin( ( (double)((unsigned char)Datagramma.at(6)+(unsigned char)Datagramma.at(5)*256-2048)/512))));
            setAngle2(-angle2k()+qRadiansToDegrees(qAsin( ( (double)((unsigned char)Datagramma.at(8)+(unsigned char)Datagramma.at(7)*256-2048)/512))));

            m_count+=1;

        }
        if(d_type==type_analog&&Datagramma.length()>=len_analog) {


            setVoltage(voltagek()*bytes2double(Datagramma.at(3), Datagramma.at(2)));


            setCurrent1(current1k()*(bytes2double(Datagramma.at(5), Datagramma.at(4))));
            setCurrent2(current2k()*(bytes2double(Datagramma.at(7), Datagramma.at(6))));
            setCurrent3(current3k()*(bytes2double(Datagramma.at(9), Datagramma.at(8))));

            setPressure(pressurek()*(bytes2double(Datagramma.at(11), Datagramma.at(10))));
            m_count+=1;

        }
        l=d_type==type_digital?len_digital:len_analog;
        Data=Data.mid(i+l, Data.length());
        i=Data.indexOf(0x55);
    }//end while
    qDebug()<<"truncated:"<<Data.toHex();


}

void tk15::fill_list()
{
    m_count+=1;
    emit countChanged();
    qDebug()<<"c:"<<m_count<<";c1:"<<m_current1
                           <<";c2:"<<m_current2
                           <<";c3:"<<m_current3
                           <<";VV:"<<m_voltage
                           <<";a1:"<<m_angle1
                           <<";a2:"<<m_angle2
                           <<";t1:"<<m_temperature
                           <<";p1:"<<m_pressure;
    if (!m_tcp) setClient_connected(false);
}

double tk15::angle2k() const
{
    return m_angle2k;
}

void tk15::setAngle2k(double angle2k)
{
    if (m_angle2k == angle2k) return;
    m_angle2k = angle2k;
    emit angle2kChanged();
}

double tk15::angle1k() const
{
    return m_angle1k;

}

void tk15::setAngle1k(double angle1k)
{
    if( m_angle1k == angle1k) return;
    m_angle1k = angle1k;
    emit angle1kChanged();
}

double tk15::pressurek() const
{
    return m_pressurek;
}

void tk15::setPressurek(double pressurek)
{
    if(m_pressurek == pressurek) return;
    m_pressurek = pressurek;
    emit pressureChanged();
}

double tk15::voltagek() const
{
    return m_voltagek;
}

void tk15::setVoltagek(double voltagek)
{
    if(m_voltagek == voltagek) return;
    m_voltagek = voltagek;
    emit voltageChanged();
}

double tk15::current3k() const
{
    return m_current3k;
}

void tk15::setCurrent3k(double current3k)
{
    if(m_current3k == current3k) return;
    m_current3k = current3k;
}

double tk15::current2k() const
{
    return m_current2k;
}

void tk15::setCurrent2k(double current2k)
{
    if(m_current2k == current2k) return;
    m_current2k = current2k;
}

double tk15::current1k() const
{
    return m_current1k;
}

void tk15::setCurrent1k(double current1k)
{
    if(m_current1k == current1k) return;
    m_current1k = current1k;
}

bool tk15::overshort_2() const
{
    return m_overshort_2;
}

void tk15::setOvershort_2(bool overshort_2)
{
    if(m_overshort_2 == overshort_2) return;
    m_overshort_2 = overshort_2;
    emit overshort_2Changed();
}

bool tk15::overshort_1() const
{
    return m_overshort_1;
}

void tk15::setOvershort_1(bool overshort_1)
{
    if(m_overshort_1 == overshort_1) return;
    m_overshort_1 = overshort_1;
    emit overshort_1Changed();
}

double tk15::voltage() const
{
    return m_voltage;
}

void tk15::setVoltage(double voltage)
{
    if(m_voltage == voltage) return;
    m_voltage = voltage;
    emit voltageChanged();
}

double tk15::current3() const
{
    return m_current3;
}

void tk15::setCurrent3(double current3)
{
    if(m_current3 == current3) return;
    m_current3 = current3;
    emit current3Changed();
}

double tk15::current2() const
{

    return m_current2;
}

void tk15::setCurrent2(double current2)
{
    if(m_current2 == current2) return;
    m_current2 = current2;
    emit current2Changed();
}

double tk15::current1() const
{
    return m_current1;
}

void tk15::setCurrent1(double current1)
{
    if(m_current1 == current1) return;
    m_current1 = current1;
    emit current1Changed();
}

double tk15::angle2() const
{
    return m_angle2;
}

void tk15::setAngle2(double angle2)
{
    if(m_angle2 == angle2) return;
    m_angle2 = angle2;
    emit angle2Changed();
}

double tk15::angle1() const
{
    return m_angle1;
}

void tk15::setAngle1(double angle1)
{
    if(m_angle1 == angle1) return;
    m_angle1 = angle1;
    emit angle1Changed();
}

double tk15::pressure() const
{
    return m_pressure;

}

void tk15::setPressure(double pressure)
{
    if(m_pressure == pressure) return;
    m_pressure = pressure;
    emit pressureChanged();
}

double tk15::temperature() const
{
    return m_temperature;
}

void tk15::setTemperature(double temperature)
{
    if( m_temperature == temperature) return;
    m_temperature = temperature;
    emit temperatureChanged();
}


uint16_t tk15::CRC16(QByteArray data) {
    uint16_t crc = 0xFFFF;

        for(int pos = 0; pos<data.length();pos++)
        {
            crc ^= (uint16_t)data.data()[pos];
            for( int i = 8; i != 0; i--)
            {
                if ((crc & 0x0001) != 0) {      // LSB is set
                    crc >>= 1;                  // Shift right
                    crc ^= 0xA001;              // XOR 0xA001
                }
                else                            // LSB is not set
                    crc >>= 1;
            }

        }
        return crc;
}

double tk15::bytes2double(const unsigned char bst, const unsigned char bml)
{
    return (double)bst*256+bml;
}

uint8_t tk15::CRC8(QByteArray data)
//calculating checksum according to Dallas/Maxim Application Note 27
//(polynomial X^8+X^5+X^4+X^0), that is as used by 1-wire protocol.
{
    uint8_t crc8=0;
    uint16_t Len=data.length();
    //qDebug()<<"CRC Data:"<<data.toHex();
    try {
    for (uint16_t i=0; i<Len; i++)
    {
      crc8=crc8_tabl[crc8 ^ (uint8_t)data[i]];
      //qDebug()<<QString::number ((uchar) data[i], 16)<<"=:"<<QString::number ((uchar) crc8, 16);

    }
    }
    catch (...) {
        qDebug()<<"CRC calc error Len:"<<Len;
    }

    return crc8;
}

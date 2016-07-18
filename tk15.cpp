#include "tk15.h"
#include <QUdpSocket>
#include <QSettings>

tk15::tk15()
{
    readSettings();
    m_client = new QUdpSocket();
    m_client->bind(QHostAddress(address()), port());
    connect(m_client, SIGNAL(readyRead()), SLOT(onClientReadyRead()));
}

tk15::~tk15()
{
    delete m_client;
    saveSettings();
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

void tk15::onClientReadyRead()
{
    QHostAddress sender;
    quint16 senderPort;
    QByteArray datagram;
    datagram.resize(m_client->pendingDatagramSize());
    m_client->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    setData(datagram);
    m_current.append("one"); m_current.append("two"); emit currentChanged();
}

void tk15::saveSettings()
{
    QSettings settings("HYCO", "TK15");
    settings.setValue("Address",m_address);
    settings.setValue("Port",m_port);
}

void tk15::readSettings()
{
    QSettings settings("HYCO", "TK15");
    setAddress(settings.value("Address","192.168.1.168").toString());
    setPort(settings.value("Port", 9999).toInt());
}

QStringList tk15::current() const
{
    return m_current;
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


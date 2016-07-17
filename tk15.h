#ifndef TK15_H
#define TK15_H

#include <QQuickItem>
#include "qstring.h"
#include <QUdpSocket>

class tk15 : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString data READ data WRITE setData NOTIFY dataChanged)
public:
    tk15();
    ~tk15();


    QString address() const;
    void setAddress(const QString &address);

    int port() const;
    void setPort(int port);

    QString data() const;
    void setData(const QString &data);

signals:
    void addressChanged();
    void portChanged();
    void dataChanged();

public slots:
    void onClientReadyRead();
private:
    QString m_address="localhost";
    int m_port=9999;
    QUdpSocket *m_client;
    QString m_data="null";
    void saveSettings();
    void readSettings();
};

#endif // TK15_H

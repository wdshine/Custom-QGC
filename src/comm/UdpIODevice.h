/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

//  UdpIODevice类：
//  扩展QUdpSocket的功能，使其在绑定状态下支持逐行读取；
//  通过缓冲区处理数据包，解决UDP消息边界问题；
//  提升与QGroundControl中其他基于QIODevice接口组件的兼容性

 #pragma once

#include <QUdpSocket>

/**
 * @brief QUdpSocket implementation of canReadLine() readLineData() in server mode.
 * The UdpIODevice class works almost exactly as a QUdpSocket, but
 * also implements canReadLine() and readLineData() while in the bound state.
 * Regular QUdpSocket only allows to use these QIODevice interfaces when using
 * connectToHost(), which means it is working as a client instead of server.
 *
 **/

class UdpIODevice: public QUdpSocket
{
    Q_OBJECT
public:
    UdpIODevice(QObject *parent = nullptr);
    bool     canReadLine() const;
    qint64   readLineData(char *data, qint64 maxSize);

private slots:
    void _readAvailableData();

private:
    QByteArray _buffer;
};

/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

//  ADS-B：Automatic Dependent Surveillance - Broadcast，广播式自动相关监视
//  1. 空中交通显示
//  - 在飞行地图上显示附近 ADS-B 飞机（商业航班/其他无人机）
//  - 通过 QmlObjectListModel 暴露给 UI 层
//  2. 防撞预警
//  - 提供实时交通信息辅助避障决策
//  3. 态势感知增强
//  - 显示飞机呼号/高度/速度等关键信息
//  该文件与 ADSBVehicle.cpp 共同实现完整的 ADS-B 交通显示功能
//  通过 TCP 连接接收 GDL90 格式数据流，解析后在地图上可视化展示

#pragma once

#include "QGCToolbox.h"
#include "QmlObjectListModel.h"
#include "ADSBVehicle.h"

#include <QThread>
#include <QTcpSocket>
#include <QTimer>
#include <QGeoCoordinate>

class ADSBVehicleManagerSettings;

class ADSBTCPLink : public QThread
{
    Q_OBJECT

public:
    ADSBTCPLink(const QString& hostAddress, int port, QObject* parent);
    ~ADSBTCPLink();

signals:
    void adsbVehicleUpdate(const ADSBVehicle::ADSBVehicleInfo_t vehicleInfo);
    void error(const QString errorMsg);

protected:
    void run(void) final;

private slots:
    void _readBytes(void);

private:
    void _hardwareConnect(void);
    void _parseLine(const QString& line);

    QString         _hostAddress;
    int             _port;
    QTcpSocket*     _socket =   nullptr;
    void _parseAndEmitCallsign(ADSBVehicle::ADSBVehicleInfo_t &adsbInfo, QStringList values);
    void _parseAndEmitLocation(ADSBVehicle::ADSBVehicleInfo_t &adsbInfo, QStringList values);
    void _parseAndEmitHeading(ADSBVehicle::ADSBVehicleInfo_t &adsbInfo, QStringList values);
};

class ADSBVehicleManager : public QGCTool {
    Q_OBJECT
    
public:
    ADSBVehicleManager(QGCApplication* app, QGCToolbox* toolbox);

    Q_PROPERTY(QmlObjectListModel* adsbVehicles READ adsbVehicles CONSTANT)

    QmlObjectListModel* adsbVehicles(void) { return &_adsbVehicles; }

    // QGCTool overrides
    void setToolbox(QGCToolbox* toolbox) final;

public slots:
    void adsbVehicleUpdate  (const ADSBVehicle::ADSBVehicleInfo_t vehicleInfo);
    void _tcpError          (const QString errorMsg);

private slots:
    void _cleanupStaleVehicles(void);

private:
    QmlObjectListModel              _adsbVehicles;
    QMap<uint32_t, ADSBVehicle*>    _adsbICAOMap;
    QTimer                          _adsbVehicleCleanupTimer;
    ADSBTCPLink*                    _tcpLink = nullptr;
};

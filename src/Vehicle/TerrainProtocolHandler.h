/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "Vehicle.h"
#include "QGCMAVLink.h"
#include "QGCLoggingCategory.h"

#include <QObject>
#include <QGeoCoordinate>

class TerrainFactGroup;

Q_DECLARE_LOGGING_CATEGORY(TerrainProtocolHandlerLog)

// TerrainProtocolHandler类（地形协议处理器）：
// 1. MAVLink协议处理
//    - 处理地形相关MAVLink消息（TERRAIN_REQUEST/TERRAIN_REPORT）
//    - 通过 mavlinkMessageReceived 方法过滤处理地形协议消息
// 2. 地形数据发送控制
//    - 实现地形数据分块传输机制（网格位编码处理）
//    - 管理当前地形请求状态（_currentTerrainRequest结构体）
// 3. 与飞行器协同工作
//    - 关联Vehicle对象实现飞行器通信
//    - 集成TerrainFactGroup更新地形加载进度（blocksPending/blocksLoaded）
// 4. 请求响应机制
//    - 解析地形请求的西南角坐标和网格精度
//    - 支持地形数据的高效分块传输
class TerrainProtocolHandler : public QObject
{
    Q_OBJECT

public:
    explicit TerrainProtocolHandler(Vehicle* vehicle, TerrainFactGroup* terrainFactGroup, QObject *parent = nullptr);

    /// @return true: Allow vehicle to continue processing, false: Vehicle should not process message
    bool mavlinkMessageReceived(const mavlink_message_t message);

private slots:
    void _sendNextTerrainData(void);

private:
    void _handleTerrainRequest  (const mavlink_message_t& message);
    void _handleTerrainReport   (const mavlink_message_t& message);
    void _sendTerrainData       (const QGeoCoordinate& swCorner, uint8_t gridBit);

    Vehicle*                    _vehicle;
    TerrainFactGroup*           _terrainFactGroup;
    bool                        _terrainRequestActive =             false;
    mavlink_terrain_request_t   _currentTerrainRequest;
    QTimer                      _terrainDataSendTimer;
};

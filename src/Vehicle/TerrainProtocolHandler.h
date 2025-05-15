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

// TerrainProtocolHandler�ࣨ����Э�鴦��������
// 1. MAVLinkЭ�鴦��
//    - ����������MAVLink��Ϣ��TERRAIN_REQUEST/TERRAIN_REPORT��
//    - ͨ�� mavlinkMessageReceived �������˴������Э����Ϣ
// 2. �������ݷ��Ϳ���
//    - ʵ�ֵ������ݷֿ鴫����ƣ�����λ���봦��
//    - ����ǰ��������״̬��_currentTerrainRequest�ṹ�壩
// 3. �������Эͬ����
//    - ����Vehicle����ʵ�ַ�����ͨ��
//    - ����TerrainFactGroup���µ��μ��ؽ��ȣ�blocksPending/blocksLoaded��
// 4. ������Ӧ����
//    - ����������������Ͻ���������񾫶�
//    - ֧�ֵ������ݵĸ�Ч�ֿ鴫��
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

/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "QGCMAVLink.h"
#include "QmlObjectListModel.h"

#include <QObject>

/// Class which represents sensor info from the SYS_STATUS mavlink message
// 解析MAVLink的SYS_STATUS消息，管理传感器状态，并通过属性暴露给QML界面
class SysStatusSensorInfo : public QObject
{
    Q_OBJECT

public:
    SysStatusSensorInfo(QObject* parent = nullptr);

    Q_PROPERTY(QStringList sensorNames  READ sensorNames    NOTIFY sensorInfoChanged)
    Q_PROPERTY(QStringList sensorStatus READ sensorStatus   NOTIFY sensorInfoChanged)

    void        update      (const mavlink_sys_status_t& sysStatus);
    QStringList sensorNames (void) const;
    QStringList sensorStatus(void) const;

signals:
    void sensorInfoChanged(void);

private:
    typedef struct {
        bool enabled;
        bool healthy;
    } SensorInfo_t;

    QMap<MAV_SYS_STATUS_SENSOR, SensorInfo_t> _sensorInfoMap;
};

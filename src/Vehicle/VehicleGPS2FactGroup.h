/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "VehicleGPSFactGroup.h"
#include "QGCMAVLink.h"

// VehicleGPSFactGroup处理主GPS数据（MAVLINK_MSG_ID_GPS_RAW_INT）
// VehicleGPS2FactGroup处理第二GPS数据（MAVLINK_MSG_ID_GPS2_RAW）
class VehicleGPS2FactGroup : public VehicleGPSFactGroup
{
    Q_OBJECT

public:
    VehicleGPS2FactGroup(QObject* parent = nullptr);

    // Overrides from VehicleGPSFactGroup
    void handleMessage(Vehicle* vehicle, mavlink_message_t& message) override;

private:
    void _handleGps2Raw(mavlink_message_t& message);
};

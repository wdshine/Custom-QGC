/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "FactGroup.h"
#include "QGCMAVLink.h"

class Vehicle;

// 聚合和管理与时间相关的参数，通过Fact系统将这些参数暴露给QML界面，支持实时更新时间信息
class VehicleClockFactGroup : public FactGroup
{
    Q_OBJECT

public:
    VehicleClockFactGroup(QObject* parent = nullptr);

    Q_PROPERTY(Fact* currentTime READ currentTime CONSTANT)
    Q_PROPERTY(Fact* currentUTCTime READ currentUTCTime CONSTANT)
    Q_PROPERTY(Fact* currentDate READ currentDate CONSTANT)

    Fact* currentTime () { return &_currentTimeFact; }
    Fact* currentUTCTime () { return &_currentUTCTimeFact; }
    Fact* currentDate () { return &_currentDateFact; }

    static const char* _currentTimeFactName;
    static const char* _currentUTCTimeFactName;
    static const char* _currentDateFactName;

    static const char* _settingsGroup;

private slots:
    void _updateAllValues() override;

private:
    Fact            _currentTimeFact;
    Fact            _currentUTCTimeFact;
    Fact            _currentDateFact;
};

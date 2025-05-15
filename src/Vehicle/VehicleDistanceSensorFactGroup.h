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

// VehicleDistanceSensorFactGroup类：
// 1.多方向传感器参数管理
// - 继承自 FactGroup 实现参数组化管理
// - 管理12个关键参数：
//     - 8个偏航角度参数（Yaw 45°~315°）
//     - 2个俯仰角度参数（Pitch 90°/270°）
//     - 最小/最大有效距离参数
// 2.MAVLink协议集成
// - 处理距离传感器相关的MAVLink消息
// - 自动解析传感器方向参数和测距数据
// 3.参数验证系统
// - 确保传感器数据在物理限制范围内
// - 提供参数边界值校验机制
// 4.QML界面绑定
// - 通过 rotationYaw45 等属性暴露方向参数
// - 支持在QML界面动态配置传感器安装方向
// - 实时显示各方向测距数据
class VehicleDistanceSensorFactGroup : public FactGroup
{
    Q_OBJECT

public:
    VehicleDistanceSensorFactGroup(QObject* parent = nullptr);

    Q_PROPERTY(Fact* rotationNone       READ rotationNone       CONSTANT)
    Q_PROPERTY(Fact* rotationYaw45      READ rotationYaw45      CONSTANT)
    Q_PROPERTY(Fact* rotationYaw90      READ rotationYaw90      CONSTANT)
    Q_PROPERTY(Fact* rotationYaw135     READ rotationYaw135     CONSTANT)
    Q_PROPERTY(Fact* rotationYaw180     READ rotationYaw180     CONSTANT)
    Q_PROPERTY(Fact* rotationYaw225     READ rotationYaw225     CONSTANT)
    Q_PROPERTY(Fact* rotationYaw270     READ rotationYaw270     CONSTANT)
    Q_PROPERTY(Fact* rotationYaw315     READ rotationYaw315     CONSTANT)
    Q_PROPERTY(Fact* rotationPitch90    READ rotationPitch90    CONSTANT)
    Q_PROPERTY(Fact* rotationPitch270   READ rotationPitch270   CONSTANT)
    Q_PROPERTY(Fact* minDistance        READ minDistance        CONSTANT)
    Q_PROPERTY(Fact* maxDistance        READ maxDistance        CONSTANT)

    Fact* rotationNone      () { return &_rotationNoneFact; }
    Fact* rotationYaw45     () { return &_rotationYaw45Fact; }
    Fact* rotationYaw90     () { return &_rotationYaw90Fact; }
    Fact* rotationYaw135    () { return &_rotationYaw135Fact; }
    Fact* rotationYaw180    () { return &_rotationYaw180Fact; }
    Fact* rotationYaw225    () { return &_rotationYaw225Fact; }
    Fact* rotationYaw270    () { return &_rotationYaw270Fact; }
    Fact* rotationYaw315    () { return &_rotationYaw315Fact; }
    Fact* rotationPitch90   () { return &_rotationPitch90Fact; }
    Fact* rotationPitch270  () { return &_rotationPitch270Fact; }
    Fact* minDistance       () { return &_minDistanceFact; }
    Fact* maxDistance       () { return &_maxDistanceFact; }

    // Overrides from FactGroup
    void handleMessage(Vehicle* vehicle, mavlink_message_t& message) override;

    static const char* _rotationNoneFactName;
    static const char* _rotationYaw45FactName;
    static const char* _rotationYaw90FactName;
    static const char* _rotationYaw135FactName;
    static const char* _rotationYaw180FactName;
    static const char* _rotationYaw225FactName;
    static const char* _rotationYaw270FactName;
    static const char* _rotationYaw315FactName;
    static const char* _rotationPitch90FactName;
    static const char* _rotationPitch270FactName;
    static const char* _minDistanceFactName;
    static const char* _maxDistanceFactName;

private:
    Fact _rotationNoneFact;
    Fact _rotationYaw45Fact;
    Fact _rotationYaw90Fact;
    Fact _rotationYaw135Fact;
    Fact _rotationYaw180Fact;
    Fact _rotationYaw225Fact;
    Fact _rotationYaw270Fact;
    Fact _rotationYaw315Fact;
    Fact _rotationPitch90Fact;
    Fact _rotationPitch270Fact;
    Fact _minDistanceFact;
    Fact _maxDistanceFact;
};

/****************************************************************************
 *
 * (c) 2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "CompInfo.h"

#include <QObject>

class FactMetaData;
class Vehicle;
class FirmwarePlugin;

// 解析和执行器相关的JSON元数据文件，确保执行器组件的配置信息正确加载
// 支持QGroundControl对执行器的动态配置和管理
class CompInfoActuators : public CompInfo
{
    Q_OBJECT

public:
    CompInfoActuators(uint8_t compId, Vehicle* vehicle, QObject* parent = nullptr);

    // Overrides from CompInfo
    void setJson(const QString& metadataJsonFileName) override;

private:
};

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
// 事件处理系统的元数据管理核心，为飞控错误代码、状态事件等提供结构化元数据支持
// 通过JSON元数据实现不同固件版本事件系统的兼容性适配
class CompInfoEvents : public CompInfo
{
    Q_OBJECT

public:
    CompInfoEvents(uint8_t compId, Vehicle* vehicle, QObject* parent = nullptr);

    // Overrides from CompInfo
    void setJson(const QString& metadataJsonFileName) override;

private:
};

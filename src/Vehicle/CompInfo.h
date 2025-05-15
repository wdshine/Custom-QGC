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
#include "QGCLoggingCategory.h"
#include "FactMetaData.h"

#include <QObject>

class FactMetaData;
class Vehicle;
class FirmwarePlugin;
class CompInfoGeneral;

/// Base class for all CompInfo types
// 1.元数据管理
// - 存储组件元数据URI（ uriMetaData() ）
// - 维护CRC校验值（ crcMetaData() ）
// - 支持元数据回退机制（ uriMetaDataFallback() ）
// 2.多语言支持
// - 管理翻译文件URI（ uriTranslation() ）
// - 支持备用翻译资源加载
// 3.抽象接口
// - 强制子类实现JSON元数据解析
class CompInfo : public QObject
{
    Q_OBJECT

public:
    CompInfo(COMP_METADATA_TYPE type, uint8_t compId, Vehicle* vehicle, QObject* parent = nullptr);

    const QString& uriMetaData() const { return _uris.uriMetaData; }
    const QString& uriMetaDataFallback() const { return _uris.uriMetaDataFallback; }
    const QString& uriTranslation() const { return _uris.uriTranslation; }

    uint32_t crcMetaData() const { return _uris.crcMetaData; }
    uint32_t crcMetaDataFallback() const { return _uris.crcMetaDataFallback; }
    bool crcMetaDataValid() const { return _uris.crcMetaDataValid; }
    bool crcMetaDataFallbackValid() const { return _uris.crcMetaDataFallbackValid; }

    void setUriMetaData(const QString& uri, uint32_t crc);

    virtual void setJson(const QString& metaDataJsonFileName) = 0;

    bool available() const { return !_uris.uriMetaData.isEmpty(); }

    const COMP_METADATA_TYPE  type;
    Vehicle* const      vehicle                = nullptr;
    const uint8_t       compId                 = MAV_COMP_ID_ALL;

private:
    friend class CompInfoGeneral;

    // Uris：
    // - 封装组件相关的URI资源路径
    // - 实现双校验机制（主/备元数据CRC校验）
    // - 通过 crcMetaDataValid 标志位验证数据完整性
    struct Uris {
        bool                crcMetaDataValid            = false;
        bool                crcMetaDataFallbackValid    = false;

        uint32_t            crcMetaData            = 0;
        uint32_t            crcMetaDataFallback    = 0;

        QString             uriMetaData;
        QString             uriMetaDataFallback;
        QString             uriTranslation;
        QString             uriTranslationFallback;
    };

    Uris _uris;
};

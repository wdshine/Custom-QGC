/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

//  QGCSerialPortInfo类——用于识别和管理硬件设备的扩展类：
//  1. 硬件设备识别 通过USB供应商ID/产品ID匹配特定硬件类型（如Pixhawk、SiK Radio等），核心逻辑可见：
//  2. 刷机支持判断 通过canFlash()方法判断设备是否支持固件升级：
//  3. Bootloader状态检测 通过isBootloader()判断设备是否处于bootloader模式：
//  4. 系统端口过滤 通过isSystemPort()排除非飞控设备（如GPS模块、系统串口）。
//  该类在固件升级流程（PX4FirmwareUpgradeThread）和设备连接（SerialLink）中起核心作用，
//  通过JSON配置文件 USBBoardInfo.json 维护硬件识别规则，支持动态扩展新设备类型。

#pragma once

#ifdef __android__
    #include "qserialportinfo.h"
#else
    #include <QSerialPortInfo>
#endif

#include "QGCLoggingCategory.h"

Q_DECLARE_LOGGING_CATEGORY(QGCSerialPortInfoLog)

/// QGC's version of Qt QSerialPortInfo. It provides additional information about board types
/// that QGC cares about.
class QGCSerialPortInfo : public QSerialPortInfo
{
public:
    typedef enum {
        BoardTypePixhawk,
        BoardTypeSiKRadio,
        BoardTypePX4Flow,
        BoardTypeOpenPilot,
        BoardTypeRTKGPS,
        BoardTypeUnknown
    } BoardType_t;

    QGCSerialPortInfo(void);
    QGCSerialPortInfo(const QSerialPort & port);

    /// Override of QSerialPortInfo::availablePorts
    static QList<QGCSerialPortInfo> availablePorts(void);

    bool getBoardInfo(BoardType_t& boardType, QString& name) const;

    /// @return true: we can flash this board type
    bool canFlash(void) const;

    /// @return true: Board is currently in bootloader
    bool isBootloader(void) const;

    /// @return true: Port is a system port and not an autopilot
    static bool isSystemPort(QSerialPortInfo* port);

private:
    typedef struct {
        const char* classString;
        BoardType_t boardType;
    } BoardClassString2BoardType_t;

    typedef struct {
        int         vendorId;
        int         productId;
        BoardType_t boardType;
        QString     name;
    } BoardInfo_t;

    typedef struct {
        QString     regExp;
        BoardType_t boardType;
        bool        androidOnly;
    } BoardRegExpFallback_t;

    static void _loadJsonData(void);
    static BoardType_t _boardClassStringToType(const QString& boardClass);
    static QString _boardTypeToString(BoardType_t boardType);

    static bool         _jsonLoaded;
    static const char*  _jsonFileTypeValue;
    static const char*  _jsonBoardInfoKey;
    static const char*  _jsonBoardDescriptionFallbackKey;
    static const char*  _jsonBoardManufacturerFallbackKey;
    static const char*  _jsonVendorIDKey;
    static const char*  _jsonProductIDKey;
    static const char*  _jsonBoardClassKey;
    static const char*  _jsonNameKey;
    static const char*  _jsonRegExpKey;
    static const char*  _jsonAndroidOnlyKey;

    static const BoardClassString2BoardType_t   _rgBoardClass2BoardType[BoardTypeUnknown];
    static QList<BoardInfo_t>                   _boardInfoList;
    static QList<BoardRegExpFallback_t>         _boardDescriptionFallbackList;
    static QList<BoardRegExpFallback_t>         _boardManufacturerFallbackList;
};


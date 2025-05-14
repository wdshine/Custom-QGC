/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

//  QGCSerialPortInfo�ࡪ������ʶ��͹���Ӳ���豸����չ�ࣺ
//  1. Ӳ���豸ʶ�� ͨ��USB��Ӧ��ID/��ƷIDƥ���ض�Ӳ�����ͣ���Pixhawk��SiK Radio�ȣ��������߼��ɼ���
//  2. ˢ��֧���ж� ͨ��canFlash()�����ж��豸�Ƿ�֧�ֹ̼�������
//  3. Bootloader״̬��� ͨ��isBootloader()�ж��豸�Ƿ���bootloaderģʽ��
//  4. ϵͳ�˿ڹ��� ͨ��isSystemPort()�ų��Ƿɿ��豸����GPSģ�顢ϵͳ���ڣ���
//  �����ڹ̼��������̣�PX4FirmwareUpgradeThread�����豸���ӣ�SerialLink������������ã�
//  ͨ��JSON�����ļ� USBBoardInfo.json ά��Ӳ��ʶ�����֧�ֶ�̬��չ���豸���͡�

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


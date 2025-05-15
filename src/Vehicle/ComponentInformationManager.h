/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "QGCLoggingCategory.h"
#include "QGCMAVLink.h"
#include "StateMachine.h"
#include "ComponentInformationCache.h"
#include "ComponentInformationTranslation.h"

#include <QElapsedTimer>

Q_DECLARE_LOGGING_CATEGORY(ComponentInformationManagerLog)

class Vehicle;
class ComponentInformationManager;
class CompInfo;
class CompInfoParam;
class CompInfoGeneral;

// 1.元数据下载流程控制
// - 实现MAVLink FTP/HTTP协议下载流程
// - 支持JSON元数据文件(_stateRequestMetaDataJson)和翻译文件(_stateRequestTranslationJson)下载
// 2.校验机制
// - 通过CRC校验确保文件完整性(_jsonMetadataCrcValid)
// - 实现元数据回退机制(_stateRequestMetaDataJsonFallback)
// 3.多协议支持
// - 同时支持MAVLink FTP(_uriIsMAVLinkFTP)和HTTP下载
// - 处理PX4参数服务器和自定义元数据源

class RequestMetaDataTypeStateMachine : public StateMachine
{
    Q_OBJECT

public:
    RequestMetaDataTypeStateMachine(ComponentInformationManager* compMgr);

    void        request     (CompInfo* compInfo);
    QString     typeToString(void);
    CompInfo*   compInfo    (void) { return _compInfo; }

    // Overrides from StateMachine
    int             stateCount      (void) const final;
    const StateFn*  rgStates        (void) const final;
    void            statesCompleted (void) const final;

private slots:
    void    _ftpDownloadComplete                (const QString& file, const QString& errorMsg);
    void    _ftpDownloadProgress                (float progress);
    void    _httpDownloadComplete               (QString remoteFile, QString localFile, QString errorMsg);
    QString _downloadCompleteJsonWorker         (const QString& jsonFileName);
    void _downloadAndTranslationComplete(QString translatedJsonTempFile, QString errorMsg);

private:
    static void _stateRequestCompInfo           (StateMachine* stateMachine);
    static void _stateRequestCompInfoDeprecated (StateMachine* stateMachine);
    static void _stateRequestMetaDataJson       (StateMachine* stateMachine);
    static void _stateRequestMetaDataJsonFallback(StateMachine* stateMachine);
    static void _stateRequestTranslationJson    (StateMachine* stateMachine);
    static void _stateRequestTranslate          (StateMachine* stateMachine);
    static void _stateRequestComplete           (StateMachine* stateMachine);
    static bool _uriIsMAVLinkFTP                (const QString& uri);

    void _requestFile(const QString& cacheFileTag, bool crcValid, const QString& uri, QString& outputFileName);

    ComponentInformationManager*    _compMgr                    = nullptr;
    CompInfo*                       _compInfo                   = nullptr;
    QString                         _jsonMetadataFileName;
    QString                         _jsonMetadataTranslatedFileName;
    bool                            _jsonMetadataCrcValid       = false;
    QString                         _jsonTranslationFileName;
    bool                            _jsonTranslationCrcValid    = false;

    QString*                        _currentFileName            = nullptr;
    QString                         _currentCacheFileTag;
    bool                            _currentFileValidCrc        = false;

    QElapsedTimer                   _downloadStartTime;

    static const StateFn  _rgStates[];
    static const int      _cStates;
};

// 1.全局元数据管理
// - 维护组件信息映射表(_compInfoMap)
// - 支持参数/通用/事件/执行器等组件类型(_stateRequestCompInfoParam等)
// 2.缓存管理
// - 集成ComponentInformationCache实现3天缓存策略(cachedFileMaxAgeSec)
// - 通过文件标签机制(_getFileCacheTag)管理不同组件类型的缓存
// 3.多语言支持
// - 使用ComponentInformationTranslation处理元数据本地化
// - 支持翻译文件与元数据文件的关联下载
// 4.进度监控
// - 通过progressUpdate信号反馈整体下载进度
// - 协调多个组件类型的并行请求

class ComponentInformationManager : public StateMachine
{
    Q_OBJECT

public:
    static constexpr int cachedFileMaxAgeSec = 3 * 24 * 3600; ///< 3 days

    ComponentInformationManager(Vehicle* vehicle);

    typedef void (*RequestAllCompleteFn)(void* requestAllCompleteFnData);

    void                requestAllComponentInformation  (RequestAllCompleteFn requestAllCompletFn, void * requestAllCompleteFnData);
    Vehicle*            vehicle                         (void) { return _vehicle; }
    CompInfoParam*      compInfoParam                   (uint8_t compId);
    CompInfoGeneral*    compInfoGeneral                 (uint8_t compId);

    // Overrides from StateMachine
    int             stateCount  (void) const final;
    const StateFn*  rgStates    (void) const final;

    ComponentInformationCache& fileCache() { return _fileCache; }
    ComponentInformationTranslation* translation() { return _translation; }

    float progress() const;

    void advance() override;

signals:
    void progressUpdate(float progress);

private:
    void _stateRequestCompInfoComplete  (void);
    bool _isCompTypeSupported           (COMP_METADATA_TYPE type);
    void _updateAllUri                  ();

    static QString _getFileCacheTag(int compInfoType, uint32_t crc, bool isTranslation);

    static void _stateRequestCompInfoGeneral        (StateMachine* stateMachine);
    static void _stateRequestCompInfoGeneralComplete(StateMachine* stateMachine);
    static void _stateRequestCompInfoParam          (StateMachine* stateMachine);
    static void _stateRequestCompInfoEvents         (StateMachine* stateMachine);
    static void _stateRequestCompInfoActuators      (StateMachine* stateMachine);
    static void _stateRequestAllCompInfoComplete    (StateMachine* stateMachine);

    Vehicle*                        _vehicle                    = nullptr;
    RequestMetaDataTypeStateMachine _requestTypeStateMachine;
    RequestAllCompleteFn            _requestAllCompleteFn       = nullptr;
    void*                           _requestAllCompleteFnData   = nullptr;
    QGCCachedFileDownload*          _cachedFileDownload         = nullptr;
    ComponentInformationCache&      _fileCache;
    ComponentInformationTranslation* _translation               = nullptr;

    QMap<uint8_t /* compId */, QMap<COMP_METADATA_TYPE, CompInfo*>> _compInfoMap;

    static const StateFn                  _rgStates[];
    static const int                      _cStates;

    friend class RequestMetaDataTypeStateMachine;
};

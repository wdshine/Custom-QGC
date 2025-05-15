/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


/// @file
///     @author Don Gagne <don@thegagnes.com>

#ifndef MultiVehicleManager_H
#define MultiVehicleManager_H

#include "Vehicle.h"
#include "QGCMAVLink.h"
#include "QmlObjectListModel.h"
#include "QGCToolbox.h"
#include "QGCLoggingCategory.h"

class FirmwarePluginManager;
class FollowMe;
class JoystickManager;
class QGCApplication;
class MAVLinkProtocol;

Q_DECLARE_LOGGING_CATEGORY(MultiVehicleManagerLog)

// MultiVehicleManager类：
// 1. 多机协同管理
//    - 维护动态无人机列表（ _vehicles 模型）
//    - 支持通过vehicleId快速检索（ getVehicleById 方法）
// 2. 焦点切换机制
//    - 管理当前活动无人机（ _activeVehicle 指针）
//    - 通过两阶段切换（ _setActiveVehiclePhase2 ）确保线程安全
// 3. 离线编辑支持
//    - 创建虚拟无人机实例用于参数调试
//    - 支持未连接物理设备时的配置操作
// 4. 心跳监测系统
//    - 通过MAVLink协议维持地面站-无人机通信
//    - 支持心跳开关控制（ gcsHeartBeatEnabled 属性）
// 5. 空间定位追踪
//    - 记录最后有效GPS坐标
//    - 通过 _coordinateChanged 信号更新地图显示
class MultiVehicleManager : public QGCTool
{
    Q_OBJECT

public:
    MultiVehicleManager(QGCApplication* app, QGCToolbox* toolbox);

    Q_INVOKABLE void        saveSetting (const QString &key, const QString& value);
    Q_INVOKABLE QString     loadSetting (const QString &key, const QString& defaultValue);

    Q_PROPERTY(bool                 activeVehicleAvailable          READ activeVehicleAvailable                                         NOTIFY activeVehicleAvailableChanged)
    Q_PROPERTY(bool                 parameterReadyVehicleAvailable  READ parameterReadyVehicleAvailable                                 NOTIFY parameterReadyVehicleAvailableChanged)
    Q_PROPERTY(Vehicle*             activeVehicle                   READ activeVehicle                  WRITE setActiveVehicle          NOTIFY activeVehicleChanged)
    Q_PROPERTY(QmlObjectListModel*  vehicles                        READ vehicles                                                       CONSTANT)
    Q_PROPERTY(bool                 gcsHeartBeatEnabled             READ gcsHeartbeatEnabled            WRITE setGcsHeartbeatEnabled    NOTIFY gcsHeartBeatEnabledChanged)
    Q_PROPERTY(Vehicle*             offlineEditingVehicle           READ offlineEditingVehicle                                          CONSTANT)
    Q_PROPERTY(QGeoCoordinate       lastKnownLocation               READ lastKnownLocation                                              NOTIFY lastKnownLocationChanged) //< Current vehicles last know location

    // Methods

    Q_INVOKABLE Vehicle* getVehicleById(int vehicleId);

    UAS* activeUas(void) { return _activeVehicle ? _activeVehicle->uas() : nullptr; }

    // Property accessors

    bool activeVehicleAvailable(void) const{ return _activeVehicleAvailable; }

    bool parameterReadyVehicleAvailable(void) const{ return _parameterReadyVehicleAvailable; }

    Vehicle* activeVehicle(void) { return _activeVehicle; }
    void setActiveVehicle(Vehicle* vehicle);

    QmlObjectListModel* vehicles(void) { return &_vehicles; }

    bool gcsHeartbeatEnabled(void) const { return _gcsHeartbeatEnabled; }
    void setGcsHeartbeatEnabled(bool gcsHeartBeatEnabled);

    Vehicle* offlineEditingVehicle(void) { return _offlineEditingVehicle; }

    // Override from QGCTool
    virtual void setToolbox(QGCToolbox *toolbox);

    QGeoCoordinate lastKnownLocation    () { return _lastKnownLocation; }

signals:
    void vehicleAdded                   (Vehicle* vehicle);
    void vehicleRemoved                 (Vehicle* vehicle);
    void activeVehicleAvailableChanged  (bool activeVehicleAvailable);
    void parameterReadyVehicleAvailableChanged(bool parameterReadyVehicleAvailable);
    void activeVehicleChanged           (Vehicle* activeVehicle);
    void gcsHeartBeatEnabledChanged     (bool gcsHeartBeatEnabled);
    void lastKnownLocationChanged       ();
#ifndef DOXYGEN_SKIP
    void _deleteVehiclePhase2Signal     (void);
#endif

private slots:
    void _deleteVehiclePhase1           (Vehicle* vehicle);
    void _deleteVehiclePhase2           (void);
    void _setActiveVehiclePhase2        (void);
    void _vehicleParametersReadyChanged (bool parametersReady);
    void _sendGCSHeartbeat              (void);
    void _vehicleHeartbeatInfo          (LinkInterface* link, int vehicleId, int componentId, int vehicleFirmwareType, int vehicleType);
    void _requestProtocolVersion        (unsigned version);
    void _coordinateChanged             (QGeoCoordinate coordinate);

private:
    bool _vehicleExists(int vehicleId);

    bool        _activeVehicleAvailable;            ///< true: An active vehicle is available
    bool        _parameterReadyVehicleAvailable;    ///< true: An active vehicle with ready parameters is available
    Vehicle*    _activeVehicle;                     ///< Currently active vehicle from a ui perspective
    Vehicle*    _offlineEditingVehicle;             ///< Disconnected vechicle used for offline editing

    QList<Vehicle*> _vehiclesBeingDeleted;          ///< List of Vehicles being deleted in queued phases
    Vehicle*        _vehicleBeingSetActive;         ///< Vehicle being set active in queued phases

    QList<int>  _ignoreVehicleIds;          ///< List of vehicle id for which we ignore further communication

    QmlObjectListModel  _vehicles;

    FirmwarePluginManager*      _firmwarePluginManager;
    JoystickManager*            _joystickManager;
    MAVLinkProtocol*            _mavlinkProtocol;
    QGeoCoordinate              _lastKnownLocation;

    QTimer              _gcsHeartbeatTimer;             ///< Timer to emit heartbeats
    bool                _gcsHeartbeatEnabled;           ///< Enabled/disable heartbeat emission
    static const int    _gcsHeartbeatRateMSecs = 1000;  ///< Heartbeat rate
    static const char*  _gcsHeartbeatEnabledKey;
};

#endif

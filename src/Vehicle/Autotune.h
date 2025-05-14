/****************************************************************************
 *
 * (c) 2009-2021 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QTimer>
#include "Vehicle.h"
#include "MAVLinkProtocol.h"

// Autotune类：
// 1. 自动调参流程控制
// - 实现PX4飞控的自动PID调参功能
// - 封装 MAV_CMD_DO_AUTOTUNE_ENABLE 指令交互 
// 2. 状态监控
// - 实时跟踪调参进度（0-100%）
// - 检测飞控是否支持自动调参功能 
// 3. MAVLink通信处理
// - 解析 MAVLINK_MSG_ID_COMMAND_ACK
// - 处理超时/失败等异常状态 
// 4. 安全机制
// - 调参前自动检查无人机状态
// - 防止在危险状态下启动调参

class Autotune : public QObject
{
    Q_OBJECT


public:
    explicit Autotune(Vehicle *vehicle);

    Q_PROPERTY(bool      autotuneEnabled      READ autotuneEnabled        NOTIFY autotuneChanged)
    Q_PROPERTY(bool      autotuneInProgress   READ autotuneInProgress     NOTIFY autotuneChanged)
    Q_PROPERTY(float     autotuneProgress     READ autotuneProgress       NOTIFY autotuneChanged)
    Q_PROPERTY(QString   autotuneStatus       READ autotuneStatus         NOTIFY autotuneChanged)

    Q_INVOKABLE void autotuneRequest ();

    static void ackHandler      (void* resultHandlerData,   int compId, const mavlink_command_ack_t& ack, Vehicle::MavCmdResultFailureCode_t failureCode);
    static void progressHandler (void* progressHandlerData, int compId, const mavlink_command_ack_t& ack);

    bool      autotuneEnabled    ();
    bool      autotuneInProgress () { return _autotuneInProgress; }
    float     autotuneProgress   () { return _autotuneProgress; }
    QString   autotuneStatus     () { return _autotuneStatus; }


public slots:
    void handleEnabled ();
    void sendMavlinkRequest();


private:
    void handleAckStatus(uint8_t ackProgress);
    void handleAckFailure();
    void handleAckError(uint8_t ackError);
    void startTimers();
    void stopTimers();


private slots:


private:
    Vehicle* _vehicle                {nullptr};
    bool     _autotuneInProgress     {false};
    float    _autotuneProgress       {0.0};
    QString  _autotuneStatus         {tr("Autotune: Not performed")};
    bool     _disarmMessageDisplayed {false};

    QTimer   _pollTimer;         // the frequency at which the polling should be performed


signals:
    void autotuneChanged ();
};

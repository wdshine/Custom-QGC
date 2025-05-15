/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "StateMachine.h"
#include "QGCMAVLink.h"
#include "QGCLoggingCategory.h"
#include "Vehicle.h"

Q_DECLARE_LOGGING_CATEGORY(InitialConnectStateMachineLog)

class Vehicle;

// 作为状态机，管理无人机连接初始化的各个步骤，包括获取版本信息、组件信息、参数、任务等
// 确保连接过程有序进行，并通过信号反馈进度。每个状态对应一个特定的初始化任务
// 状态转换由基类StateMachine管理，具体的状态处理函数执行实际的通信和数据处理
class InitialConnectStateMachine : public StateMachine
{
    Q_OBJECT

public:
    InitialConnectStateMachine(Vehicle* vehicle);

    // Overrides from StateMachine
    int             stateCount      (void) const final;
    const StateFn*  rgStates        (void) const final;
    void            statesCompleted (void) const final;

    void advance() override;

signals:
    void progressUpdate(float progress);

private slots:
    void gotProgressUpdate(float progressValue);
    void standardModesRequestCompleted();

private:
    static void _stateRequestAutopilotVersion           (StateMachine* stateMachine);
    static void _stateRequestProtocolVersion            (StateMachine* stateMachine);
    static void _stateRequestCompInfo                   (StateMachine* stateMachine);
    static void _stateRequestStandardModes              (StateMachine* stateMachine);
    static void _stateRequestCompInfoComplete           (void* requestAllCompleteFnData);
    static void _stateRequestParameters                 (StateMachine* stateMachine);
    static void _stateRequestMission                    (StateMachine* stateMachine);
    static void _stateRequestGeoFence                   (StateMachine* stateMachine);
    static void _stateRequestRallyPoints                (StateMachine* stateMachine);
    static void _stateSignalInitialConnectComplete      (StateMachine* stateMachine);

    static void _autopilotVersionRequestMessageHandler  (void* resultHandlerData, MAV_RESULT commandResult, Vehicle::RequestMessageResultHandlerFailureCode_t failureCode, const mavlink_message_t& message);
    static void _protocolVersionRequestMessageHandler   (void* resultHandlerData, MAV_RESULT commandResult, Vehicle::RequestMessageResultHandlerFailureCode_t failureCode, const mavlink_message_t& message);

    float _progress(float subProgress = 0.f);

    Vehicle* _vehicle;

    static const StateFn    _rgStates[];
    static const int        _rgProgressWeights[];
    static const int        _cStates;

    int _progressWeightTotal;
};

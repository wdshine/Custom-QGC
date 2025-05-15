/****************************************************************************
 *
 * (c) 2022 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QObject>

#include <QmlObjectListModel.h>
#include <libevents/libs/cpp/parse/health_and_arming_checks.h>

// HealthAndArmingCheckProblem类：
// 1. 问题详情存储
//    - 封装问题的消息/描述/严重等级三要素
//    - 通过 expanded 属性控制QML界面折叠状态
// 2. QML数据绑定
//    - 提供消息(message)/描述(description)/严重程度(severity)的只读属性
//    - 支持展开状态(expanded)的双向绑定
// 3. UI交互支持
//    - 触发 expandedChanged 信号更新界面

class HealthAndArmingCheckProblem : public QObject
{
    Q_OBJECT
public:
    HealthAndArmingCheckProblem(const QString& message, const QString& description, const QString& severity)
    : _message(message), _description(description), _severity(severity) {}

    Q_PROPERTY(QString message                            READ message                CONSTANT)
    Q_PROPERTY(QString description                        READ description            CONSTANT)
    Q_PROPERTY(QString severity                           READ severity               CONSTANT)
    Q_PROPERTY(bool expanded                              READ expanded               WRITE setExpanded NOTIFY expandedChanged)

    const QString& message() const { return _message; }
    const QString& description() const { return _description; }
    const QString& severity() const { return _severity; }

    bool expanded() const { return _expanded; }
    void setExpanded(bool expanded) { _expanded = expanded; emit expandedChanged(); }

signals:
    void expandedChanged();
private:
    const QString _message;
    const QString _description;
    const QString _severity;
    bool _expanded{false};
};

// HealthAndArmingCheckReport类：
// 1. 飞行状态决策
//    - 评估当前模式下的可解锁/起飞/执行任务状态
//    - 集成GPS状态显示( gpsState )
// 2. 问题集合管理
//    - 存储当前飞行模式相关的健康检查问题
//    - 自动过滤非当前模式的问题（通过 flightModeGroup ）
// 3. 事件系统集成
//    - 处理 events::HealthAndArmingChecks::Results 事件数据
//    - 动态生成问题列表并触发界面更新信号
class HealthAndArmingCheckReport : public QObject
{
    Q_OBJECT
public:

    Q_PROPERTY(bool supported                              READ supported               NOTIFY updated)
    Q_PROPERTY(bool canArm                                 READ canArm                  NOTIFY updated)
    Q_PROPERTY(bool canTakeoff                             READ canTakeoff              NOTIFY updated)
    Q_PROPERTY(bool canStartMission                        READ canStartMission         NOTIFY updated)
    Q_PROPERTY(bool hasWarningsOrErrors                    READ hasWarningsOrErrors     NOTIFY updated)
    Q_PROPERTY(QString gpsState                            READ gpsState                NOTIFY updated)
    Q_PROPERTY(QmlObjectListModel* problemsForCurrentMode  READ problemsForCurrentMode  NOTIFY updated)

    HealthAndArmingCheckReport();
    virtual ~HealthAndArmingCheckReport();

    bool supported() const { return _supported; }
    bool canArm() const { return _canArm; }
    bool canTakeoff() const { return _canTakeoff; }
    bool canStartMission() const { return _canStartMission; }
    bool hasWarningsOrErrors() const { return _hasWarningsOrErrors; }

    const QString& gpsState() const { return _gpsState; }

    QmlObjectListModel* problemsForCurrentMode() { return _problemsForCurrentMode; }

    void update(uint8_t compid, const events::HealthAndArmingChecks::Results& results, int flightModeGroup);

    void setModeGroups(int takeoffModeGroup, int missionModeGroup);

signals:
    void updated();

private:
    bool _supported{false};
    bool _canArm{true}; ///< whether arming is possible for the current mode
    bool _canTakeoff{true};
    bool _canStartMission{true};
    bool _hasWarningsOrErrors{false};
    QString _gpsState{};

    int _takeoffModeGroup{-1};
    int _missionModeGroup{-1};

    QmlObjectListModel* _problemsForCurrentMode = new QmlObjectListModel(this); ///< list of HealthAndArmingCheckProblem*
};


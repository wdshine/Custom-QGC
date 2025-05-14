/****************************************************************************
 *
 * (c) 2021 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

 // 多个类协同实现无人机执行器参数的可视化配置，支持动态界面生成、参数校验和三维布局预览功能

#pragma once


#include <QString>
#include <QRegularExpressionMatch>
#include <QVector3D>
#include <QJsonValue>

#include <stdint.h>

#include "ParameterManager.h"

Q_DECLARE_LOGGING_CATEGORY(ActuatorsConfigLog)


/**
 * Represents a per-channel or per-item vehicle configuration parameter
 */
// 定义执行器配置参数的元数据，支持位操作、浮点转布尔等特殊显示方式，用于动态生成参数配置界面
struct Parameter {
    enum class DisplayOption {
        Default,
        BoolTrueIfPositive, ///< Show checkbox for float/int value
        Bitset,             ///< integer displayed as boolean (checkbox), where the index defines the bit
    };

    QString label{};
    QString name{};                                       ///< vehicle parameter name, this may have an index in the form '${i}'
    int indexOffset{};                                    ///< extra offset to the ${i} index, or bitset shift offset
    DisplayOption displayOption{DisplayOption::Default};
    bool advanced{false};                                 ///< whether this should only be shown as advanced option

    void parse(const QJsonValue& jsonValue);
};

/**
 * Fact to show a specific bit in a bitset (integer fact) as boolean option
 */
class FactBitset : public Fact
{
    Q_OBJECT
public:
    FactBitset(QObject* parent, Fact* integerFact, int offset);

    virtual ~FactBitset() = default;

private slots:
    void onIntegerFactChanged();
    void onThisFactChanged();
private:
    Fact* _integerFact{nullptr};
    const int _offset;
    bool _ignoreChange{false};
};

/**
 * Fact to show a float fact as boolean, true if >0, false otherwise
 */
class FactFloatAsBool : public Fact
{
    Q_OBJECT
public:
    FactFloatAsBool(QObject* parent, Fact* floatFact);

    virtual ~FactFloatAsBool() = default;

private slots:
    void onFloatFactChanged();
    void onThisFactChanged();
private:
    Fact* _floatFact{nullptr};
    bool _ignoreChange{false};
};

/**
 * Evaluates a string expression containing a vehicle parameter name and comparison to a constant value
 */
class Condition
{
public:
    enum class Operation {
        AlwaysTrue,
        AlwaysFalse,
        GreaterThan,  // >
        GreaterEqual, // >=
        Equal,        // ==
        NotEqual,     // !=
        LessThan,     // <
        LessEqual     // <=
    };

    Condition() = default;

    /**
     * Constructor
     * @param condition generic form: true|false|<param_name><operation><signed integer>
     *                  where: <operation>: [>,>=,==,!=,<,<=]
     */
    Condition(const QString& condition, ParameterManager* parameterManager);

    bool evaluate() const;

    bool hasCondition() const { return _operation != Operation::AlwaysTrue; }

    Fact* fact() const { return _fact; }

private:
    QString _parameter{};
    Operation _operation{Operation::AlwaysTrue};
    int32_t _value{0};
    Fact* _fact{nullptr};
};


/**
 * Actuator used for rendering
 */
// 存储执行器（电机、舵机）的几何布局信息，用于可视化引擎渲染无人机结构
struct ActuatorGeometry
{
    enum class Type {
        Motor,
        Servo,
        Other
    };

    enum class SpinDirection {
        Unspecified,
        ClockWise,
        CounterClockWise,
    };

    static Type typeFromStr(const QString& type);

    struct RenderOptions {
        bool highlight{false};
    };

    ActuatorGeometry(Type type_=Type::Other, int index_=0, QVector3D position_={},
            SpinDirection spinDirection_=SpinDirection::Unspecified)
    : type(type_), index(index_), position(position_), spinDirection(spinDirection_) { }

    Type type;
    int index;
    int labelIndexOffset{0};
    QVector3D position;
    SpinDirection spinDirection;

    RenderOptions renderOptions{};
};


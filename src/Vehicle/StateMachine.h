/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QObject>

// 通用的状态机框架，便于子类实现具体状态流转逻辑，封装了状态管理的基本机制，如状态切换、状态函数调用等
class StateMachine : public QObject
{
    Q_OBJECT

public:
    typedef void (*StateFn)(StateMachine* stateMachine);

    StateMachine(void);

    /// Start the state machine with the first step
    void start(void);

    /// Advance the state machine to the next state and call the state function
    virtual void advance(void);

    /// Move the state machine to the specified state and call the state function
    void move(StateFn stateFn);

    StateFn currentState(void) const;

    /// @return The number of states in the rgStates array
    virtual int stateCount(void) const = 0;

    /// @return Array of states to execute
    virtual const StateFn* rgStates(void) const = 0;

    /// Called when all states have completed
    virtual void statesCompleted(void) const;

    bool active() const { return _active; }

protected:
    bool    _active = false;
    int     _stateIndex  = -1;
};

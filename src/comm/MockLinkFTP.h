/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

//  1. 协议模拟
//  - 实现MAVLink FTP服务端逻辑，处理文件列表请求、文件读取、会话管理等操作
//  - 支持标准FTP操作码（kCmdListDirectory/kCmdOpenFileRO/kCmdReadFile等）
//  - 通过 `MockLinkFTP.cc` 中的处理逻辑模拟真实设备行为
// 2. 测试支持
//  - 支持多种错误模式（errModeNoResponse/errModeNakResponse等），用于验证客户端的错误恢复能力
//  - 在 `FileManagerTest.cc` 中用于测试文件列表获取、断点续传等场景
//  - 通过设置随机丢包(_randomDropsEnabled)测试网络异常情况
// 3. 数据管理
//  - 维护虚拟文件系统(_fileList)，支持动态创建测试文件(_createTestTempFile)
//  - 处理文件会话状态(_sessionId)，管理文件读写指针位置(_currentFile)
//  - 支持特殊文件路径处理（如@PARAM/param.pck二进制参数文件）
// 4. 消息处理
//  - 在 `mavlinkMessageReceived` 中解析MAVLink消息
//  - 生成符合协议规范的ACK/NAK响应(_sendResponse/_sendNak)

// 通过模拟真实设备的FTP协议，使得QGC的FTPManager、FileManager等模块可以在不依赖真实硬件的情况下进行单元测试。

#pragma once

#include "QGCMAVLink.h"

#include <QStringList>
#include <QFile>

class MockLink;

/// Mock implementation of Mavlink FTP server.
class MockLinkFTP : public QObject
{
    Q_OBJECT
    
public:
    MockLinkFTP(uint8_t systemIdServer, uint8_t componentIdServer, MockLink* mockLink);
    
    /// @brief Sets the list of files returned by the List command. Prepend names with F or D
    /// to indicate (F)ile or (D)irectory.
    void setFileList(QStringList& fileList) { _fileList = fileList; }
    
    /// @brief By calling setErrorMode with one of these modes you can cause the server to simulate an error.
    typedef enum {
        errModeNone,                ///< No error, respond correctly
        errModeNoResponse,          ///< No response to any request, client should eventually time out with no Ack
        errModeNakResponse,         ///< Nak all requests
        errModeNoSecondResponse,    ///< No response to subsequent request to initial command
        errModeNakSecondResponse,   ///< Nak subsequent request to initial command
        errModeBadSequence          ///< Return response with bad sequence number
    } ErrorMode_t;
    
    /// @brief Sets the error mode for command responses. This allows you to simulate various server errors.
    void setErrorMode(ErrorMode_t errMode) { _errMode = errMode; };
    
    /// @brief Array of failure modes you can cycle through for testing. By looping through this array you can avoid
    /// hardcoding the specific error modes in your unit test. This way when new error modes are added your unit test
    /// code may not need to be modified.
    static const ErrorMode_t rgFailureModes[];
    
    /// @brief The number of ErrorModes in the rgFailureModes array.
    static const size_t cFailureModes;
    
    /// Called to handle an FTP message
    void mavlinkMessageReceived(const mavlink_message_t& message);

    void enableRandromDrops(bool enable) { _randomDropsEnabled = enable; }
    void enableBinParamFile(bool enable) { _BinParamFileEnabled = enable; }

    static const char* sizeFilenamePrefix;

signals:
    /// You can connect to this signal to be notified when the server receives a Terminate command.
    void terminateCommandReceived(void);
    
    /// You can connect to this signal to be notified when the server receives a Reset command.
    void resetCommandReceived(void);
    
private:
    void        _sendAck                (uint8_t targetSystemId, uint8_t targetComponentId, uint16_t seqNumber, MavlinkFTP::OpCode_t reqOpCode);
    void        _sendNak                (uint8_t targetSystemId, uint8_t targetComponentId, MavlinkFTP::ErrorCode_t error, uint16_t seqNumber, MavlinkFTP::OpCode_t reqOpCode);
    void        _sendNakErrno           (uint8_t targetSystemId, uint8_t targetComponentId, uint8_t nakErrno, uint16_t seqNumber, MavlinkFTP::OpCode_t reqOpCode);
    void        _sendResponse           (uint8_t targetSystemId, uint8_t targetComponentId, MavlinkFTP::Request* request, uint16_t seqNumber);
    void        _listCommand            (uint8_t senderSystemId, uint8_t senderComponentId, MavlinkFTP::Request* request, uint16_t seqNumber);
    void        _openCommand            (uint8_t senderSystemId, uint8_t senderComponentId, MavlinkFTP::Request* request, uint16_t seqNumber);
    void        _readCommand            (uint8_t senderSystemId, uint8_t senderComponentId, MavlinkFTP::Request* request, uint16_t seqNumber);
    void        _burstReadCommand          (uint8_t senderSystemId, uint8_t senderComponentId, MavlinkFTP::Request* request, uint16_t seqNumber);
    void        _terminateCommand       (uint8_t senderSystemId, uint8_t senderComponentId, MavlinkFTP::Request* request, uint16_t seqNumber);
    void        _resetCommand           (uint8_t senderSystemId, uint8_t senderComponentId, uint16_t seqNumber);
    uint16_t    _nextSeqNumber          (uint16_t seqNumber);
    QString     _createTestTempFile     (int size);
    
    /// if request is a string, this ensures it's null-terminated
    static void ensureNullTemination(MavlinkFTP::Request* request);

    QStringList _fileList;  ///< List of files returned by List command
    
    QFile                   _currentFile;
    ErrorMode_t             _errMode            = errModeNone;  ///< Currently set error mode, as specified by setErrorMode
    const uint8_t           _systemIdServer;                    ///< System ID for server
    const uint8_t           _componentIdServer;                 ///< Component ID for server
    MockLink*               _mockLink;                          ///< MockLink to communicate through
    bool                    _lastReplyValid     = false;
    uint16_t                _lastReplySequence  = 0;
    mavlink_message_t       _lastReply;
    bool                    _randomDropsEnabled = false;
    bool                    _BinParamFileEnabled = false;

    static const uint8_t    _sessionId          = 1;    ///< We only support a single fixed session
};


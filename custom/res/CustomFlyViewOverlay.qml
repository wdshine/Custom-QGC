/****************************************************************************
 *
 * (c) 2009-2019 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 * @file
 *   @author Gus Grubba <gus@auterion.com>
 */

import QtQuick          2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts  1.11

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.Palette       1.0
import QGroundControl.ScreenTools   1.0

import Custom.Widgets 1.0

Item {
    property var parentToolInsets                       // These insets tell you what screen real estate is available for positioning the controls in your overlay
    property var totalToolInsets:   _totalToolInsets    // The insets updated for the custom overlay additions
    property var mapControl

    readonly property string noGPS:         qsTr("NO GPS")
    readonly property real   indicatorValueWidth:   ScreenTools.defaultFontPixelWidth * 7

    property var    _activeVehicle:         QGroundControl.multiVehicleManager.activeVehicle
    property real   _indicatorDiameter:     ScreenTools.defaultFontPixelWidth * 18
    property real   _indicatorsHeight:      ScreenTools.defaultFontPixelHeight
    property var    _sepColor:              qgcPal.globalTheme === QGCPalette.Light ? Qt.rgba(0,0,0,0.5) : Qt.rgba(1,1,1,0.5)
    property color  _indicatorsColor:       qgcPal.text
    property bool   _isVehicleGps:          _activeVehicle ? _activeVehicle.gps.count.rawValue > 1 && _activeVehicle.gps.hdop.rawValue < 1.4 : false
    property string _altitude:              _activeVehicle ? (isNaN(_activeVehicle.altitudeRelative.value) ? "0.0" : _activeVehicle.altitudeRelative.value.toFixed(1)) + ' ' + _activeVehicle.altitudeRelative.units : "0.0"
    property string _distanceStr:           isNaN(_distance) ? "0" : _distance.toFixed(0) + ' ' + QGroundControl.unitsConversion.appSettingsHorizontalDistanceUnitsString
    property real   _heading:               _activeVehicle   ? _activeVehicle.heading.rawValue : 0
    property real   _distance:              _activeVehicle ? _activeVehicle.distanceToHome.rawValue : 0
    property string _messageTitle:          ""
    property string _messageText:           ""
    property real   _toolsMargin:           ScreenTools.defaultFontPixelWidth * 0.75

    function secondsToHHMMSS(timeS) {
        var sec_num = parseInt(timeS, 10);
        var hours   = Math.floor(sec_num / 3600);
        var minutes = Math.floor((sec_num - (hours * 3600)) / 60);
        var seconds = sec_num - (hours * 3600) - (minutes * 60);
        if (hours   < 10) {hours   = "0"+hours;}
        if (minutes < 10) {minutes = "0"+minutes;}
        if (seconds < 10) {seconds = "0"+seconds;}
        return hours+':'+minutes+':'+seconds;
    }

    QGCToolInsets {
        id:                     _totalToolInsets
        leftEdgeTopInset:       parentToolInsets.leftEdgeTopInset
        leftEdgeCenterInset:    exampleRectangle.leftEdgeCenterInset
        leftEdgeBottomInset:    parentToolInsets.leftEdgeBottomInset
        rightEdgeTopInset:      parentToolInsets.rightEdgeTopInset
        rightEdgeCenterInset:   parentToolInsets.rightEdgeCenterInset
        rightEdgeBottomInset:   parent.width - compassBackground.x
        topEdgeLeftInset:       parentToolInsets.topEdgeLeftInset
        topEdgeCenterInset:     compassArrowIndicator.y + compassArrowIndicator.height
        topEdgeRightInset:      parentToolInsets.topEdgeRightInset
        bottomEdgeLeftInset:    parentToolInsets.bottomEdgeLeftInset
        bottomEdgeCenterInset:  parentToolInsets.bottomEdgeCenterInset
        bottomEdgeRightInset:   parent.height - attitudeIndicator.y
    }

    // This is an example of how you can use parent tool insets to position an element on the custom fly view layer
    // - we use parent topEdgeLeftInset to position the widget below the toolstrip
    // - we use parent bottomEdgeLeftInset to dodge the virtual joystick if enabled
    // - we use the parent leftEdgeTopInset to size our element to the same width as the ToolStripAction
    // - we export the width of this element as the leftEdgeCenterInset so that the map will recenter if the vehicle flys behind this element
    Rectangle {
        id: exampleRectangle
        visible: false // to see this example, set this to true. To view insets, enable the insets viewer FlyView.qml
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: parentToolInsets.topEdgeLeftInset + _toolsMargin
        anchors.bottomMargin: parentToolInsets.bottomEdgeLeftInset + _toolsMargin
        anchors.leftMargin: _toolsMargin
        width: parentToolInsets.leftEdgeTopInset - _toolsMargin
        color: 'red'

        property real leftEdgeCenterInset: visible ? x + width : 0
    }

    // 飞机状态显示
    Item {
        id:             quadcopterStatus

        // 控件的位置和尺寸
        width: 130
        height: 130
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 10  // 以内边距限制小组件位置

        // 半透明的黑色背景矩形
        Rectangle {
            id: background
            width: parent.width
            height: parent.height
            color: "black"
            opacity: 0.7
            radius: 5

            // 添加鼠标事件拦截区域
            MouseArea {
                id: backgroundBlocker
                anchors.fill: parent
                acceptedButtons: Qt.AllButtons
                hoverEnabled: true
                propagateComposedEvents: false  // 关键：阻止事件传递
                onWheel: wheel.accepted = true // 阻止滚轮事件穿透

                // 可选：添加点击反馈效果
                onClicked: {
                    console.log("background area clicked")
                    // 可以在此处添加点击处理逻辑
                }
            }
        }

        // 无人机状态显示区域
        Item {
            id: quadcopterItem
            width: parent.width
            height: parent.height

            // 工作状态显示
            property string bodyStatus: "normal"
            property string motor1Status: "normal"
            property string motor2Status: "warning"
            property string motor3Status: "error"
            property string motor4Status: "normal"

            // 新增颜色定义
            QtObject {
                id: stateColors               
                readonly property color normal: _activeVehicle && _activeVehicle.sub ? "#87CEFA" : "#90EE90"
                readonly property color warning: "#FFFF00"
                readonly property color error: "#FF0000"
                readonly property color inactive: "#80FFFFFF"
            }

            // 中心机身（长方形）
            Rectangle {
                id: body
                width: 40
                height: 40
                radius: 5
                z:2
                color: quadcopterItem.bodyStatus === "inactive" ? stateColors.inactive :
                       quadcopterItem.bodyStatus === "normal" ? stateColors.normal :
                       quadcopterItem.bodyStatus === "warning" ? stateColors.warning :
                       quadcopterItem.bodyStatus === "error" ? stateColors.error : "transparent"
                anchors.centerIn: parent

                // 添加鼠标事件拦截区域
                MouseArea {
                    id: bodyBlocker
                    anchors.fill: parent
                    acceptedButtons: Qt.AllButtons
                    hoverEnabled: true
                    propagateComposedEvents: false  // 关键：阻止事件传递
                    onWheel: wheel.accepted = true // 阻止滚轮事件穿透

                    // 可选：添加点击反馈效果
                    onClicked: {
                        console.log("body area clicked")
                        // 可以在此处添加点击处理逻辑
                    }
                }
            }

            // 四个电机（使用 Rectangle 替代 Ellipse）
            Rectangle {
                id: motor1
                width: 20
                height: 20
                radius: 10 // 设置圆角来模拟圆形
                z:2 // 覆盖住线条
                color: quadcopterItem.motor1Status === "inactive" ? stateColors.inactive:
                       quadcopterItem.motor1Status === "normal" ? stateColors.normal:
                       quadcopterItem.motor1Status === "warning" ? stateColors.warning:
                       quadcopterItem.motor1Status === "error" ? stateColors.error : "transparent"
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 16

                // 添加鼠标事件拦截区域
                MouseArea {
                    id: motor1Blocker
                    anchors.fill: parent
                    acceptedButtons: Qt.AllButtons
                    hoverEnabled: true
                    propagateComposedEvents: false  // 关键：阻止事件传递
                    onWheel: wheel.accepted = true // 阻止滚轮事件穿透

                    // 可选：添加点击反馈效果
                    onClicked: {
                        console.log("motor1 area clicked")
                        // 可以在此处添加点击处理逻辑
                    }
                }
            }

            Rectangle {
                id: motor2
                width: 20
                height: 20
                radius: 10 // 设置圆角来模拟圆形
                z:2
                color: quadcopterItem.motor2Status === "inactive" ? stateColors.inactive :
                       quadcopterItem.motor2Status === "normal" ? stateColors.normal :
                       quadcopterItem.motor2Status === "warning" ? stateColors.warning :
                       quadcopterItem.motor2Status === "error" ? stateColors.error : "transparent"
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 16

                // 添加鼠标事件拦截区域
                MouseArea {
                    id: motor2Blocker
                    anchors.fill: parent
                    acceptedButtons: Qt.AllButtons
                    hoverEnabled: true
                    propagateComposedEvents: false  // 关键：阻止事件传递
                    onWheel: wheel.accepted = true // 阻止滚轮事件穿透

                    // 可选：添加点击反馈效果
                    onClicked: {
                        console.log("motor2 area clicked")
                        // 可以在此处添加点击处理逻辑
                    }
                }
            }

            Rectangle {
                id: motor3
                width: 20
                height: 20
                radius: 10 // 设置圆角来模拟圆形
                z:2
                color: quadcopterItem.motor3Status === "inactive" ? stateColors.inactive :
                       quadcopterItem.motor3Status === "normal" ? stateColors.normal :
                       quadcopterItem.motor3Status === "warning" ? stateColors.warning :
                       quadcopterItem.motor3Status === "error" ? stateColors.error : "transparent"
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: 16

                // 添加鼠标事件拦截区域
                MouseArea {
                    id: motor3Blocker
                    anchors.fill: parent
                    acceptedButtons: Qt.AllButtons
                    hoverEnabled: true
                    propagateComposedEvents: false  // 关键：阻止事件传递
                    onWheel: wheel.accepted = true // 阻止滚轮事件穿透

                    // 可选：添加点击反馈效果
                    onClicked: {
                        console.log("motor3 area clicked")
                        // 可以在此处添加点击处理逻辑
                    }
                }
            }

            Rectangle {
                id: motor4
                width: 20
                height: 20
                radius: 10 // 设置圆角来模拟圆形
                z:2
                color: quadcopterItem.motor4Status === "inactive" ? stateColors.inactive :
                       quadcopterItem.motor4Status === "normal" ? stateColors.normal :
                       quadcopterItem.motor4Status === "warning" ? stateColors.warning :
                       quadcopterItem.motor4Status === "error" ? stateColors.error : "transparent"
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 16

                // 添加鼠标事件拦截区域
                MouseArea {
                    id: motor4Blocker
                    anchors.fill: parent
                    acceptedButtons: Qt.AllButtons
                    hoverEnabled: true
                    propagateComposedEvents: false  // 关键：阻止事件传递
                    onWheel: wheel.accepted = true // 阻止滚轮事件穿透

                    // 可选：添加点击反馈效果
                    onClicked: {
                        console.log("motor4 area clicked")
                        // 可以在此处添加点击处理逻辑
                    }
                }
            }

            // 连接机身和电机的线条
            // 连接机身和电机1
            Rectangle {
                id: line1
                width: 4
                color: "white"
                readonly property real bodyCenterX: body.x + body.width/2
                readonly property real bodyCenterY: body.y + body.height/2
                readonly property real motorCenterX: motor1.x + motor1.width/2
                readonly property real motorCenterY: motor1.y + motor1.height/2

                height: Math.sqrt(Math.pow(motorCenterX - bodyCenterX, 2) +
                                 Math.pow(motorCenterY - bodyCenterY, 2))
                rotation: Math.atan2(motorCenterY - bodyCenterY,
                                    motorCenterX - bodyCenterX) * 180 / Math.PI
                x: bodyCenterX - width/2
                y: bodyCenterY
                transformOrigin: Item.Top
            }

            // 连接机身和电机2
            Rectangle {
                id: line2
                width: 4
                color: "white"
                readonly property real bodyCenterX: body.x + body.width/2
                readonly property real bodyCenterY: body.y + body.height/2
                readonly property real motorCenterX: motor2.x + motor2.width/2
                readonly property real motorCenterY: motor2.y + motor2.height/2

                height: Math.sqrt(Math.pow(motorCenterX - bodyCenterX, 2) +
                                 Math.pow(motorCenterY - bodyCenterY, 2))
                rotation: Math.atan2(motorCenterY - bodyCenterY,
                                    motorCenterX - bodyCenterX) * 180 / Math.PI
                x: bodyCenterX - width/2
                y: bodyCenterY
                transformOrigin: Item.Top
            }

            // 连接机身和电机3
            Rectangle {
                id: line3
                width: 4
                color: "white"
                readonly property real bodyCenterX: body.x + body.width/2
                readonly property real bodyCenterY: body.y + body.height/2
                readonly property real motorCenterX: motor3.x + motor3.width/2
                readonly property real motorCenterY: motor3.y + motor3.height/2

                height: Math.sqrt(Math.pow(motorCenterX - bodyCenterX, 2) +
                                 Math.pow(motorCenterY - bodyCenterY, 2))
                rotation: Math.atan2(motorCenterY - bodyCenterY,
                                    motorCenterX - bodyCenterX) * 180 / Math.PI
                x: bodyCenterX - width/2
                y: bodyCenterY
                transformOrigin: Item.Top
            }

            // 连接机身和电机4
            Rectangle {
                id: line4
                width: 4
                color: "white"
                readonly property real bodyCenterX: body.x + body.width/2
                readonly property real bodyCenterY: body.y + body.height/2
                readonly property real motorCenterX: motor4.x + motor4.width/2
                readonly property real motorCenterY: motor4.y + motor4.height/2

                height: Math.sqrt(Math.pow(motorCenterX - bodyCenterX, 2) +
                                 Math.pow(motorCenterY - bodyCenterY, 2))
                rotation: Math.atan2(motorCenterY - bodyCenterY,
                                    motorCenterX - bodyCenterX) * 180 / Math.PI
                x: bodyCenterX - width/2
                y: bodyCenterY
                transformOrigin: Item.Top
            }
        }

    }

    //-------------------------------------------------------------------------
    //-- Heading Indicator
    Rectangle {
        id:                         compassBar
        height:                     ScreenTools.defaultFontPixelHeight * 1.5
        width:                      ScreenTools.defaultFontPixelWidth  * 50
        color:                      "#DEDEDE"
        radius:                     2
        clip:                       true
        anchors.top:                headingIndicator.bottom
        anchors.topMargin:          -headingIndicator.height / 2
        anchors.horizontalCenter:   parent.horizontalCenter
        Repeater {
            model: 720
            QGCLabel {
                function _normalize(degrees) {
                    var a = degrees % 360
                    if (a < 0) a += 360
                    return a
                }
                property int _startAngle: modelData + 180 + _heading
                property int _angle: _normalize(_startAngle)
                anchors.verticalCenter: parent.verticalCenter
                x:              visible ? ((modelData * (compassBar.width / 360)) - (width * 0.5)) : 0
                visible:        _angle % 45 == 0
                color:          "#75505565"
                font.pointSize: ScreenTools.smallFontPointSize
                text: {
                    switch(_angle) {
                    case 0:     return "N"
                    case 45:    return "NE"
                    case 90:    return "E"
                    case 135:   return "SE"
                    case 180:   return "S"
                    case 225:   return "SW"
                    case 270:   return "W"
                    case 315:   return "NW"
                    }
                    return ""
                }
            }
        }
    }
    Rectangle {
        id:                         headingIndicator
        height:                     ScreenTools.defaultFontPixelHeight
        width:                      ScreenTools.defaultFontPixelWidth * 4
        color:                      qgcPal.windowShadeDark
        anchors.top:                parent.top
        anchors.topMargin:          _toolsMargin
        anchors.horizontalCenter:   parent.horizontalCenter
        QGCLabel {
            text:                   _heading
            color:                  qgcPal.text
            font.pointSize:         ScreenTools.smallFontPointSize
            anchors.centerIn:       parent
        }
    }
    Image {
        id:                         compassArrowIndicator
        height:                     _indicatorsHeight
        width:                      height
        source:                     "/custom/img/compass_pointer.svg"
        fillMode:                   Image.PreserveAspectFit
        sourceSize.height:          height
        anchors.top:                compassBar.bottom
        anchors.topMargin:          -height / 2
        anchors.horizontalCenter:   parent.horizontalCenter
    }

    Rectangle {
        id:                     compassBackground
        anchors.bottom:         attitudeIndicator.bottom
        anchors.right:          attitudeIndicator.left
        anchors.rightMargin:    -attitudeIndicator.width / 2
        width:                  -anchors.rightMargin + compassBezel.width + (_toolsMargin * 2)
        height:                 attitudeIndicator.height * 0.75
        radius:                 2
        color:                  qgcPal.window

        Rectangle {
            id:                     compassBezel
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin:     _toolsMargin
            anchors.left:           parent.left
            width:                  height
            height:                 parent.height - (northLabelBackground.height / 2) - (headingLabelBackground.height / 2)
            radius:                 height / 2
            border.color:           qgcPal.text
            border.width:           1
            color:                  Qt.rgba(0,0,0,0)
        }

        Rectangle {
            id:                         northLabelBackground
            anchors.top:                compassBezel.top
            anchors.topMargin:          -height / 2
            anchors.horizontalCenter:   compassBezel.horizontalCenter
            width:                      northLabel.contentWidth * 1.5
            height:                     northLabel.contentHeight * 1.5
            radius:                     ScreenTools.defaultFontPixelWidth  * 0.25
            color:                      qgcPal.windowShade

            QGCLabel {
                id:                 northLabel
                anchors.centerIn:   parent
                text:               "N"
                color:              qgcPal.text
                font.pointSize:     ScreenTools.smallFontPointSize
            }
        }

        Image {
            id:                 headingNeedle
            anchors.centerIn:   compassBezel
            height:             compassBezel.height * 0.75
            width:              height
            source:             "/custom/img/compass_needle.svg"
            fillMode:           Image.PreserveAspectFit
            sourceSize.height:  height
            transform: [
                Rotation {
                    origin.x:   headingNeedle.width  / 2
                    origin.y:   headingNeedle.height / 2
                    angle:      _heading
                }]
        }

        Rectangle {
            id:                         headingLabelBackground
            anchors.top:                compassBezel.bottom
            anchors.topMargin:          -height / 2
            anchors.horizontalCenter:   compassBezel.horizontalCenter
            width:                      headingLabel.contentWidth * 1.5
            height:                     headingLabel.contentHeight * 1.5
            radius:                     ScreenTools.defaultFontPixelWidth  * 0.25
            color:                      qgcPal.windowShade

            QGCLabel {
                id:                 headingLabel
                anchors.centerIn:   parent
                text:               _heading
                color:              qgcPal.text
                font.pointSize:     ScreenTools.smallFontPointSize
            }
        }
    }

    Rectangle {
        id:                     attitudeIndicator
        anchors.bottomMargin:   _toolsMargin + parentToolInsets.bottomEdgeRightInset
        anchors.rightMargin:    _toolsMargin
        anchors.bottom:         parent.bottom
        anchors.right:          parent.right
        height:                 ScreenTools.defaultFontPixelHeight * 6
        width:                  height
        radius:                 height * 0.5
        color:                  qgcPal.windowShade

        CustomAttitudeWidget {
            size:               parent.height * 0.95
            vehicle:            _activeVehicle
            showHeading:        false
            anchors.centerIn:   parent
        }
    }
}

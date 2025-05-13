/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.12
import QtQuick.Controls         2.4
import QtQuick.Dialogs          1.3
import QtQuick.Layouts          1.12

import QtLocation               5.3
import QtPositioning            5.3
import QtQuick.Window           2.2
import QtQml.Models             2.1

import QGroundControl               1.0
import QGroundControl.Controllers   1.0
import QGroundControl.Controls      1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.Palette       1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Vehicle       1.0

// To implement a custom overlay copy this code to your own control in your custom code source. Then override the
// FlyViewCustomLayer.qml resource with your own qml. See the custom example and documentation for details.
Item {
    id: _root

    property var parentToolInsets               // These insets tell you what screen real estate is available for positioning the controls in your overlay
    property var totalToolInsets:   _toolInsets // These are the insets for your custom overlay additions
    property var mapControl

    // since this file is a placeholder for the custom layer in a standard build, we will just pass through the parent insets
    QGCToolInsets {
        id:                     _toolInsets
        leftEdgeTopInset:       parentToolInsets.leftEdgeTopInset
        leftEdgeCenterInset:    parentToolInsets.leftEdgeCenterInset
        leftEdgeBottomInset:    parentToolInsets.leftEdgeBottomInset
        rightEdgeTopInset:      parentToolInsets.rightEdgeTopInset
        rightEdgeCenterInset:   parentToolInsets.rightEdgeCenterInset
        rightEdgeBottomInset:   parentToolInsets.rightEdgeBottomInset
        topEdgeLeftInset:       parentToolInsets.topEdgeLeftInset
        topEdgeCenterInset:     parentToolInsets.topEdgeCenterInset
        topEdgeRightInset:      parentToolInsets.topEdgeRightInset
        bottomEdgeLeftInset:    parentToolInsets.bottomEdgeLeftInset
        bottomEdgeCenterInset:  parentToolInsets.bottomEdgeCenterInset
        bottomEdgeRightInset:   parentToolInsets.bottomEdgeRightInset
    }


    // 新增飞机状态显示
    Item {
        id: quadcopterStatus

        // 使用 QGCToolInsets 来限制位置和大小
        property var parentToolInsets: _toolInsets  // 获取父控件的内边距信息

        // 控件的位置和尺寸
        width: 130
        height: 130
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: parentToolInsets.leftEdgeBottomInset + 10  // 以内边距限制小组件位置

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

            // 中心机身（长方形）
            Rectangle {
                id: body
                width: 40
                height: 40
                radius: 5
                z:2
                color: quadcopterItem.bodyStatus === "inactive" ? "rgba(255, 255, 255, 0.5)" :
                       quadcopterItem.bodyStatus === "normal" ? "lightgreen" :
                       quadcopterItem.bodyStatus === "warning" ? "yellow" :
                       quadcopterItem.bodyStatus === "error" ? "red" : "transparent"
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
                color: quadcopterItem.motor1Status === "inactive" ? "rgba(255, 255, 255, 0.5)" :
                       quadcopterItem.motor1Status === "normal" ? "lightgreen" :
                       quadcopterItem.motor1Status === "warning" ? "yellow" :
                       quadcopterItem.motor1Status === "error" ? "red" : "transparent"
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
                color: quadcopterItem.motor2Status === "inactive" ? "rgba(255, 255, 255, 0.5)" :
                       quadcopterItem.motor2Status === "normal" ? "lightgreen" :
                       quadcopterItem.motor2Status === "warning" ? "yellow" :
                       quadcopterItem.motor2Status === "error" ? "red" : "transparent"
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
                color: quadcopterItem.motor3Status === "inactive" ? "rgba(255, 255, 255, 0.5)" :
                       quadcopterItem.motor3Status === "normal" ? "lightgreen" :
                       quadcopterItem.motor3Status === "warning" ? "yellow" :
                       quadcopterItem.motor3Status === "error" ? "red" : "transparent"
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
                color: quadcopterItem.motor4Status === "inactive" ? "rgba(255, 255, 255, 0.5)" :
                       quadcopterItem.motor4Status === "normal" ? "lightgreen" :
                       quadcopterItem.motor4Status === "warning" ? "yellow" :
                       quadcopterItem.motor4Status === "error" ? "red" : "transparent"
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




}

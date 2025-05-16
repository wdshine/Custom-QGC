/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick          2.11
import QtQuick.Layouts  1.11

import QGroundControl                       1.0
import QGroundControl.Controls              1.0
import QGroundControl.MultiVehicleManager   1.0
import QGroundControl.ScreenTools           1.0
import QGroundControl.Palette               1.0

// 自定义工具指示器
Item {
    id:             _root
    width:          customIcon.width * 1.1
    anchors.top:    parent.top
    anchors.bottom: parent.bottom

    property bool showIndicator: true

    // 这里可以添加您需要的属性
    property var _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle

    // 弹出信息组件
    Component {
        id: customInfo

        Rectangle {
            width:  customCol.width + ScreenTools.defaultFontPixelWidth * 3
            height: customCol.height + ScreenTools.defaultFontPixelHeight * 2
            radius: ScreenTools.defaultFontPixelHeight * 0.5
            color:  qgcPal.window
            border.color: qgcPal.text

            Column {
                id:                 customCol
                spacing:            ScreenTools.defaultFontPixelHeight * 0.5
                width:              Math.max(customGrid.width, customLabel.width)
                anchors.margins:    ScreenTools.defaultFontPixelHeight
                anchors.centerIn:   parent

                QGCLabel {
                    id:             customLabel
                    text:           qsTr("自定义工具")
                    font.family:    ScreenTools.demiboldFontFamily
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                // 在这里添加您的自定义控件
                GridLayout {
                    id:                 customGrid
                    anchors.margins:    ScreenTools.defaultFontPixelWidth
                    columnSpacing:      ScreenTools.defaultFontPixelWidth
                    columns:            2
                    anchors.horizontalCenter: parent.horizontalCenter

                    QGCButton {
                        text: qsTr("功能1")
                        onClicked: {
                            // 实现功能1的逻辑
                            console.log("功能1被点击")
                        }
                    }

                    QGCButton {
                        text: qsTr("功能2")
                        onClicked: {
                            // 实现功能2的逻辑
                            console.log("功能2被点击")
                        }
                    }
                }
            }
        }
    }

    QGCColoredImage {
        id:                 customIcon
        anchors.top:        parent.top
        anchors.bottom:     parent.bottom
        width:              height
        sourceSize.height:  height
        source:             "/qmlimages/Gears.svg" // 使用现有图标或添加自己的图标
        fillMode:           Image.PreserveAspectFit
        color:              qgcPal.text
    }

    MouseArea {
        anchors.fill:   parent
        onClicked: {
            mainWindow.showIndicatorPopup(_root, customInfo)
        }
    }
}
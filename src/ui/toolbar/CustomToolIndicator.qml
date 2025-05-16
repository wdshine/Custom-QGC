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

// �Զ��幤��ָʾ��
Item {
    id:             _root
    width:          customIcon.width * 1.1
    anchors.top:    parent.top
    anchors.bottom: parent.bottom

    property bool showIndicator: true

    // ��������������Ҫ������
    property var _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle

    // ������Ϣ���
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
                    text:           qsTr("�Զ��幤��")
                    font.family:    ScreenTools.demiboldFontFamily
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                // ��������������Զ���ؼ�
                GridLayout {
                    id:                 customGrid
                    anchors.margins:    ScreenTools.defaultFontPixelWidth
                    columnSpacing:      ScreenTools.defaultFontPixelWidth
                    columns:            2
                    anchors.horizontalCenter: parent.horizontalCenter

                    QGCButton {
                        text: qsTr("����1")
                        onClicked: {
                            // ʵ�ֹ���1���߼�
                            console.log("����1�����")
                        }
                    }

                    QGCButton {
                        text: qsTr("����2")
                        onClicked: {
                            // ʵ�ֹ���2���߼�
                            console.log("����2�����")
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
        source:             "/qmlimages/Gears.svg" // ʹ������ͼ�������Լ���ͼ��
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
/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick 2.12

import QGroundControl               1.0
import QGroundControl.ScreenTools   1.0

//-------------------------------------------------------------------------
//-- Toolbar Indicators
Row {
    id:                 indicatorRow
    anchors.top:        parent.top
    anchors.bottom:     parent.bottom
    anchors.margins:    _toolIndicatorMargins
    spacing:            ScreenTools.defaultFontPixelWidth * 1.5

    property var  _activeVehicle:           QGroundControl.multiVehicleManager.activeVehicle
    property real _toolIndicatorMargins:    ScreenTools.defaultFontPixelHeight * 0.66

    function dropMessageIndicatorTool() {
        toolIndicatorsRepeater.dropMessageIndicatorTool();
    }

    // 通过 QGroundControl.corePlugin.toolBarIndicators 加载核心插件定义的动态指示器
    Repeater {
        id:     appRepeater
        model:  QGroundControl.corePlugin.toolBarIndicators
        Loader {
            anchors.top:        parent.top
            anchors.bottom:     parent.bottom
            source:             modelData
            visible:            item.showIndicator
        }
    }

    // 绑定当前激活载具的 toolIndicators 属性
    // 提供消息指示器处理接口 dropMessageIndicatorTool()
    Repeater {
        id:     toolIndicatorsRepeater
        model:  _activeVehicle ? _activeVehicle.toolIndicators : []

        function dropMessageIndicatorTool() {
            for (var i=0; i<count; i++) {
                var thisTool = itemAt(i);
                if (thisTool.item.dropMessageIndicator) {
                    thisTool.item.dropMessageIndicator();
                }
            }
        }

        Loader {
            anchors.top:        parent.top
            anchors.bottom:     parent.bottom
            source:             modelData
            visible:            item.showIndicator
        }
    }

    // 绑定当前激活载具的 toolIndicators 属性
    Repeater {
        model: _activeVehicle ? _activeVehicle.modeIndicators : []
        Loader {
            anchors.top:        parent.top
            anchors.bottom:     parent.bottom
            source:             modelData
            visible:            item.showIndicator
            // visible:            false
        }
    }
}

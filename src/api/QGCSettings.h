/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

// 封装单个设置页面的元数据，包括标题、QML资源的URL和图标
// 使得在QML界面中可以动态加载和显示这些设置页面
// 作为配置项的数据模型，方便在设置菜单中管理和展示各个设置选项

#pragma once

#include <QObject>
#include <QUrl>

/// @file
/// @brief Core Plugin Interface for QGroundControl. Settings element.
/// @author Gus Grubba <gus@auterion.com>

class QGCSettings : public QObject
{
    Q_OBJECT
public:
    QGCSettings(QString title, QUrl url, QUrl icon = QUrl());

    Q_PROPERTY(QString  title       READ title      CONSTANT)
    Q_PROPERTY(QUrl     url         READ url        CONSTANT)
    Q_PROPERTY(QUrl     icon        READ icon       CONSTANT)

    virtual QString     title       () { return _title; }
    virtual QUrl        url         () { return _url;   }
    virtual QUrl        icon        () { return _icon;  }

protected:
    QString _title;
    QUrl    _url;
    QUrl    _icon;
};

/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

// ��װ��������ҳ���Ԫ���ݣ��������⡢QML��Դ��URL��ͼ��
// ʹ����QML�����п��Զ�̬���غ���ʾ��Щ����ҳ��
// ��Ϊ�����������ģ�ͣ����������ò˵��й����չʾ��������ѡ��

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

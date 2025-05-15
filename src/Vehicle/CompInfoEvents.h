/****************************************************************************
 *
 * (c) 2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "CompInfo.h"

#include <QObject>

class FactMetaData;
class Vehicle;
class FirmwarePlugin;
// �¼�����ϵͳ��Ԫ���ݹ�����ģ�Ϊ�ɿش�����롢״̬�¼����ṩ�ṹ��Ԫ����֧��
// ͨ��JSONԪ����ʵ�ֲ�ͬ�̼��汾�¼�ϵͳ�ļ���������
class CompInfoEvents : public CompInfo
{
    Q_OBJECT

public:
    CompInfoEvents(uint8_t compId, Vehicle* vehicle, QObject* parent = nullptr);

    // Overrides from CompInfo
    void setJson(const QString& metadataJsonFileName) override;

private:
};

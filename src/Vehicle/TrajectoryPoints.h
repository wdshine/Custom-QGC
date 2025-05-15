/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "QmlObjectListModel.h"

#include <QGeoCoordinate>

class Vehicle;

// TrajectoryPoints�ࣨ�켣�������
// 1. �켣���ݴ洢
//    - ʹ�� QVariantList _points ά���켣�����꼯��
//    - ͨ�� QGeoCoordinate _lastPoint ���������Ч�����
// 2. �Զ���¼����
//    - ���������ƶ�ʱ�Զ���ӹ켣��
//    - Ӧ��˫���ݲ���ˣ�
//      - �����ݲ_distanceTolerance=2.0�ף�
//      - ��λ���ݲ_azimuthTolerance=1.5�ȣ�
// 3. �������ƽӿ�
// 4. QML����ͬ��
//    - ͨ�� list() ������¶�켣�㼯�ϸ�QML
//    - ֧�ֶ�̬�켣���ӻ�����
class TrajectoryPoints : public QObject
{
    Q_OBJECT

public:
    TrajectoryPoints(Vehicle* vehicle, QObject* parent = nullptr);

    Q_INVOKABLE QVariantList list(void) const { return _points; }

    void start  (void);
    void stop   (void);

public slots:
    void clear  (void);

signals:
    void pointAdded     (QGeoCoordinate coordinate);
    void updateLastPoint(QGeoCoordinate coordinate);
    void pointsCleared  (void);

private slots:
    void _vehicleCoordinateChanged(QGeoCoordinate coordinate);

private:
    Vehicle*        _vehicle;
    QVariantList    _points;
    QGeoCoordinate  _lastPoint;
    double          _lastAzimuth;

    static constexpr double _distanceTolerance = 2.0;
    static constexpr double _azimuthTolerance = 1.5;
};

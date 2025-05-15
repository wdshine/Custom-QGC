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

// TrajectoryPoints类（轨迹点管理）：
// 1. 轨迹数据存储
//    - 使用 QVariantList _points 维护轨迹点坐标集合
//    - 通过 QGeoCoordinate _lastPoint 缓存最后有效坐标点
// 2. 自动记录机制
//    - 当飞行器移动时自动添加轨迹点
//    - 应用双重容差过滤：
//      - 距离容差（_distanceTolerance=2.0米）
//      - 方位角容差（_azimuthTolerance=1.5度）
// 3. 操作控制接口
// 4. QML界面同步
//    - 通过 list() 方法暴露轨迹点集合给QML
//    - 支持动态轨迹可视化更新
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

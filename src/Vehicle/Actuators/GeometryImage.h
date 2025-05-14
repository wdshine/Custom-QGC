/****************************************************************************
 *
 * (c) 2021 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

 // 动态生成无人机执行器布局的图像，支持QML界面显示，允许通过位置进行交互操作

#pragma once

#include <QObject>
#include <QString>

#include <functional>
#include <cmath>

#include <QQuickImageProvider>
#include <QVector2D>
#include <QPainter>

#include <QGCPalette.h>

#include "Common.h"


namespace GeometryImage {

/**
 * Renders an image of an airframe geometry (currently only multirotor)
 */
class VehicleGeometryImageProvider : public QQuickImageProvider
{
public:

    struct ImagePosition {
        ActuatorGeometry::Type type;
        int index;
        QPointF position;
        float radius;
    };

    void drawAxisIndicator(QPainter& p, const QPointF& origin, float fontSize, const QColor& color);

    QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize) override;

    static VehicleGeometryImageProvider* instance();

    int getHighlightedMotorIndexAtPos(const QPointF& position);

    QList<ActuatorGeometry>& actuators() { return _actuators; }

    int numMotors() const;

private:
    VehicleGeometryImageProvider();
    ~VehicleGeometryImageProvider() = default;

    QList<ActuatorGeometry> _actuators{};

    QList<ImagePosition> _actuatorImagePositions{}; ///< highlighted actuators image positions
    QGCPalette           _palette;
};

} // namespace GeometryImage


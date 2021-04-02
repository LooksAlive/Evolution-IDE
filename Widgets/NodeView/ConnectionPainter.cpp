#include <QIcon>
#include <QPainterPathStroker>
#include "Connection.h"

#include "ConnectionPainter.h"


void ConnectionPainter::paint(QPainter* painter, Connection* connection) {
    drawHoveredOrSelected(painter, connection);

    drawSketchLine(painter, connection);

    drawNormalLine(painter, connection);

#ifdef NODE_DEBUG_DRAWING
    debugDrawing(painter, connection);
#endif

    // draw end points

    QPointF const & source = connection->source();
    QPointF const & sink   = connection->sink();

    const float pointDiameter = connection->PointDiameter;

    painter->setPen(connection->ConstructionColor);
    painter->setBrush(connection->ConstructionColor);
    double const pointRadius = pointDiameter / 2.0;
    painter->drawEllipse(source, pointRadius, pointRadius);
    painter->drawEllipse(sink, pointRadius, pointRadius);
}

QPainterPath ConnectionPainter::getPainterStroke(Connection* connection) {
    auto cubic = cubicPath(connection);

    QPointF const& source = connection->source();
    QPainterPath result(source);

    unsigned int segments = 20;

    for (auto i = 0ul; i < segments; ++i)
    {
        double ratio = double(i + 1) / segments;
        result.lineTo(cubic.pointAtPercent(ratio));
    }

    QPainterPathStroker stroker; stroker.setWidth(10.0);

    return stroker.createStroke(result);
}

QPainterPath ConnectionPainter::cubicPath(Connection *connection) {
    QPointF const& source = connection->source();
    QPointF const& sink   = connection->sink();

    auto c1c2 = connection->pointsC1C2();

    // cubic spline
    QPainterPath cubic(source);

    cubic.cubicTo(c1c2.first, c1c2.second, sink);

    return cubic;
}

void ConnectionPainter::debugDrawing(QPainter * painter, Connection* connection) {
    Q_UNUSED(painter);
    Q_UNUSED(connection);

    {
        QPointF const& source = connection->source();
        QPointF const& sink   = connection->sink();

        auto points = connection->pointsC1C2();

        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);

        painter->drawLine(QLineF(source, points.first));
        painter->drawLine(QLineF(points.first, points.second));
        painter->drawLine(QLineF(points.second, sink));
        painter->drawEllipse(points.first, 3, 3);
        painter->drawEllipse(points.second, 3, 3);

        painter->setBrush(Qt::NoBrush);

        painter->drawPath(cubicPath(connection));
    }

    {
        painter->setPen(Qt::yellow);

        painter->drawRect(connection->boundingRect());
    }
}

void ConnectionPainter::drawSketchLine(QPainter * painter, Connection* connection) {
    const ConnectionState& state =
            connection->connectionState();

    if (state.requiresPort()) {
        QPen p;
        p.setWidth(connection->ConstructionLineWidth);
        p.setColor(connection->ConstructionColor);
        p.setStyle(Qt::DashLine);

        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);

        auto cubic = cubicPath(connection);
        // cubic spline
        painter->drawPath(cubic);
    }
}

void ConnectionPainter::drawHoveredOrSelected(QPainter * painter, Connection* connection) {
    bool const hovered = connection->_hovered;
    bool const selected = connection->isSelected();

    // drawn as a fat background
    if (hovered || selected)
    {
        QPen p;
        double const lineWidth = connection->LineWidth;

        p.setWidth(2 * lineWidth);
        p.setColor(selected ? connection->SelectedHaloColor : connection->HoveredColor);

        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);

        // cubic spline
        auto cubic = cubicPath(connection);
        painter->drawPath(cubic);
    }
}


void ConnectionPainter::drawNormalLine(QPainter * painter, Connection* connection) {
    ConnectionState const& state =
            connection->connectionState();

    if (state.requiresPort())
        return;

    // colors
    QColor normalColorOut  = connection->NormalColor;
    QColor normalColorIn   = connection->NormalColor;
    QColor selectedColor = connection->SelectedColor;

    bool gradientColor = false;

    if (connection->UseDataDefinedColors) {
        auto dataTypeOut = connection->dataType(Node::PortType::Out);
        auto dataTypeIn = connection->dataType(Node::PortType::In);

        gradientColor = (dataTypeOut.id != dataTypeIn.id);

        normalColorOut  = connection->normalColor(dataTypeOut.id);
        normalColorIn   = connection->normalColor(dataTypeIn.id);
        selectedColor = normalColorOut.darker(200);
    }

    // geometry
    double const lineWidth = connection->LineWidth;

    // draw normal line
    QPen p;

    p.setWidth(lineWidth);

    bool const selected = connection->isSelected();


    auto cubic = cubicPath(connection);
    if (gradientColor)
    {
        painter->setBrush(Qt::NoBrush);

        QColor c = normalColorOut;
        if (selected)
            c = c.darker(200);
        p.setColor(c);
        painter->setPen(p);

        unsigned int const segments = 60;

        for (unsigned int i = 0ul; i < segments; ++i)
        {
            double ratioPrev = double(i) / segments;
            double ratio = double(i + 1) / segments;

            if (i == segments / 2)
            {
                QColor c = normalColorIn;
                if (selected)
                    c = c.darker(200);

                p.setColor(c);
                painter->setPen(p);
            }
            painter->drawLine(cubic.pointAtPercent(ratioPrev),
                              cubic.pointAtPercent(ratio));
        }

        {
            QIcon icon(":convert.png");

            QPixmap pixmap = icon.pixmap(QSize(22, 22));
            painter->drawPixmap(cubic.pointAtPercent(0.50) - QPoint(pixmap.width()/2,
                                                                    pixmap.height()/2),
                                pixmap);

        }
    }
    else
    {
        p.setColor(normalColorOut);

        if (selected)
        {
            p.setColor(selectedColor);
        }

        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);

        painter->drawPath(cubic);
    }
}

#pragma once

#include <QPainter>

class Connection;

class ConnectionPainter {
public:
    static void paint(QPainter* painter, Connection* connection);
    static QPainterPath getPainterStroke(Connection* connection);

private:
    static void drawSketchLine(QPainter * painter, Connection* connection);
    static void debugDrawing(QPainter * painter, Connection* connection);
    static QPainterPath cubicPath(Connection *connection);
    static void drawHoveredOrSelected(QPainter * painter, Connection* connection);
    static void drawNormalLine(QPainter * painter, Connection* connection);

};

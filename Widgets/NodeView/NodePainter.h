#pragma once

#include <QPainter>

#include "NodeScene.h"
#include "Node.h"


class NodePainter {
public:
    NodePainter();
    ~NodePainter() = default;

public:
    static void paint(QPainter* painter, Node* node, NodeScene* scene);

    static void drawNodeRect(QPainter* painter, Node* node);
    static void drawModelName(QPainter* painter, Node* node);
    static void drawEntryLabels(QPainter* painter, Node* node);
    static void drawConnectionPoints(QPainter* painter, Node* node, NodeScene* scene, Connection *connection);
    static void drawFilledConnectionPoints(QPainter* painter, Node* node, Connection *connection);
    static void drawResizeRect(QPainter* painter, Node* node);
    static void drawValidationRect(QPainter * painter, Node* node);
};

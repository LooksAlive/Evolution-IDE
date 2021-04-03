#include <QMargins>
#include <cmath>

#include "Connection.h"
#include "NodePainter.h"

void NodePainter::paint(QPainter* painter, Node* node, NodeScene* scene) {
    node->recalculateSize(painter->font());
    
    //--------------------------------------------
    
    drawNodeRect(painter, node);
    
    drawConnectionPoints(painter, node, scene);
    
    drawFilledConnectionPoints(painter, node);
    
    drawModelName(painter, node);
    
    drawEntryLabels(painter, node);
    
    drawResizeRect(painter, node);
    
    drawValidationRect(painter, node);
    
    /// call custom painter
    /*
  if (auto painterDelegate = node->painterDelegate())
  {
    painterDelegate->paint(painter);
  }
  */
}


void NodePainter::drawNodeRect(QPainter* painter, Node* node) {    
    const QColor color = node->isSelected()
            ? node->SelectedBoundaryColor
            : node->NormalBoundaryColor;
    
    if (node->_hovered)
    {
        QPen p(color, node->HoveredPenWidth);
        painter->setPen(p);
    }
    else
    {
        QPen p(color, node->PenWidth);
        painter->setPen(p);
    }
    
    QLinearGradient gradient(QPointF(0.0, 0.0),
                             QPointF(2.0, node->_height));
    
    gradient.setColorAt(0.0, node->GradientColor0);
    gradient.setColorAt(0.03, node->GradientColor1);
    gradient.setColorAt(0.97, node->GradientColor2);
    gradient.setColorAt(1.0, node->GradientColor3);
    
    painter->setBrush(gradient);
    
    const float diam = node->ConnectionPointDiameter;
    
    QRectF boundary( -diam, -diam, 2.0 * diam + node->_width, 2.0 * diam + node->_height);
    
    double const radius = 3.0;
    
    painter->drawRoundedRect(boundary, radius, radius);
}


void NodePainter::drawConnectionPoints(QPainter* painter, Node* node, NodeScene* scene, Connection *connection) {
    float diameter = node->ConnectionPointDiameter;
    auto  reducedDiameter = diameter * 0.6;
    
    for(Node::PortType portType: {Node::PortType::Out, Node::PortType::In})
    {
        size_t n = node->getEntries(portType).size();
        
        for (unsigned int i = 0; i < n; ++i)
        {
            QPointF p = node->portScenePosition(i, portType);
            
            auto const & dataType = model->dataType(portType, i);
            
            bool canConnect = (node->getEntries(portType)[i].empty() ||
                               (portType == Node::PortType::Out &&
                                model->portOutConnectionPolicy(i) == NodeDataModel::ConnectionPolicy::Many) );
            
            double r = 1.0;
            if (node->isReacting() &&
                    canConnect &&
                    portType == state.reactingPortType())
            {
                
                auto   diff = node->_draggingPos - p;
                double dist = std::sqrt(QPointF::dotProduct(diff, diff));
                bool   typeConvertable = false;
                
                {
                    if (portType == Node::PortType::In)
                    {
                        typeConvertable = scene.registry().getTypeConverter(state.reactingDataType(), dataType) != nullptr;
                    }
                    else
                    {
                        typeConvertable = scene.registry().getTypeConverter(dataType, state.reactingDataType()) != nullptr;
                    }
                }
                
                if (state.reactingDataType().id == dataType.id || typeConvertable)
                {
                    double const thres = 40.0;
                    r = (dist < thres) ?
                                (2.0 - dist / thres ) :
                                1.0;
                }
                else
                {
                    double const thres = 80.0;
                    r = (dist < thres) ?
                                (dist / thres) :
                                1.0;
                }
            }
            
            if (connection->UseDataDefinedColors)
            {
                painter->setBrush(connection->normalColor(dataType.id));
            }
            else
            {
                painter->setBrush(node->ConnectionPointColor);
            }
            
            painter->drawEllipse(p,
                                 reducedDiameter * r,
                                 reducedDiameter * r);
        }
    };
}


void NodePainter::drawFilledConnectionPoints(QPainter * painter, Node* node, Connection *connection) {
    auto diameter = node->ConnectionPointDiameter;
    
    for(Node::PortType portType: {Node::PortType::Out, Node::PortType::In})
    {
        size_t n = node->getEntries(portType).size();
        
        for (size_t i = 0; i < n; ++i)
        {
            QPointF p = node->portScenePosition(i, portType);
            
            if (!node->getEntries(portType)[i].empty())
            {
                auto const & dataType = model->dataType(portType, i);
                
                if (connection->UseDataDefinedColors)
                {
                    QColor const c = connection->normalColor(dataType.id);
                    painter->setPen(c);
                    painter->setBrush(c);
                }
                else
                {
                    painter->setPen(node->FilledConnectionPointColor);
                    painter->setBrush(node->FilledConnectionPointColor);
                }
                
                painter->drawEllipse(p,
                                     diameter * 0.4,
                                     diameter * 0.4);
            }
        }
    }
}


void NodePainter::drawModelName(QPainter * painter, Node* node) {        
    //if (!model->captionVisible())
    //    return;
    
    //QString const &name = model->caption();
    
    QFont f = painter->font();
    
    f.setBold(true);
    
    QFontMetrics metrics(f);
    
    auto rect = metrics.boundingRect("name");   // name
    
    QPointF position((node->_width - rect.width()) / 2.0,
                     (node->_spacing + node->_entryHeight) / 3.0);
    
    painter->setFont(f);
    painter->setPen(node->FontColor);
    painter->drawText(position, "name");    // name
    
    f.setBold(false);
    painter->setFont(f);
}


void NodePainter::drawEntryLabels(QPainter * painter, Node* node) {
    const QFontMetrics & metrics = painter->fontMetrics();
    
    for(Node::PortType portType: {Node::PortType::Out, Node::PortType::In}) {
        const auto& entries = node->getEntries(portType);
        const size_t n = entries.size();
        for (size_t i = 0; i < n; ++i)
            QPointF point = node->portScenePosition(i, portType);

            if (entries[i].empty())
                painter->setPen(node->FontColorFaded);
            else
                painter->setPen(node->FontColor);
            
            QString s;
            
            if (model->portCaptionVisible(Node::portType, i))
            {
                s = model->portCaption(Node::portType, i);
            }
            else
            {
                s = model->dataType(Node::portType, i).name;
            }
            
            auto rect = metrics.boundingRect(s);
            
            point.setY(p.y() + rect.height() / 4.0);
            
            switch (portType)
            {
            case Node::PortType::In:
                point.setX(5.0);
                break;
                
            case Node::PortType::Out:
                point.setX(node->_width - 5.0 - rect.width());
                break;
                
            default:
                break;
            }
            
            painter->drawText(point, s);
        }
    }
}


void NodePainter::drawResizeRect(QPainter * painter, Node* node) {
    if (model->resizable()) {
        painter->setBrush(Qt::gray);
        painter->drawEllipse(node->resizeRect());
    }
}


void NodePainter::drawValidationRect(QPainter * painter, Node* node) {
    auto modelValidationState = model->validationState();
    
    if (modelValidationState != NodeValidationState::Valid) {
        const QColor color = node->isSelected()
                ? node->SelectedBoundaryColor
                : node->NormalBoundaryColor;
        
        if (node->_hovered)
        {
            QPen p(color, node->HoveredPenWidth);
            painter->setPen(p);
        }
        else
        {
            QPen p(color, node->PenWidth);
            painter->setPen(p);
        }
        
        //Drawing the validation message background
        if (modelValidationState == NodeValidationState::Error)
        {
            painter->setBrush(node->ErrorColor);
        }
        else
        {
            painter->setBrush(node->WarningColor);
        }
        
        double const radius = 3.0;
        
        const float diam = node->ConnectionPointDiameter;
        
        QRectF boundary(-diam,
                        -diam + node->_height - node->_validationHeight,
                        2.0 * diam + node->_width,
                        2.0 * diam + node->_validationHeight);
        
        painter->drawRoundedRect(boundary, radius, radius);
        
        painter->setBrush(Qt::gray);
        
        //Drawing the validation message itself
        QString const &errorMsg = model->validationMessage();
        
        QFont f = painter->font();
        
        QFontMetrics metrics(f);
        
        auto rect = metrics.boundingRect(errorMsg);
        
        QPointF position((node->_width - rect.width()) / 2.0,
                         node->_height - (node->_validationHeight - diam) / 2.0);
        
        painter->setFont(f);
        painter->setPen(node->FontColor);
        painter->drawText(position, errorMsg);
    }
}

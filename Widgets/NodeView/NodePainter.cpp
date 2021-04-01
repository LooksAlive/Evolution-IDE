#include <QMargins>
#include <cmath>

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
    NodeStyle const& nodeStyle = model->nodeStyle();
    
    auto color = graphicsObject.isSelected()
            ? nodeStyle.SelectedBoundaryColor
            : nodeStyle.NormalBoundaryColor;
    
    if (geom.hovered())
    {
        QPen p(color, nodeStyle.HoveredPenWidth);
        painter->setPen(p);
    }
    else
    {
        QPen p(color, nodeStyle.PenWidth);
        painter->setPen(p);
    }
    
    QLinearGradient gradient(QPointF(0.0, 0.0),
                             QPointF(2.0, geom.height()));
    
    gradient.setColorAt(0.0, nodeStyle.GradientColor0);
    gradient.setColorAt(0.03, nodeStyle.GradientColor1);
    gradient.setColorAt(0.97, nodeStyle.GradientColor2);
    gradient.setColorAt(1.0, nodeStyle.GradientColor3);
    
    painter->setBrush(gradient);
    
    float diam = nodeStyle.ConnectionPointDiameter;
    
    QRectF boundary( -diam, -diam, 2.0 * diam + geom.width(), 2.0 * diam + geom.height());
    
    double const radius = 3.0;
    
    painter->drawRoundedRect(boundary, radius, radius);
}


void NodePainter::drawConnectionPoints(QPainter* painter, Node* node, NodeScene* scene) {
    NodeStyle const& nodeStyle      = model->nodeStyle();
    auto const     &connectionStyle = StyleCollection::connectionStyle();
    
    float diameter = nodeStyle.ConnectionPointDiameter;
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
                
                auto   diff = geom.draggingPos() - p;
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
            
            if (connectionStyle.useDataDefinedColors())
            {
                painter->setBrush(connectionStyle.normalColor(dataType.id));
            }
            else
            {
                painter->setBrush(nodeStyle.ConnectionPointColor);
            }
            
            painter->drawEllipse(p,
                                 reducedDiameter * r,
                                 reducedDiameter * r);
        }
    };
}


void NodePainter::drawFilledConnectionPoints(QPainter * painter, Node* node) {
    NodeStyle const& nodeStyle       = model->nodeStyle();
    auto const     & connectionStyle = StyleCollection::connectionStyle();
    
    auto diameter = nodeStyle.ConnectionPointDiameter;
    
    for(PortType portType: {PortType::Out, PortType::In})
    {
        size_t n = state.getEntries(portType).size();
        
        for (size_t i = 0; i < n; ++i)
        {
            QPointF p = node->portScenePosition(i, portType);
            
            if (!node->getEntries(portType)[i].empty())
            {
                auto const & dataType = model->dataType(portType, i);
                
                if (connectionStyle.useDataDefinedColors())
                {
                    QColor const c = connectionStyle.normalColor(dataType.id);
                    painter->setPen(c);
                    painter->setBrush(c);
                }
                else
                {
                    painter->setPen(nodeStyle.FilledConnectionPointColor);
                    painter->setBrush(nodeStyle.FilledConnectionPointColor);
                }
                
                painter->drawEllipse(p,
                                     diameter * 0.4,
                                     diameter * 0.4);
            }
        }
    }
}


void NodePainter::drawModelName(QPainter * painter, Node* node) {
    NodeStyle const& nodeStyle = model->nodeStyle();
    
    Q_UNUSED(state);
    
    if (!model->captionVisible())
        return;
    
    QString const &name = model->caption();
    
    QFont f = painter->font();
    
    f.setBold(true);
    
    QFontMetrics metrics(f);
    
    auto rect = metrics.boundingRect(name);
    
    QPointF position((node->_width - rect.width()) / 2.0,
                     (node->_spacing + geom.entryHeight()) / 3.0);
    
    painter->setFont(f);
    painter->setPen(nodeStyle.FontColor);
    painter->drawText(position, name);
    
    f.setBold(false);
    painter->setFont(f);
}


void NodePainter::drawEntryLabels(QPainter * painter, Node* node) {
    QFontMetrics const & metrics =
            painter->fontMetrics();
    
    for(Node::PortType portType: {Node::PortType::Out, Node::PortType::In})
    {
        auto const &nodeStyle = model->nodeStyle();
        
        auto& entries = node->getEntries(portType);
        
        size_t n = entries.size();
        
        for (size_t i = 0; i < n; ++i)
        {
            QPointF p = geom.portScenePosition(i, portType);
            
            if (entries[i].empty())
                painter->setPen(nodeStyle.FontColorFaded);
            else
                painter->setPen(nodeStyle.FontColor);
            
            QString s;
            
            if (model->portCaptionVisible(portType, i))
            {
                s = model->portCaption(portType, i);
            }
            else
            {
                s = model->dataType(portType, i).name;
            }
            
            auto rect = metrics.boundingRect(s);
            
            p.setY(p.y() + rect.height() / 4.0);
            
            switch (portType)
            {
            case Node::PortType::In:
                p.setX(5.0);
                break;
                
            case Node::PortType::Out:
                p.setX(node->_width - 5.0 - rect.width());
                break;
                
            default:
                break;
            }
            
            painter->drawText(p, s);
        }
    }
}


void NodePainter::drawResizeRect(QPainter * painter, Node* node) {
    if (model->resizable())
    {
        painter->setBrush(Qt::gray);
        
        painter->drawEllipse(geom.resizeRect());
    }
}


void NodePainter::drawValidationRect(QPainter * painter, Node* node) {
    auto modelValidationState = model->validationState();
    
    if (modelValidationState != NodeValidationState::Valid)
    {
        NodeStyle const& nodeStyle = model->nodeStyle();
        
        auto color = graphicsObject.isSelected()
                ? nodeStyle.SelectedBoundaryColor
                : nodeStyle.NormalBoundaryColor;
        
        if (geom.hovered())
        {
            QPen p(color, nodeStyle.HoveredPenWidth);
            painter->setPen(p);
        }
        else
        {
            QPen p(color, nodeStyle.PenWidth);
            painter->setPen(p);
        }
        
        //Drawing the validation message background
        if (modelValidationState == NodeValidationState::Error)
        {
            painter->setBrush(nodeStyle.ErrorColor);
        }
        else
        {
            painter->setBrush(nodeStyle.WarningColor);
        }
        
        double const radius = 3.0;
        
        float diam = nodeStyle.ConnectionPointDiameter;
        
        QRectF boundary(-diam,
                        -diam + geom.height() - geom.validationHeight(),
                        2.0 * diam + geom.width(),
                        2.0 * diam + geom.validationHeight());
        
        painter->drawRoundedRect(boundary, radius, radius);
        
        painter->setBrush(Qt::gray);
        
        //Drawing the validation message itself
        QString const &errorMsg = model->validationMessage();
        
        QFont f = painter->font();
        
        QFontMetrics metrics(f);
        
        auto rect = metrics.boundingRect(errorMsg);
        
        QPointF position((node->_width - rect.width()) / 2.0,
                         node->_height - (geom.validationHeight() - diam) / 2.0);
        
        painter->setFont(f);
        painter->setPen(nodeStyle.FontColor);
        painter->drawText(position, errorMsg);
    }
}

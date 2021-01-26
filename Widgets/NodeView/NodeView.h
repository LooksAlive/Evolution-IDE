#ifndef EVOLUTION_IDE_NODEVIEW_H
#define EVOLUTION_IDE_NODEVIEW_H

#include <QWidget>

class NodeView : public QWidget{
Q_OBJECT
public:
    explicit NodeView(QWidget *parent = nullptr);
    ~NodeView() = default;

private:
    void createNodeView();

};


#endif //EVOLUTION_IDE_NODEVIEW_H

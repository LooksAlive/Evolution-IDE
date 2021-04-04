#ifndef STRUCTURETREE_H
#define STRUCTURETREE_H

#include <QTreeWidget>
#include <QWidget>

#include "Diagnostics.h"    // diagnostic level for item highlight

/*
 * filled from AST... top is class, children are functions
 * one click ... brief info
 * bouble-click open in node (if class -> whole nodes structure otherwise only 1 function, enum, ...)
*/

class StructureTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit StructureTree(QWidget *parent = nullptr);
    ~StructureTree() = default;

    // symbols get their own icons
    void addClass(const QString& className);
    // removes also all connected children
    void removeClass(const QString& className);

    // for certain class
    void addClassItem(const QString& className, const QString& symbolName);
    void removeClassItem(const QString& className, const QString& symbolName);

    // returns toplevelitem ; child
    QTreeWidgetItem* findClassIndex(const QString& className);
    QTreeWidgetItem* findsymbolInClass(QTreeWidgetItem* classItem, const QString& symbolName);

    // indicates warning, error, etc.  If symbolName not empty, do only class
    void highlightItem(const QString& className, const QString& symbolName = QString(), const QString& msg = QString(),
                       const clang::DiagnosticsEngine::Level &level = clang::DiagnosticsEngine::Level::Ignored);
    void removehighlightedItem(const QString& className, const QString& symbolName = QString());

    // sort items in order user defines
    void sortAsc();
    void sortDesc();
    void sortAsInFile();

private:
    void createWindow();


signals:
    void removeItem(QTreeWidgetItem*);


};

#endif // STRUCTURETREE_H

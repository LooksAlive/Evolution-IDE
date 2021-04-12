#ifndef TEXTNODERELATIONMANAGER_H
#define TEXTNODERELATIONMANAGER_H

/*
 * This class handles text operation between files and nodes
 * NOTE! Files are synchronized, if opened in node, then also in editor
 *
 * Functions, classes, variables, etc. are gained from AST for current translation unit
 * and created nodes.
*/

#include <QTextCursor>

#include <iostream>
#include <vector>

#include "filemanager.h"    // there will be always a read and write, if not synchronized (default)
#include "Protocol.h"   // Range

/*
 * diff ::::: https://www.techiedelight.com/implement-diff-utility/
*/
class ClangBridge;
class PlainTextEdit;
class Node;

class TextNodeRelationManager {
public:
    TextNodeRelationManager();

    ~TextNodeRelationManager() = default;

    // by default if user occupy nodes changes do not affect text editor and so 2. way
    // TODO: change this to false and also updateText()
    bool synchronizeNodeAndTextEditors = true;

    void setCurrentWidget(PlainTextEdit *e) { edit = e; }

    void setClangBridge(ClangBridge *bridge) { clang = bridge; }


    // nodeID related to text range
    struct NodeData {
        clang::clangd::Range range;
        Node *node;
    };

    // ranges for scopes in 1 TU
    // std::vector<Node> scopes;

    struct TUdata {
        std::vector<NodeData> scopes;
        QString filePath;
    };

    std::vector<TUdata> TUScopesData;


    // sets cursor on selected text with original node range, to simplify
    // remove, insert operations
    void cursorWithSelectedText(NodeData *node, QTextCursor &cursor);

    // using clang pointer in current edit * ;  fill TUScopesData
    void createNodesFromAST();

    // FROM text TO node            small changes, otherwise createNodesFromAST() for whole T.U.
    // when synchronizeNodeAndTextEditors is ON
    void updateNode();

    // FROM node TO text
    void updateText(NodeData *node, const bool &scopeChange, const bool &TUchange);

    // remove whole node, text scope
    void removeScope(NodeData *node);

    // format text node, then call updateText() for formated partiotion (scope or more) it depends...
    // update ranges
    void formatNode(NodeData *node);


    // from text
    Node* getNodeIDFromBlock(const QPoint &pos) const;

    // from node         returns block/line
    QPair<QString, int> getTextPostitionFromNode(Node *node) const;


private:
    PlainTextEdit *edit;
    ClangBridge *clang;

};

#endif // TEXTNODERELATIONMANAGER_H

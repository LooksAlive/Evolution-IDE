#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Clang/ClangBridge.h"
#include "TextNodeRelationManager.h"

TextNodeRelationManager::TextNodeRelationManager() {

}

void TextNodeRelationManager::updateText(NodeData *node, const bool &scopeChange, const bool &TUchange) {
    if (!scopeChange) {
        return;
    }
    if (!TUchange) {
        return;
    }
    QTextCursor cursor = edit->textCursor();
    cursorWithSelectedText(node, cursor);
    // original text
    const QString originalText = cursor.selectedText();

    // new text in node gather by invoking node text by its node->nodeID
    const QString newText = "sdf";
    // it might be more or less.
    cursor.removeSelectedText();
    cursor.insertText(newText);

}

void TextNodeRelationManager::cursorWithSelectedText(TextNodeRelationManager::NodeData *node, QTextCursor &cursor) {
    edit->setCursorPosition(cursor, node->range.start.character, node->range.start.line);
    const int linesToMove = node->range.end.line - node->range.start.line;
    if (linesToMove) {
        for (int i = 0; i <= linesToMove; i++) {
            cursor.movePosition(QTextCursor::NextBlock,
                                QTextCursor::KeepAnchor);
        }
    }
    cursor.movePosition(QTextCursor::NextCharacter,
                        QTextCursor::KeepAnchor,
                        node->range.end.character);
}


void TextNodeRelationManager::createNodesFromAST() {
    // clang->
}

void TextNodeRelationManager::updateNode() {

}

void TextNodeRelationManager::removeScope(NodeData *node) {
    QTextCursor cursor = edit->textCursor();
    cursorWithSelectedText(node, cursor);
    const QString newText = "sakjlsdf"; // gather new by node with node->nodeID
    cursor.removeSelectedText();
    cursor.insertText(newText);
}

void TextNodeRelationManager::formatNode(NodeData *node) {
    // range in tooling
    QTextCursor tc = edit->textCursor();
    edit->setCursorPosition(tc, node->range.start.character, node->range.start.line);
    const int startPos = tc.position();

    //clang->formatCode(edit->getFilePath().toStdString(), "gather node text by node->nodeID",
    //                  tooling::Range(startPos, QString("jkasdfh").size())); // get new node text
}


int TextNodeRelationManager::getNodeIDFromBlock(const QPoint &pos) const {
    const QString filepath = edit->getFilePath();

    for (const auto &TUD : TUScopesData) {
        if (TUD.filePath == filepath) {
            for (const auto &SC : TUD.scopes) {
                if (SC.range.contains(clang::clangd::Position{pos.y(), pos.x()})) {
                    return SC.nodeID;
                }
            }
        }
    }
}

QPair<QString, int> TextNodeRelationManager::getTextPostitionFromNode(const int &nodeID) const {
    QString filepath;

    for (const auto &TUD : TUScopesData) {
        if (TUD.filePath == filepath) {
            for (const auto &SC : TUD.scopes) {
                if (SC.nodeID == nodeID) {
                    return QPair(TUD.filePath, SC.range.start.line);
                }
            }
        }
    }
}

#include <QAbstractItemView>
#include "icons/IconFactory.h"
#include <QStringListModel>

#include "completer.h"

Completer::Completer(QObject *parent) : QCompleter(parent) {
    setCaseSensitivity(Qt::CaseInsensitive);
    setMaxVisibleItems(8);
    //setCompletionMode(InlineCompletion);
    setCompletionMode(QCompleter::PopupCompletion);
    setFilterMode(Qt::MatchStartsWith);// MatchContains, for this i have to edit insertion of text
    setWrapAround(false);
    // highlighting completer input html data
    // auto *delegate = new Delegate();
    //auto delegate = new CompleterItemDelegate();
    //popup()->setItemDelegate(delegate);

    // TODO: to implement i need to catch abstractModel signal when data changed get remaining data
    // and update listWidget with them.

    /*
    Popup = new QListWidget();
    Popup->setMouseTracking(true);
    Popup->setStatusTip("statustip");
    Popup->setToolTip("tooltip");
    Popup->hide();
    auto *item = new QListWidgetItem("myitem3");
    item->setIcon(QIcon(IconFactory::Build));
    item->setStatusTip("itemstatustip");
    item->setToolTip("itemtooltip");
    Popup->addItem(item);
    Popup->addItem("nimm");
    Popup->addItem("myitem2");

    setPopup(Popup);
    // new QStringListModel(words, completer)
    setModel(Popup->model());
    auto delegate = new CompleterItemDelegate();
    popup()->setItemDelegate(delegate);
    */

}

void Completer::addItem(const std::string &name, const std::string &returnType, const QIcon &icon) {
    /*
    auto *item = new QListWidgetItem();
    item->setText(QString::fromStdString(returnType + "  " + name));
    item->setIcon(icon);
    item->setToolTip(QString::fromStdString(name));
    item->setStatusTip(QString::fromStdString("status: " + name));
    Popup->addItem(item);
    */
}

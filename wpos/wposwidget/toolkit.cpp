#include "toolkit.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMessageBox>

void selectTreeItemUp(QTreeWidget* tree){
    auto items = tree->selectedItems();
    if( items.isEmpty() ) return;
    auto current = items.first();
    auto previous = current;

    if ( current != tree->itemAt(0,0))
        current = tree->itemAbove(current);

    previous->setSelected(false);
    current->setSelected(true);
    tree->scrollToItem(current);
}

void selectTreeItemDown(QTreeWidget* tree){
    auto items = tree->selectedItems();
    if( items.isEmpty() ) return;
    auto current = items.first();
    auto previous = current;

    if ( current != tree->itemAt( tree->itemAt(0,0)->childCount() - 1, 0))
        current = tree->itemBelow(current);

    previous->setSelected(false);
    current->setSelected(true);
    tree->scrollToItem(current);
}

void selectTreeItemFirst(QTreeWidget* tree){
    auto current = tree->itemAt(0,0);
    auto previous = current;

    previous->setSelected(false);
    current->setSelected(true);
    tree->scrollToItem(current);
}

void selectTreeItemLast(QTreeWidget* tree){
    auto current = tree->itemAt( tree->itemAt(0,0)->childCount() - 1, 0);
    auto previous = current;

    previous->setSelected(false);
    current->setSelected(true);
    tree->scrollToItem(current);
}

void showWarning(QWidget* parent, const QString& caption , const QString& text){
    QMessageBox::warning(parent, caption, text, QMessageBox::NoButton);
}

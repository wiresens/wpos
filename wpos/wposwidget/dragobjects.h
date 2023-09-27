/***************************************************************************
                          bslgenericscreenwidget.h  -  description
                             -------------------
    begin                : Tue Sep 9 2003
    copyright            : (C) 2004 by Napsis S.L.
    email                : carlos@napsis.com
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <QApplication>
#include <QCursor>
#include <QSplitter>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QTableWidget>

class QDragEnterEvent;
class QDragDropEvent;

/*******************************************************
*    Drag and drop listbox
*******************************************************/
class BslDDListBox : public QListView
{
    Q_OBJECT
public:
    explicit BslDDListBox(QWidget * parent = 0, Qt::WindowFlags f = 0 );
    // Low-level drag and drop
    void dragEnterEvent( QDragEnterEvent *evt );
    void dropEvent( QDropEvent *evt );
    void mousePressEvent( QMouseEvent *evt );
    void mouseMoveEvent(QMouseEvent * evt);

signals:
     void textEnter(const QString& name);
private:
    bool dragging;
};

class BslDDIconViewItem : public QListWidgetItem{
public:
    BslDDIconViewItem( QListWidget* parent, const QString& text, const QPixmap& icon ):
                       QListWidgetItem( icon, text, parent ){}

    BslDDIconViewItem(QListWidget* parent, const QString &text ):
                      QListWidgetItem( text, parent ){}

    // High-level drag and drop
    bool acceptDrop(const QMimeData *mime ) const;
    void dropped(QDropEvent* evt);
};

class BslDDIconView : public QListWidget
{
    Q_OBJECT
public:
    explicit BslDDIconView( QWidget * parent = nullptr, const QString& name = QString() ) :
    QListWidget( parent ) {
        setObjectName(name);
        setViewMode(QListView::IconMode);
    }
    // High-level drag and drop
    QDrag *dragObject();
public slots:
    void slotNewItem(QDropEvent *evt);
};

class BslDDTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit BslDDTable( QWidget * parent = nullptr, const QString& name = QString());
    // Low-level drag and drop
    void dragEnterEvent( QDragEnterEvent *evt );
    void dropEvent( QDropEvent *evt );
    void mousePressEvent( QMouseEvent *evt );
    void mouseMoveEvent(QMouseEvent * evt);

    void swapRows(int row1, int row2);

signals:
    void textEnter(const QString& name);
	void textEnter(int x, int y, const QString& name);
	void textEnter(int x, int y);
     
private:
    bool dragging;
};



/***************************************************************************
                          screenstack.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRODUCT_SCREEN_STACK_H
#define PRODUCT_SCREEN_STACK_H

#include <QWidget>
#include <QStackedWidget>
#include <QStringList>
#include "productscreen.h"
#include "product.h"


#include <QMap>

class XmlConfig;
class KDirWatch;

class ProductScreenStack : public QStackedWidget  {
    Q_OBJECT

public:
    explicit ProductScreenStack(const QString& file_name,
                                QWidget *parent=0,
                                const QString& name = QString());

    explicit ProductScreenStack(XmlConfig *xml,
                                QWidget *parent=0,
                                const QString& name = QString());

    explicit ProductScreenStack(QWidget *parent=0,
                                const QString& name = QString());

    ~ProductScreenStack();

    virtual bool initScreenStack(XmlConfig *xml);
    virtual bool initScreenStack(const QString& file_name);
    virtual QString visibleScreenName();

    virtual void setDefaultScreenName(const QString &name);
    virtual QString defaultScreenName();


    int getDefaultSize();
    int getDefaultTextSize();
    QString getDefaultFamily();
    QString getDefaultTextFamily();
    QString getTextBackgroundColor();

public slots:
    bool reset();
    bool remove(const QString& screen_name);
    bool addScreen(const QString& screen_name, XmlConfig *xml);
    bool setScreen(const QString& screen_name);
    bool setDefaultScreen();

    void disableAll();
    void defaultValuesAll();

    void productClickedSlot(Product *prod);
    void deleteProductSlot(Product *prod);
    void combineWithSlot(QStringList list);
    void productDefinitionSlot(XmlConfig *xml);

    void setSpecialModeColor(ProductScreen::ModeColor mode);

    QString nextScreen();
    QString prevScreen();

    void genericDataSignalSlot(const QString& signal_name,XmlConfig *xml);
    void genericSignalSlot(const QString& signal_name);

signals:
    void productDefinition(XmlConfig *xml);
    void defaultValue();
    void splashRequested(const QString &message, int aligment, const QColor &color);

protected:
    Product *selected_product {};
    QMap<QString, ProductScreen*>* screen_dict {};
    QList<ProductScreen*> *screen_list {};

    QString default_screen;
    QString last_file;
    QString last_screen_name;

    QString text_background_color{"#FFFFFF"};
    QString fontfamily{"Arial"};
    QString text_fontfamily {"Arial Black"};
    int fontsize{ 8 };
    int text_fontsize { 10 };
    ProductScreen::ModeColor color_mode {ProductScreen::Normal};
    ProductScreen::ModeColor previous_color_mode {ProductScreen::Normal};
    bool is_color_mode {false};
};

#endif

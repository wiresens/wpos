#ifndef OFFER_CREATION_WIDGET_H
#define OFFER_CREATION_WIDGET_H

#include "offereditionwidget.h"

class OfferCreationWidget : public OfferEditionWidget {
    Q_OBJECT
public:
    explicit OfferCreationWidget(QWidget* parent = 0, const QString& name = QString {});

public slots:
    virtual void clear() override;
    virtual void typeChangedSlot(const QString& type) override;
    virtual void offerChangedSlot(const QString& type) override;
    virtual void acceptSlot() override;

protected:
    virtual bool save() override; // unique
    virtual bool hasAllValues() override;
};

#endif // OFFER_CREATION_WIDGET_H

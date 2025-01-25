#ifndef OPTION_EDITION_WIDGET_H
#define OPTION_EDITION_WIDGET_H

#include "productsmodule/optionsmodule/productoptionmodule.h"
#include "ui_optioneditionwidgetbase.h"

class FloatKeyboard;
class QListWidgetItem;

class OptionEditionWidget : public QWidget,
                            protected Ui::OptionEditionWidgetBase {
    Q_OBJECT

public:
    explicit OptionEditionWidget(QWidget* parent = 0, const QString& name = QString {});
    ~OptionEditionWidget();

public slots:
    void clear();

    void upTypeSlot();
    void downTypeSlot();
    void upOptionSlot();
    void downOptionSlot();

    void typeClickedSlot();
    virtual void acceptSlot();
    virtual void notify() { }

protected:
    virtual void showEvent(QShowEvent* e) override;
    virtual void getTypes();
    virtual void getOptionTypes(const QString& type);

    bool checkAllValues();

private:
    void fillOptions(const QString& domain, const QString& sub_domain, const QString& tag, QComboBox* cbx, const QString& data);

protected:
    FloatKeyboard* float_kb {};
    ProductOptionModule mod;
};

#endif // OPTION_EDITION_WIDGET_H

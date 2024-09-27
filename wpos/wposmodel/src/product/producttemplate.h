#ifndef PRODUCTTEMPLATE_H
#define PRODUCTTEMPLATE_H

#include "persistable.h"
#include "exceptions.h"
#include "uom.h"

using ulong = unsigned long;

namespace wpos{
namespace model{

//class ProductTemplate:  public Persistable<ProductTemplate>{
class ProductTemplate:  public Persistable{
    friend class odb::access;

public:
    using DefaultUom = std::shared_ptr<Uom>;

    ProductTemplate(const ProductTemplate&) = default;
    ProductTemplate& operator=(const ProductTemplate&) = default;
    ~ProductTemplate() = default;

    ProductTemplate(
        DefaultUom uom,
        const string& name,
        const string& code,
        bool consumable = false)
        :uom_{uom},
        code_{code},
        name_{name},
        consumable_{consumable}
    {
        if( name.empty() ) throw EmptyValueException{};
    }

    void setCode(const string& code){ code_ = code; }
    const string& getCode() const { return code_; }

    void setName(const string& name){ if( !name.empty()) name_ = name; }
    const string& getName() const { return name_; }
    const string& code() const { return code_ ; }
    Uom& uom() const { return *uom_; }


    void setImageFile(const string& imageFile){  imageFile_ = imageFile; }
    const string& getImageFile() const { return imageFile_; }

    void activate(){ setActive(true); }
    void deActivate(){ setActive(false); }
    bool isActive() const { return getActive() ; }

    bool isConsumable() const { return getConsumable();}

private:
    ProductTemplate() = default;

    void setActive(bool active){ active_ = active ; }
    bool getActive() const { return active_ ;}

    void setConsumable(bool consumable) { consumable_ = consumable ;}
    bool getConsumable() const { return consumable_ ;}

public:
    const ulong id{0};

private:
    DefaultUom uom_;

    string code_{'\0'};
    string name_;
    string imageFile_;
    bool active_{true};
    bool consumable_{false};
};

}
}

#endif // PRODUCTTEMPLATE_H

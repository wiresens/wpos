#ifndef PRODUCTTEMPLATE_H
#define PRODUCTTEMPLATE_H

#include "../persistable.h"
#include "uom.h"

namespace ws{
namespace model{

//class ProductTemplate:  public Persistable<ProductTemplate>{
class ProductTemplate:  public Persistable{
    friend class odb::access;

public:
    using DefaultUom = std::shared_ptr<Uom>;

    ProductTemplate(const ProductTemplate&) = default;
    ProductTemplate& operator=(const ProductTemplate&) = default;
    ~ProductTemplate() = default;

    ProductTemplate(DefaultUom uom, const string& name, const string& code, bool consumable = false)
        :uom_{uom}, code_{code}, name_{name}, consumable_{consumable}{}

    ulong getId() const{ return id_; }

    void setCode(const string& code){ code_ = code; }
    const string& getCode() const { return code_; }

    void setName(const string& name){ if( !name.empty()) name_ = name; }
    const string& getName() const { return name_; }

    void setImageFile(const string& imageFile){  imageFile_ = imageFile; }
    const string& getImageFile() const { return imageFile_; }

    void activate(){ setActive(true); }
    void deActivate(){ setActive(false); }
    bool isActive() const { return getActive() ; }

    bool isConsumable() const { return getConsumable();}

private:
    ProductTemplate() = default;
    inline void setId(ulong id){  if(id) id_ = id ;}

    void setActive(bool active){ active_ = active ; }
    bool getActive() const { return active_ ;}

    void setConsumable(bool consumable) { consumable_ = consumable ;}
    bool getConsumable() const { return consumable_ ;}

private:
    ulong id_{0};
    DefaultUom uom_;

    string code_;
    string name_;
    string imageFile_;
    bool active_{true};
    bool consumable_{false};
};

}
}

#endif // PRODUCTTEMPLATE_H

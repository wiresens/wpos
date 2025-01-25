/***************************************************************************
                          ProductOptionsInterface.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef H_PRODUCT_OPTION_MODULE_INTERFACE
#define H_PRODUCT_OPTION_MODULE_INTERFACE

#include <QString>

class ProductOptionModuleInterface {
    /**
     *    deleteOption delete the option specificated into the xml
     *    @params QString xml with the properties of the option to delete
     *    @return bool: true if the option was deleted successfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <options>
     *              <option>
     *                   <option_type>option's type</option_type>
     *                   <option_name>option's name</option_name>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool deleteOption(const QString&) = 0;

    /**
     *    deleteOptionType delete the option type specificated
     *    @param QString option's type
     *    @return bool: true if the option's type and all options of this option's type
     *    was deleted successfully
     */
    virtual bool deleteOptionType(const QString&) = 0;

    /**
     *    existOption return true if this option exist into database
     *    @param QString xml with all properties of this option
     *    @return bool: true if this option exist into database
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <options>
     *              <option>
     *                   <option_type>option's type</option_type>
     *                   <option_name>option's name</option_name>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool existOption(const QString&) = 0;

    /**
     *    existOptionType return true if this option's type exist into database
     *    @param QString with option's type
     *    @return bool: true if this option's type exist into database
     */
    virtual bool existOptionType(const QString&) = 0;

    /**
     *    getDescriptionOption return the option's description of the option
     *    especificated into the xml
     *    @param QString xml with option's type and option's name
     *    @return QString description of this option
     *
     *    param:
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <options>
     *                   <option>
     *                        <option_type>option's type</option_type>
     *                        <option_name>option's name</option_name>
     *                   </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getDescriptionOption(const QString&) = 0;

    /**
     *    getDescriptionOptionType return the description of the option's type
     *    specificated for the param
     *    @param QString option's type
     *    @return QString description of the option's type specificated
     */
    virtual QString getDescriptionOptionType(const QString&) = 0;

    /**
     *    getOption return all properties of the option specificated
     *    @param QString xml with option's type and option's name
     *    @return QString xml with all properties of this option
     *
     *    param:
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <options>
     *                   <option>
     *                        <option_type>option's type</option_type>
     *                        <option_name>option's name</option_name>
     *                   </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     *
     *    return:
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <options>
     *              <option>
     *                   <option_type>option's type</option_type>
     *                   <option_name>option's name</option_name>
     *                   <description_option>description of option</description_option>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getOption(const QString&) = 0;

    virtual QString getAllOptions() = 0;

    /**
     *    getOptions return all options of the option's type specificated
     *    @params QString option's type
     *    @return QString: xml with all options of this option's type
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <options>
     *              <option>
     *                   <option_type></option_type>
     *                   <option_name>option1's name</option_name>
     *              </option>
     *              <option>
     *                   <option_type></option_type>
     *                   <option_name>option2's name</option_name>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getOptions(const QString&) = 0;

    /**
     *    getOptionType return a xml with all properties of the specificated option's type
     *    @param QString option's type
     *    @return QString xml with the properties of this option's type
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <options>
     *              <option>
     *                   <option_type>option1's type</option_type>
     *                   <description_type>description option1's type</description_type>
     *                   <option_name></option_name>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getOptionType(const QString&) = 0;

    /**
     *    getOptionTypes return a xml with all option's types
     *    @return QString: xml with all option's types
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <options>
     *              <option>
     *                   <option_type>option1's type</option_type>
     *                   <option_name></option_name>
     *              </option>
     *              <option>
     *                   <option_type>option2's type</option_type>
     *                   <option_name></option_name>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getOptionTypes() = 0;

    /**
     *    insertOption insert into the database the option specificated into this xml
     *    @params QString xml with all option's properties
     *    @return bool: true if the option was inserted succesfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <options>
     *              <option>
     *                   <option_type>option's type</option_type>
     *                   <option_name>option's name</option_name>
     *                   <description_option>option's description</description_option>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool insertOption(const QString&) = 0;

    /**
     *    insertOptionType insert the option's type specificated into the xml of the param
     *    @params QString xml with all properties of the option's type to insert
     *    @return bool: true if the option's type was inserted successfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <options>
     *              <option>
     *                   <option_type>option's type</option_type>
     *                   <description_type>description of option's type</description_type>
     *                   <option_name></option_name>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool insertOptionType(const QString&) = 0;

    /**
     *    updateOption update the option specificated into the xml of the param
     *    @param QString xml with all properties of the option to update
     *    @return bool: true if the option was updated successfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <options>
     *              <option>
     *                   <option_type>option's type</option_type>
     *                   <option_name>option's name</option_name>
     *                   <description_option>option's description</description_option>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool updateOption(const QString&) = 0;

    /**
     *    updateOptionType update the option's type specificated into the xml of the param
     *    @param QString xml with all properties of the option's type to update
     *    @return bool: true if the option's type was updated successfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <product_code></product_code>
     *         <options>
     *              <option>
     *                   <option_type>option's type</option_type>
     *                   <description_type>description of option's type</description_type>
     *                   <option_name></option_name>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool updateOptionType(const QString&) = 0;

    /**
     *    existProductAndOption return true if the product has the option specificated into
     *    the xml of the param
     *    @param QString xml with properties of product and option
     *    @return bool return true if this option has the option specificated
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <product_code>product's code</product_code>
     *         <options>
     *              <option>
     *                   <option_type>option's type</option_type>
     *                   <option_name>option's name</option_name>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool existProductAndOption(const QString&) = 0;

    /**
     *    getProductOptions return all options of the product specificated
     *    @param QString with the product's code
     *    @return QString xml with the properties of product's options
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <options>
     *              <option>
     *                   <option_type>option1's type</option_type>
     *                   <description_type>description of option1's type</description_type>
     *                   <option_name>option1's name</option_name>
     *                   <description_option>option1's description</description_option>
     *                   <value>option1's value</value>
     *                   <default>true if this option1 is default</default>
     *              </option>
     *              <option>
     *                   <option_type>option2's type</option_type>
     *                   <description_type>description of option2's type</description_type>
     *                   <option_name>option2's name</option_name>
     *                   <description_option>option2's description</description_option>
     *                   <value>option2's value</value>
     *                   <default>true if this option2 is default</default>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual QString getProductOptions(const QString&) = 0;

    /**
     *    insertOptionsToProduct insert all options of the xml of the param for the product
     *    specificated (if the option exist, updated this)
     *    @param QString xml with product's code and the properties of all options of this
     *    products
     *    @return bool: true if all option was inserted into the database
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <product_code>product's code</product_code>
     *         <options>
     *              <option>
     *                   <option_type>option1's type</option_type>
     *                   <description_type>description of option1's type</description_type>
     *                   <option_name>option1's name</option_name>
     *                   <description_option>option1's description</description_option>
     *                   <value>option1's value</value>
     *                   <default>true if this option1 is default</default>
     *              </option>
     *              <option>
     *                   <option_type>option2's type</option_type>
     *                   <description_type>description of option2's type</description_type>
     *                   <option_name>option2's name</option_name>
     *                   <description_option>option2's description</description_option>
     *                   <value>option2's value</value>
     *                   <default>true if this option2 is default</default>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool insertOptionsToProduct(const QString&) = 0;

    /**
     *    updateOptionsToProduct update all options from the xml for the product code
     *    specificated (if the option don't exist, create this)
     *    @param QString xml with product's code and all options for this product's code
     *    @return bool: true if all options was updated into the database
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <product_code>product's code</product_code>
     *         <options>
     *              <option>
     *                   <option_type>option1's type</option_type>
     *                   <description_type>description of option1's type</description_type>
     *                   <option_name>option1's name</option_name>
     *                   <description_option>option1's description</description_option>
     *                   <value>option1's value</value>
     *                   <default>true if this option1 is default</default>
     *              </option>
     *              <option>
     *                   <option_type>option2's type</option_type>
     *                   <description_type>description of option2's type</description_type>
     *                   <option_name>option2's name</option_name>
     *                   <description_option>option2's description</description_option>
     *                   <value>option2's value</value>
     *                   <default>true if this option2 is default</default>
     *              </option>
     *         </options>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD  PRODUCT_OPTIONS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (product_code?, options) >
     *    <!ELEMENT product_code (#PCDATA) >
     *    <!ELEMENT options (option*) >
     *    <!ELEMENT option (option_type, description_type?, option_name, description_option?, value?, default?) >
     *    <!ELEMENT option_type (#PCDATA) >
     *    <!ELEMENT description_type (#PCDATA) >
     *    <!ELEMENT option_name (#PCDATA) >
     *    <!ELEMENT description_option (#PCDATA) >
     *    <!ELEMENT value (#PCDATA) >
     *    <!ELEMENT default (#PCDATA) >
     */
    virtual bool updateOptionsToProduct(const QString&) = 0;

    /**
     *  delete option from a product.
     * @param QString product_code
     * @param QString option_type
     * @param QString option_name
     * @return true if the option was deleted
     */
    virtual bool deleteOptionsToProduct(const QString&, const QString&, const QString&) = 0;

    /**
     *    this is a brute force method... it aplies to all products a given offer
     *    the offer should exists.
     *    @param QString Option_type with the offer type
     *    @param QString Option_name, with the offer name
     *    @param QString value with the offer value.
     */
    virtual void setAllProductOptionValue(const QString&, const QString&, const QString&) = 0;

    /**
     *    this is a brute force method... it aplies to all products that already have the offer
     *    a given offer, a new value .The offer should exists.
     *    @param QString Option_type with the offer type
     *    @param QString Option_name, with the offer name
     *    @param QString value with the offer value.
     */
    virtual void updateAllProductOptionValue(const QString&, const QString&, const QString&) = 0;
};

#endif

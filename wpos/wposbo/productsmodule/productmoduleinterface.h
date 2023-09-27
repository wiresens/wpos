/***************************************************************************
                          ProductInterface.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/


#ifndef H_PRODUCT_MODULE_INTERFACE
#define H_PRODUCT_MODULE_INTERFACE

#include <QString>

class ProductModuleInterface{
     /**
     *    deleteProduct delete a product from the database
     *    @params QString with the product's code of the product to delete
     *    @return boolean: return true if the product was deleted
     */
     virtual bool deleteProduct(const QString&)=0;

     /**
     *    getProduct return a xml with the product's properties
     *    @params QString with the product's code of the product to consult
     *    @return QString with the product's properties
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <product>
     *              <code>product's code</code>
     *              <name>product's name</name>
     *              <price>product's price</price>
     *              <tax>product's tax</tax>
     *              <logo>product's logo</logo>
     *              <description>product's description</description>
     *         </product>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCTS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (products)>
     *    <!ELEMENT products (product*) >
     *    <!ELEMENT product (code?, name?, price?, tax?, logo?, description?) >
     *    <!ELEMENT code (#PCDATA) >
     *    <!ELEMENT name (#PCDATA) >
     *    <!ELEMENT price (#PCDATA) >
     *    <!ELEMENT tax (#PCDATA) >
     *    <!ELEMENT logo (#PCDATA) >
     *    <!ELEMENT description (#PCDATA) >
     */
    //only return the name of a product_code
     virtual QString getProductName(const QString&)=0;
     virtual QString getProduct(const QString&)=0;

     /**
     *    getProductLike return the code of the product when the product's name
     *    coincide with the name specificated
     *    @params QString with the product's name to consult
     *    @return QString with the code of this product
     */
     virtual QString getProductLike(const QString&)=0;

     /**
     *    getFamilies return the families into the database
     *    @return QString with a xml with all families
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <families>
     *              <family>family1's name</family>
     *              <family>family2's name</family>
     *         </families>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD FAMILIES:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (families) >
     *    <!ELEMENT families (family*) >
     *    <!ELEMENT family (#PCDATA) >
     */
     virtual QString getFamilies()=0;

     /**
     *    getLogo return the product's code, product's name and product's logo of the product
     *    with product's name equals to param's value
     *    @param QString with the product's name
     *    @return QString xml with product's code, product's name and product's logo
     *
     *    Xml Return:
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <products>
     *              <product>
     *                   <code>product1's code</code>
     *                   <name>product1's name</name>
     *                   <logo>product1's logo</logo>
     *              </product>
     *              <product>
     *                   <code>product2's code</code>
     *                   <name>product2's name</name>
     *                   <logo>product2's logo</logo>
     *              </product>
     *         </product>
     *    </AUTOMATICALY_GENERATED_XML>
     *
     *    DTD PRODUCTS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (products)>
     *    <!ELEMENT products (product*) >
     *    <!ELEMENT product (code?, name?, price?, tax?, logo?, description?) >
     *    <!ELEMENT code (#PCDATA) >
     *    <!ELEMENT name (#PCDATA) >
     *    <!ELEMENT price (#PCDATA) >
     *    <!ELEMENT tax (#PCDATA) >
     *    <!ELEMENT logo (#PCDATA) >
     *    <!ELEMENT description (#PCDATA) >
     */
     virtual QString getLogo(const QString&)=0;
     virtual QString getLogoFromProductCode(const QString&)=0;
     
     /**
     *    getLogo return a ProductData with product's name, product's code and product's logo
     *    @param QString with the list of products' name
     *    @return QString xml with product's code, product's name and product's logo for all
     *    products of the list
     *
     *    Xml Param:
     *    <AUTOMATICALLY_GENEREATED_XML>
     *         <products>
     *              <product>
     *                   <name>product1's name</name>
     *              </product>
     *              <product>
     *                   <name>product2's name</name>
     *              </product>
     *         </products>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCTS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (products)>
     *    <!ELEMENT products (product*) >
     *    <!ELEMENT product (code?, name?, price?, tax?, logo?, description?) >
     *    <!ELEMENT code (#PCDATA) >
     *    <!ELEMENT name (#PCDATA) >
     *    <!ELEMENT price (#PCDATA) >
     *    <!ELEMENT tax (#PCDATA) >
     *    <!ELEMENT logo (#PCDATA) >
     *    <!ELEMENT description (#PCDATA) >
     *
     *    Xml Return:
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <products>
     *              <product>
     *                   <code>product1's code</code>
     *                   <name>product1's name</name>
     *                   <logo>product1's logo</logo>
     *              </product>
     *              <product>
     *                   <code>product2's code</code>
     *                   <name>product2's name</name>
     *                   <logo>product2's logo</logo>
     *              </product>
     *         </product>
     *    </AUTOMATICALY_GENERATED_XML>
     *
     *    DTD PRODUCTS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (products)>
     *    <!ELEMENT products (product*) >
     *    <!ELEMENT product (code?, name?, price?, tax?, logo?, description?) >
     *    <!ELEMENT code (#PCDATA) >
     *    <!ELEMENT name (#PCDATA) >
     *    <!ELEMENT price (#PCDATA) >
     *    <!ELEMENT tax (#PCDATA) >
     *    <!ELEMENT logo (#PCDATA) >
     *    <!ELEMENT description (#PCDATA) >
     */
     virtual QString getLogos(const QString&)=0;
     
     /**
     *    getTaxes return the taxes into the database
     *    @return QString with a xml with all taxes
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <taxes>
     *              <tax>tax1's name</tax>
     *              <tax>tax2's name</tax>
     *         </taxes>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD TAXES:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (taxes)>
     *    <!ELEMENT taxes (tax*) >
     *    <!ELEMENT tax (#PCDATA) >
     */
     virtual QString getTaxes()=0;

          /**
     *    getProducts return string with all products of the database
     *    (unitary products and composition products order by product_name)
     *    @return QString: xml with all product's properties
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <products>
     *              <product>
     *                   <code>product1's code</code>
     *                   <name>product1's name</name>
     *                   <price>product1's price</price>
     *                   <tax>product1's tax</tax>
     *                   <logo>product1's logo</logo>
     *                   <description>product1's description</description>
     *              </product>
     *              <product>
     *                   <code>product2's code</code>
     *                   <name>product2's name</name>
     *                   <price>product2's price</price>
     *                   <tax>product2's tax</tax>
     *                   <logo>product2's logo</logo>
     *                   <description>product2's description</description>
     *              </product>
     *         </products>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCTS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (products)>
     *    <!ELEMENT products (product*) >
     *    <!ELEMENT price (#PCDATA) >
     *    <!ELEMENT tax (#PCDATA) >
     *    <!ELEMENT logo (#PCDATA) >
     *    <!ELEMENT description (#PCDATA) >
     */
     virtual QString getProductsExtend()=0;
     
     /**
     *    getProducts return string with all products of the database
     *    (unitary products and composition products order by product_name)
     *    @return QString: xml with product's code, product's name, product's logo
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <products>
     *              <product>
     *                   <code>product1's code</code>
     *                   <name>product1's name</name>
     *                   <logo>product1's logo</logo>
     *              </product>
     *              <product>
     *                   <code>product2's code</code>
     *                   <name>product2's name</name>
     *                   <logo>product2's logo</logo>
     *              </product>
     *         </products>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCTS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (products)>
     *    <!ELEMENT products (product*) >
     *    <!ELEMENT product (code, name, price?, tax?, logo?, description?) >
     *    <!ELEMENT price (#PCDATA) >
     *    <!ELEMENT tax (#PCDATA) >
     *    <!ELEMENT logo (#PCDATA) >
     *    <!ELEMENT description (#PCDATA) >
     */
     virtual QString getProducts()=0;
     
     /**
     *    getUnitaryProducts return a xml with all unitary products
     *    @return QString with a xml with all unitary products
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <products>
     *              <product>
     *                   <code>product1's code</code>
     *                   <name>product1's name</name>
     *                   <price>product1's price</price>
     *                   <tax>product1's tax</tax>
     *                   <logo>product1's logo</logo>
     *                   <description>product1's description</description>
     *              </product>
     *              <product>
     *                   <code>product2's code</code>
     *                   <name>product2's name</name>
     *                   <price>product2's price</price>
     *                   <tax>product2's tax</tax>
     *                   <logo>product2's logo</logo>
     *                   <description>product2's description</description>
     *              </product>
     *         </products>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCTS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (products)>
     *    <!ELEMENT products (product*) >
     *    <!ELEMENT product (code?, name?, price?, tax?, logo?, description?) >
     *    <!ELEMENT code (#PCDATA) >
     *    <!ELEMENT name (#PCDATA) >
     *    <!ELEMENT price (#PCDATA) >
     *    <!ELEMENT tax (#PCDATA) >
     *    <!ELEMENT logo (#PCDATA) >
     *    <!ELEMENT description (#PCDATA) >
     */
     virtual QString getUnitaryProducts()=0;

     /**
     *    insertProduct insert a product into the database
     *    @params QString with the product's properties to insert
     *    @return bool: true if the product was inserted succesfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <product>
     *              <code>product's code</code>
     *              <name>product's name</name>
     *              <price>product's price</price>
     *              <tax>produt's tax</tax>
     *              <logo>product's logo</logo>
     *              <description>product's description</description>
     *         </product>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCTS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (products)>
     *    <!ELEMENT products (product*) >
     *    <!ELEMENT product (code?, name?, price?, tax?, logo?, description?) >
     *    <!ELEMENT code (#PCDATA) >
     *    <!ELEMENT name (#PCDATA) >
     *    <!ELEMENT price (#PCDATA) >
     *    <!ELEMENT tax (#PCDATA) >
     *    <!ELEMENT logo (#PCDATA) >
     *    <!ELEMENT description (#PCDATA) >
     */
     virtual bool insertProduct(const QString&)=0;

     /**
     *    updateProduct update a product into the database
     *    @params QString with the product's properties to update
     *    @return bool: true if the product was updated succesfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <product>
     *              <code>product's code</code>
     *              <name>product's name</name>
     *              <price>product's price</price>
     *              <tax>produt's tax</tax>
     *              <logo>product's logo</logo>
     *              <description>product's description</description>
     *         </product>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCTS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (products)>
     *    <!ELEMENT products (product*) >
     *    <!ELEMENT product (code?, name?, price?, tax?, logo?, description?) >
     *    <!ELEMENT code (#PCDATA) >
     *    <!ELEMENT name (#PCDATA) >
     *    <!ELEMENT price (#PCDATA) >
     *    <!ELEMENT tax (#PCDATA) >
     *    <!ELEMENT logo (#PCDATA) >
     *    <!ELEMENT description (#PCDATA) >
     */
     virtual bool updateProduct(const QString&)=0;

     /**
     *    getCompositions return a xml with all combinations (products with more
     *    than one ingredient)
     *    @return QString with a xml with all combinations
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <products>
     *              <product>
     *                   <code>product1's code</code>
     *                   <name>product1's name</name>
     *                   <price>product1's price</price>
     *                   <tax>product1's tax</tax>
     *                   <logo>product1's logo</logo>
     *                   <description>product1's description</description>
     *              </product>
     *              <product>
     *                   <code>product2's code</code>
     *                   <name>product2's name</name>
     *                   <price>product2's price</price>
     *                   <tax>product2's tax</tax>
     *                   <logo>product2's logo</logo>
     *                   <description>product2's description</description>
     *              </product>
     *         </products>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD PRODUCTS:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (products)>
     *    <!ELEMENT products (product*) >
     *    <!ELEMENT product (code?, name?, price?, tax?, logo?, description?) >
     *    <!ELEMENT code (#PCDATA) >
     *    <!ELEMENT name (#PCDATA) >
     *    <!ELEMENT price (#PCDATA) >
     *    <!ELEMENT tax (#PCDATA) >
     *    <!ELEMENT logo (#PCDATA) >
     *    <!ELEMENT description (#PCDATA) >
     */
     virtual QString getCompositions()=0;

     /**
     *    insertProductComposition insert a combination into the database
     *    @params QString with a xml with combination's properties
     *    @return bool: return true if the combinations was inserted successfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <composition>
     *              <composition_code>composition's code</composition_code>
     *              <ingredient>
     *                   <ingredient_code>ingredient1's code</ingredient_code>
     *                   <quantity>ingredient1's quantity</quantity>
     *              </ingredient>
     *              <ingredient>
     *                   <ingredient_code>ingredient2's code</ingredient_code>
     *                   <quantity>ingredient2's quantity</quantity>
     *              </ingredient>
     *         </composition>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD COMPOSITION:
      *   <!ELEMENT AUTOMATICALLY_GENERATED_XML (composition)>
     *    <!ELEMENT composition (composition_code, ingredient+) >
     *    <!ELEMENT composition_code (#PCDATA) >
     *    <!ELEMENT ingredient (ingredient_code, quantity) >
     *    <!ELEMENT ingredient_code (#PCDATA) >
     *    <!ELEMENT quantity (#PCDATA) >
    */
     virtual bool insertProductComposition(const QString&)=0;

     /**
     *    updateProductComposition update a combination into the database
     *    @params QString with a xml with the combination's properties
     *    @return bool: return true if the combinations was updated successfully
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <composition>
     *              <composition_code>composition's code</composition_code>
     *              <ingredient>
     *                   <ingredient_code>ingredient1's code</ingredient_code>
     *                   <quantity>ingredient1's quantity</quantity>
     *              </ingredient>
     *              <ingredient>
     *                   <ingredient_code>ingredient2's code</ingredient_code>
     *                   <quantity>ingredient2's quantity</quantity>
     *              </ingredient>
     *         </composition>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD COMPOSITION:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (composition)>
     *    <!ELEMENT composition (composition_code, ingredient+) >
     *    <!ELEMENT composition_code (#PCDATA) >
     *    <!ELEMENT ingredient (ingredient_code, quantity) >
     *    <!ELEMENT ingredient_code (#PCDATA) >
     *    <!ELEMENT quantity (#PCDATA) >
     */
     virtual bool updateProductComposition(const QString&)=0;

     /**
     *    existProductComposition return true if the composition specificated
     *    into the xml param exist into the database
     *    @param QString xml with all properties of this composition
     *    @return bool: return true if the combination exists
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <composition>
     *              <composition_code>composition's code</composition_code>
     *              <ingredient>
     *                   <ingredient_code>ingredient1's code</ingredient_code>
     *                   <quantity>quantity of the ingredient1</quantity>
     *              </ingredient>
     *              <ingredient>
     *                   <ingredient_code>ingredient2's code</ingredient_code>
     *                   <quantity>quantity of the ingredient2</quantity>
     *              </ingredient>
     *         </composition>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD COMPOSITION:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (composition)>
     *    <!ELEMENT composition (composition_code, ingredient+) >
     *    <!ELEMENT composition_code (#PCDATA) >
     *    <!ELEMENT ingredient (ingredient_code, quantity) >
     *    <!ELEMENT ingredient_code (#PCDATA) >
     *    <!ELEMENT quantity (#PCDATA) >
     */
     virtual bool existProductComposition(const QString&)=0;

     /**
     *    getIngredients return string with all ingredients of the product specificated
     *    into the param
     *
     *    @param QString product's code
     *    @return QString xml with all ingredients of this product
     *
     *    <AUTOMATICALLY_GENERATED_XML>
     *         <composition>
     *              <composition_code>composition's code</composition_code>
     *              <ingredient>
     *                   <ingredient_code>ingredient1's code</ingredient_code>
     *                   <quantity>quantity of ingredient1</quantity>
     *              </ingredient>
     *              <ingredient>
     *                   <ingredient_code>ingredient2's code</ingredient_code>
     *              </ingredient>
     *         </composition>
     *    </AUTOMATICALLY_GENERATED_XML>
     *
     *    DTD COMPOSITION:
     *    <!ELEMENT AUTOMATICALLY_GENERATED_XML (composition)>
     *    <!ELEMENT composition (composition_code, ingredient+) >
     *    <!ELEMENT composition_code (#PCDATA) >
     *    <!ELEMENT ingredient (ingredient_code, quantity) >
     *    <!ELEMENT ingredient_code (#PCDATA) >
     *    <!ELEMENT quantity (#PCDATA) >
     */
     virtual QString getIngredients(const QString& product_code)=0;

     /**
     *    getCompositionsWithIngredient return list with all composition that one of this
     *    ingredients are equal to param's value
     *
     *    @param QString with the ingredient's code
     *    @return QString xml with all composition with this ingredient
     *          //FIXME SET THE CORRECT PARAMS
     *
     */
     virtual QString getCompositionsWithIngredient(const QString& ingredient_code)=0;

     /**
     *  this method returns a QString with the product_code of the
     *  product named as the QString first parameter
     *  this method does not return an XML.
     *
     *    @param QString with the product name
     *    @return the product code of the product
     *
     */
     virtual QString getProductCodeByProductName(const QString& product_name)=0;
     virtual int getNumberOfProducts()=0;
     virtual void saveBarXml()=0;

     /**
     * this method set a product at a printer system device.
     * this makes a product available to be printed at kitchen devices.
     */
     virtual void setProductAtPrinter(const QString&, const QString&, bool)=0;
     virtual bool getProductAtPrinter(const QString&,  const QString&)=0;
};

#endif



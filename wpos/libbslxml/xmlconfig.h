/***************************************************************************
                          xmlconfig.h  -  description
                             -------------------
    begin                : mar oct 8 2002
    copyright            : (C) 2002 by Napsis S.L.
    email                : support@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef XMLCONFIG_H
#define XMLCONFIG_H

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomNamedNodeMap>
#include <QtXml/QDomElement>
#include <QtXml/QDomDocumentFragment>
#include <QFile>
#include <QStack>

class String;
class QString;
class QRegExp;
class XmlConfigPrivate;

#define _HAS_VALIDATE_METHOD_

struct XmlDtdValidationException : std::exception{};

/**
    * @short this class give access to the xml files in a silly but easy way
  * @version 0.1.1
    * This class will deal with the configuration stored in the XML file
    * When a class wants to know the configuration stored or wants to store his own
    * it just write and read in this object.
    * The aim of the class is to be very easy to work with.
    * It provides two different APIs , API 1 and API 2 (original names isn't it?)
    * API 1  This is the API that must be used, it's supposed to work
    * The general form of the tag is  "x[n].y[m].z[p]" We look for the string in the xml doc
    * the string must be unique or we will just return the first one we find.
    * The numbers are known thanks to howManyTags();
    * API 2 is the *Simple functions, if they are not needed will be deprecated and deleted
    * from this object
    */

class XmlConfig {

public:
    /**
    * CONSTRUCTOR
    * We make the xml document from the file. So we can close the file, we have all the info
    * in doc, we only deal with Xml clases from now on.
    *
    * @param file File we read the XML from , if the file can't be read or it's not a valid XML
    * file will print an error messague and exit
    *
    * @param mode The mode we open the document. If we open the document in readonly mode we can't
    * save the changes in the same file, so it's safe always to read from a XML without breaking it
    */
    explicit XmlConfig(
        const QString& file_name = QString{},
        QIODevice::OpenMode mode = QIODevice::ReadWrite | QIODevice::Text
    );

    /**
  * CONSTRUCTOR
  * It seems that the standard copy-constructor, even when it's copying the objects
  * Ok, it's not able to copy also the information in the document, so the copy-constructor
  * is used to copy objects and this constructor to copy objects and data.
  * Especial note that the data are in the document, they are not taken from the file,
  * for that purporse there is the constructor above, this means that
  * when save is executed any data in the file (even xml) will be overwritten
  * Example  XmlConfig(conf->domDocument(),conf->fileName(),conf->openMode());
  * will indeed do a copy of the object and data, note that the domain is not set.
  * @param document : the document, taken from other xml calling xml->domDocument()
  * @param file_name: the file we will write the document to
  * @param mode: the mode we'll open the file
  */
    explicit XmlConfig(
        const QDomDocument &document,
        const QString& file_name = {},
        QIODevice::OpenMode mode = QIODevice::ReadWrite | QIODevice::Text
    );

    XmlConfig(
        const QString& xml_cfg_file,
        const QString& dtd_cfg_file,
        QIODevice::OpenMode mode = QIODevice::ReadWrite | QIODevice::Text
    );

    XmlConfig(const XmlConfig&) = delete;
    XmlConfig& operator = (const XmlConfig&) = delete;

    XmlConfig& operator=(XmlConfig&&);
    XmlConfig(XmlConfig&&);

    /**
    * DESTRUCTOR
  * We must delete the document to free our memory and we also must take care of
  * deleting any posible temporal file we were using
    */

    ~XmlConfig();

    void clear();

    /*
  * Damnnn constructors, they don't return anything so we can't know if really we have read
  * something of a file or the file was corrupted or have no XML.
  * If the file is corrupted the tree can't be constructed, and all the methods will fail
  * to check if everything is OK we have this method
  * @return true if correct memory tree false if the object is inconsistent
  */
    bool wellFormed();

    /*
  * this method construct the XmlConfig from a QString, if the string contains a
  * valid xml it will return true.
  * @return true if correct memory tree false if the object is inconsistent
  */
    bool readXmlFromString(const QString& xml_str);
    /*
  * this method construct a QString with all the xml tree
  * @return a QString with the xml tree
  */
    QString toString() const;

#ifdef _HAS_VALIDATE_METHOD_
    bool validateXmlWithDTD(QString path, bool verbose = false);
    //    void validate(const QString& xml_str, const QString& dtdfile){}
    bool validateXmlWithInternalDTD(bool verbose = false);
#endif

    /*
  * This method is a long requested feature. Return the tags that exist in a domain domain
  * The list will have the [x] in the names if the node is not the first so we get:
  * city city[1] city[2] ... this is useful for displaying text in the gui when we know
  * nothing is being repeated
  * @param domain : the domain we want to look for in . DO NOT set the domain
  * @return the list of the node names
  */
    QStringList list( const QString& domain = {});

    /**
    * @param tag It's the tag we look for, we return the number of the tags with this name
    * in the domain
    * @param domain It's the domain we look the tags in. If domain is nullptr, we look in
    * the entire document or in the current jail.
  * Example <disk><partition><partition></disk> <disk><partition></disk>
    * howManyTags ("partition")  returns 3
    * howManyTags("partition","disk[0]") returns 2
    * howManyTags("partition", "disk[1]") returns 1
    * @return  the number of tags with name tag  in the domain domain
    */
    int howManyTags(const QString& tag, const QString& domain = {});

    /**
    * how many atributes has a tag, useful to navigate between the atributes
  * if the tag is null, we will read how many attributes are in the current domain
  * Example <disk number=3 ><partition type=jfs mounted=false><partition></disk>
  * setDomain (disk); howManyAttributes(partition) will return 2
  * setDomain (disk); howManyAttributes() will return 1
    * @param tag in the form "x[n].y[m].z[p]"
    * @return the number of atributes of tag
    */
    int howManyAttributes(const QString& tag = {});

    /**
    * Saves the xml data in a file named file and flush it to disk.
    * If the filename is null it will write the data to the same file it read
    * it from. Note that the constructor of this object needs to be said
    * explicitely that you want to save data with QIODevice::ReadWrite
    * @param file the file we want to save the XML data in
    * @return true if success or false otherwise
    */
    bool save(const QString& file = {});

    /**
    * return the name of the file  we have taken the data from
  * if we save the data in other file, this method will return the name
    * of the new file
    * In short, it returns the name of the representation of the data
    * in disk (surely not updated)
    * @return the name of the file
    */
    QString fileName();

    /**
  * Simply print in stdout what's in the xml right now, it prints the in memory representation
  * Just for debugging purposes, is a bit time consuming so better not leave in a real program
  * @param section the section to debug, to limit the output to a fragment of the XML file
  * @param ident how many identation is used to represent father/child
  */
    void debug(const QString& section = {}, int ident = 0);

    /**
  * @return the mode we are opening the file
  */
    QIODevice::OpenMode openMode();

    //************************************************************************************
    //API 1 PUBLIC
    //************************************************************************************


    //*********************************
    // ELEMENTS
    //*********************************

    /**
    * The general form of the tag is  "x[n].y[m].z[p]" We look for the string in the xml doc
    * the string must be unique or we will just return the first one we find.
    * The numbers are known thanks to howManyTags();
    * TODO?: it's trivial to implement a way to detect a non-unique string so we can return the error but
    * it will require other param
    * @param tag from where we are going to read data
    * @returns the data from the tag with name tag
    */
    QString readString(const QString& tag);

    /**
    * @return the integer in the string, integer is a very common type so it's worthy
  */
    int readInt(const QString& key);

    /**
    * Writes the value value in the tag tag , it's done in memory if you want to modify the
    * file .save() must be called to sync data with disk
    * @param tag the tag where we want to write the value
    * @param value the value to be written
    * @return true if succesfull , false otherwhise (false if the tag don't exist)
    */
    bool write(const QString& tag, const QString& value);

    /**
    *	Same as above, writes a integer in the xml configuration
    * @param tag the tag where we want to write the value
    * @param value the value to be written
    * @return true if succesfull , false otherwhise
    */
    bool write(const QString& tag, int value);

    /**
  * The same that write but if the tag dont exist it will create it and write the value
  * @param tag the tag to be written
  * @param value the value to be written
  * @return true if succesfull false otherwise
  */
    bool doWrite(const QString& tag, const QString& value);

    /**
  * A convenient function, lots of times, we'll write int
  * @param tag the tag to be written
  * @param value the value to be written
  * @return true if succesfull false otherwise
  */
    bool doWrite(const QString& tag, int value);

    /**
    * This will create a new Element in the xml taking the root as the
    * jail we are in , we have the x[].y[].z[] notation again.
    * This will create a <!--comment --><node_name> value </node_name> in the xml doc
    * The comment is absolutely optional
    * The node will be written in the position marked with [n] or in the LAST position
    * if no [n] specified
  * Warning If no value parameter is given you WON'T be able to write a value in this node later
  * you just can add new subelements or attributes
  * Warning, this method will create elements inside elements so createElement(z.z.z.z) will make 4 new
  * elements (assuming no z already existed), but createElement(z[2].z[4].z[5]) will do nothing as it
  * will fail. This method wont do all the z elements required to have z[4]
    * @param node_name the name for this element
    * @param value the value, for convenience, it saves to call write afterwards
    * @param comment the comment that will be written JUST BEFORE, half implemented
    * @return true if success false if fail.
    */
    bool createElement(
        const QString& node_name,
        const QString& value = {},
        const QString& comment = {}
    );

    /**
  * This method do the same that the above method and also set the domain to the recently created element
  * It seems that it's a very used sequence to do this, so i let it in the same method to make it simple.
    * @param node_name the name for this element
    * @param value the value, for convenience, it saves to call write afterwards
    * @param comment the comment that will be written JUST BEFORE, half implemented
    * @return true if success false if fail.
  */
    bool createElementSetDomain(
        const QString& node_name,
        const QString& value = {},
        const QString& comment = {}
    );

    /**
    * This will delete a element <tag> tag value </tag>
    * also note that this can delete a big amount of info as
    * <a> <d>info</d><e>info</e>< <c> info</c></a>  deleting "a" here
    * will delete all the children tags
    * @param tag the tag to be deleted in x.y.z notation
    * @return true if succesful , false if not found
    */
    bool deleteElement(const QString& tag);

    //*********************************
    // ATTRIBUTES
    //*********************************

    /*
  * This will read the name of a attribute according with it's position in the tag
  * To know how many attrs are in the tag we can call howManyAttributes
  * @param tag the tag were we look for the attributes
  * @attr the attr number we want to get the name
  * @return the attr name
  */
    QString readAttributeName(const QString& tag, int attr);

    /**
  * Will read the attribute of a tag
  * This method is also useful to check if a tag has an attribute
  * if (c->readAttribute(tag,attr)=={}) will check if the attribute exists
  * @param tag the tag we want to read the attribute from
  * @param attr the name of the attribute we want to read the value
  * @return the value of the attribute or null if not found
  */
    QString readAttribute(const QString& tag, const QString& attr);

    /**
  * Will write the value of an attribute
  * @param tag the tag that has the attribute
  * @param attr the name of the attribute to write
  * @param value the value of this attribute
  * @return false if we can't write the value, this can happen if the tag don't
  * exist or the attribute don't exist
  */
    bool writeAttribute(
        const QString& tag,
        const QString& attr,
        const QString& value
    );

    /**
  * Will delete an attribute from a tag
  * @param tag the tag that has the attribute
  * @param attr the attribute to delete
  * @return false if tag or attr not found true otherwise
  */
    bool deleteAttribute(const QString& tag, const QString& attr);

    /**
  * Will create a new attribute for this tag, note that an element can't have two elements
  * with the same name, so if you try to create an element that already exist this
  * method will overwrite it. So it can be used as "i really meant to write" because
  * it writes to the attribute or create a new one and writes
  * @param tag the tag that has the attribute
  * @param attr the atribute to create
  * @param value the value to write to the attribute just created , note that the default
  * value is null, this + overwrite  can make you put your attr to null if you don't check
  * @return false if the tag wasn't found or true otherwise
  */
    bool createAttribute(
        const QString& tag,
        const QString& attr,
        const QString& value = {}
    );

    /**
  * Will create a new attribute in the current domain, done for convenience
  * it behaive exactly as the above method but tag is the current domain.
  * this make easier create attributes for newly create elementes
  * @param attr: the attribute to create
  * @param value the value to write to the attribute just created
  * @return false there's some problem with the curren domain , true if created
  */
    bool createAttributeHere(
        const QString& attr,
        const QString& value = {}
    );

    //*********************************
    // DOMAIN
    //*********************************

    /**
    * Limit the scope of all the operations to the domain in @param domain
    * This make the reads/writes more efficient but are not in any way
    * necesary so you can just ignore setDomain and releaseDomain
    * in your application
    * note that as you enter in a jail when using this function you must
    * take care about exiting from it using releaseDomain
    * you can anidate domains, setDomain(a) ; setDomain (b) will set the domain to
    * a.b and so on
    * note that you can say if the domain you are setting is global (relative to the document)
    * or relative to the current domain, so :
    * setDomain("a.b.c", true); is the same that
    * setDomain("a"); setDomain(b); setDomain(c);
    * @param domain  domain in simple x[n].y[m].z[p] notation
    * @param overwrite_domain true if you set a new domain , false if the domain is relative to the old domain
  * @return true if sucessfull , important check this to be sure you are in a domain
    */
    bool setDomain(
        const QString& domain,
        bool overwrite_domain = false
    );

    /**
  * we want to enter in that domain, if this domain don't exist we'll create it
  * so if this method return false really awful things are happening
    * @param domain  domain in simple x[n].y[m].z[p] notation
    * @param overwrite_domain true if you set a new domain , false if the domain is relative to the old domain
  * @return true if sucessfull , important check this to be sure you are in a domain
  * @see setDomain
  */
    bool doSetDomain(const QString& domain, bool overwrite_domain = false);

    /**
    * Just release the jail the setDomain() enter us in, read the
    * setDomain comment
    * the domain must be a simple string no need to specify [] as we take
    * care of releasing just what was set.
  * This function has 2 behaviour controled by the recursive parameter
  * If recursive is true (default) the wider domain that match the name will be freed,
    * for instance:
  <PRE>
    * setDomain ("disk[1].partition[2].partition.geometry")
    * releaseDomain("geometry"); will let the domain set to disk[1].partition[2].partition
    * releaseDomain("partition");  will let the domain set to disk[1]
    * releaseDomain("disk");  will eliminate the jail at all
  * indenpendly of the order the function was called in
  *
  </PRE>
  * If the recursive parameter is set to false, the smaller domain possible will be
  * freed, it's very useful when we have several nodes anidated, for instance:
  <PRE>
    * setDomain ("disk[1].partition[2].partition.geometry")
    * releaseDomain("geometry"); will let the domain set to disk[1].partition[2].partition
    * releaseDomain("partition",false);  will let the domain set to disk[1].partition[2]
    * releaseDomain("disk");  will eliminate the jail at all
  * indenpendly of the orden the function was called in
  *
  </PRE>
  * so really the behaviour only changes in cases when we anidate tags
    * @param domain the domain to be released
  * @param recursive indicates if the domain will be freed recursively
    * @return true if successful
    */
    bool releaseDomain(const QString& domain, bool recursive = true);

    /**
  * AGGGGGGHHHHHHHHHHH
  * This method must be named releaseDomain but DAMN the little green gnome of the office
  * is doing nasty things with the virtual tables. When releaseDomain (string) is called
  * this function is called!! Amazing but true, so we changed the name, it sucks but...
  * Program really MUST take care were they are but just in case they got lost
  * they have this method that is given for convenience, it has 2 behaviour:
  * ----if recursive is set to false it just go out of the current domain (go to the
  *     superior domain (the thing before the .)
  * ----if recursive is set to true it just release all domains.
  * @param recursive indicate if we release the domain recursively or no
  * @return true if possible. False if for example there's no domain set
  */
    bool delDomain (bool recursive = true);

    /**
  * @return the current domain
  */
    QString getDomain();

    /*
  * saves and restores the current domain
  */
    void pushDomain();  //saveCurrentDomain()
    void popDomain();   //restoreCurrentDomain()

    //*********************************
    // ADVANCED
    //*********************************

    /**
  * Copy a certain fragment in a xmlconfig to other one
  * copy (x, y.z , m.n)   will copy what's inside y.z (no y either z) inside m.n
  * that can be data, comentaries, elements...
  * they will copy it to m.n if m.n don't exist will create
  * m inside it will create n and inside it will copy all the data
  * Of course everything works fine with the x[].y[].z[] notation and we are inside domain
  * if some param is missing (or is "") the current domain will be used
  * Will create a new node with the name and write the contents inside it
  * It will allways create a new node even if its a node with that node name
  * for instance:
  * copy (src, "x.y[2]" "y[2]") will create a new node in the position y[2] and
  * will copy the data inside it. The former y[2] will become y[3]
  * @param xml_src the object we'll read data from
  * @param src the node we take the data from
  * @param dst the node we write to
  * @return true in success
  */

    bool copy (
        XmlConfig *xml_src,
        const QString& src = {},
        const QString& dst = {}
    );

    /**
  * When you copy to a element that already exist it will copy
  * the contents inside it. If it doesn't exist will create it.
  * for instance:
  * copy (src, "x.y[2]", "x")  will mergue inside y[2] if
  * it exist or will create it otherwise.
  * @param xml_src the object we'll read data from
  * @param src the node we take the data from
  * @param dst the node we write to
  * @return true in success
  */
    bool merge (
        XmlConfig *xml_src,
        const QString& src = {},
        const QString& dst = {}
    );

    /**
  * Move a certain fragment in a xmlconfig to other one
  * Move (x, y.z , m.n)   will move what's inside y.z (no y either z) inside m.n
  * that can be data, comentaries, elements...
  * it behaive the same that above but will delete recursively z
  * @param xml_src the object we'll read data from
  * @param src the node we take the data from and delete elements
  * @param dst the node we write to
  * @return true if successfull
  */
    bool move (
        XmlConfig *xml_src,
        const QString& src = {},
        const QString& dst = {}
    );

    /**
  * Will search a tag inside the tree and will return the list of the nodes found
  * that contains an element with that name, useful when we don't want to make
  * several for (){}; to find if out elements has other elements in them
  * If you want to get the full path not just the path relative to the current domain <br>
  * getDomain()+"."+findNode()
  * @param tag the name of the node for instance filesystem and will return what partitions
  * has an element named partition
  * @param domain the domain we search in
  * @param recursive if true we will find this tag recursively else we look for only in this level
  * @return the list of the nodes that has the tag searched,  <BR>
  * <disk> <partition> </mountpoint></partition></partition><partition></mountpoint></partition></disk> <BR>
  * conf->setDomain(disk); <BR>
  * conf->findNode("mountpoint"); returns partition, partition[2] <BR>
  * if the stringlist.count()==0 nothing was found , note that null can be returned as value
  * because the node can be children of the root node.
  */
    QStringList findNode(
        const QString& tag,
        const QString& domain = {},
        bool recursive = true
    );

    /*
  * Overloaded function for convenience, it look for a regular expresion, why we use 2 method
  * may be not obvious, but we have to keep in mind that the xml work must be as automatic as possible
  * the application must know as little as possible so maybe we'll end up searching for something we don't
  * know what's but we reading from a file, if it something like "aa*bb" maybe se want to search for that
  * literal string instead of the regular expresion, that why the overloaded method is provided, so the
  * programmer can control if the param will be taken as literal or as regexp
  */
    QStringList findNode(
        const QRegExp& tag,
        const QString& domain = {},
        bool recursive = true
    );

    /**
  * Will search for a value inside the domain and will return a list of nodes found
  * that has as value this value. The value will be interpreted literal, for more control
  * look at the next method <br>
  * <ein> <ein2> hello</ein2></ein>
  * findValue("hello") will return ein.ein2<br>
  * findNode("ein2") will return ein
  * @param value the value to look for in the document
  * @param domain the domain we look for
  * @return a list with all the elements that has inside the value
  */
    QStringList findValue(
        const QString& value,
        const QString& domain = {}
    );

    /*
  * Overloaded function provide for convenience, @see findValue
  */
    QStringList findValue(
        const QRegExp& value,
        const QString& domain = {}
    );

    /*
  * return the nodes that with a specific value
  * findNodeValue("path","/dev/hda2","disk.partition" );
  * will return the partitions with the path that has /dev/hda2
  *
  */
    QStringList findNodeValue(
        const QString& node,
        const QString& value,
        const QString& domain
    );

    // This will be implemented when useful
    //  QStringList findAttrNode(const QString& attr, const QString& domain);
    //  QStringList findAttrValue(const QString& attr_value, const QString& domain);


    /**
    * It will create a comment just over the tag tag
    * the comments are not parsed with this API, so you can't change
    * nor delete them (not yet)
    * @param tag the tag over where we put the comment
    * @param comment the comment we want to write
    * @return true if succesful , false otherwise
  */
    bool createComment(
        const QString& tag,
        const QString& comment
    );

    /*******
* Utilities
****************/

    /**
  * It returns the document, it's not supposed the programs above will need it
  * but just in case, and it also simplifies the creation of copy and move methods
  * WARNING: If you use this method you have to know what are you doing
  * @return the qt representation of the document
  */
    const QDomDocument &getDocument() const;

private:

    //************************************************************************************
    //API 1 PRIVATE
    //************************************************************************************


    /**
    * This method finds a node from a tag  name. It understand the x[n].y[m].z[p]
    * thing and will be used everywhere
    * @param tag the tag we want to find in this document
    * @return the node in the document with this tag as name
    **/
    QDomNode nodeFromTagPrivate(const QString& tag);

    /**
    * Helper function  for nodeFromTag. It just return the first node found.
    * We first look all the siblings and then begin to enter in the next level so
    * in the end we must have walked the entire tree. It's not very efficent for now.
    * @param tag the tag name we are looking for
    * @param from we start looking in this node "inwards"
    * @param anidate if true we'll search between the children, if false we'll search between the siblings
    * @return the first node with this node found
  **/
    QDomNode findPrivate(
        const QString& tag,
        const QDomNode& from,
        bool anidate = true
     );

    /**
    * Yet another helper method, it accepts a string  token[number]
    * and returns token and number with no [ ].
    * @param token is the QString that we want to parse
    * @param number is the number inside the brackets
    * @return  the parsed string
    **/
    QString discardBracketPrivate(QString token, int *number);


    /**
  * Other helper method for create, it can be merged with the next one, it's used so when you
  * want to create a.b.c.d.e and you have already a.b it will create c.d.e
  */
    bool createElementRecursivePrivate(
        const QString& node_name,
        const QString& value = {},
        const QString& comment = {},
        bool set_domain = false
    );

    /**
  * This is a helper method, it does the real job of createElement and createElementSetDomain
  * Read the comments there.
  */
    bool createElementPrivate(
        const QString& node_name,
        const QString& value = {},
        const QString& comment = {},
        bool set_domain = false
    );

    /**
  * This is the method used by copy, merge and move, it just copy or merges depending on the
  * merge parameter if merge is true it merges else it copies
  */
    bool copyPrivate (
        XmlConfig *xml_src,
        const QString& src = {},
        const QString& dst = {},
        bool merge = false
    );

    /*
  * This method get a node from the DOM tree and returns the string
  * in x[m].y[n] representation of this node. This is very convenient
  * when playing with domains
  * @param  node the node from which we want to know the string
  * @return the string of the node
  */
    QString tagFromNodePrivate(const QDomNode& node);

    /*
  * It returns the node from this domain but don't set it , this is a crap, im sure i can make
  * it so i share this code.......
  */
    QDomNode temporyDomainPrivate (const QString& domain );

private:
    XmlConfigPrivate   *m_impl{};
    QDomDocument        m_dom;  //the document pointing to our data
    QDomNode            m_node; //the current node , why is it not a pointer?
    QDomNode            m_domain;      //the current domain , why is it not a pointer?
    QString             m_str_domain; //the domain in a string representation
    QStack<QString>     m_stacked_str_domain;
};

#endif

/***************************************************************************
                          xmlconfig.cpp  -  description
                             -------------------
    begin                : Sat Apr 6 2002
    copyright            : (C) 2002 by Napsis S.L
    email                : jordi@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "xmlconfigprivate.h"
#include "xmlconfig.h"
#include "utils.h"

#include <QString>
#include <QStringList>
#include <QRegExp>

#define _HAS_VALIDATE_METHOD_

#include <libxml2/libxml/xmlmemory.h>
#include <libxml2/libxml/xmlversion.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/valid.h>

#include <iostream>
using std::cerr;

#ifdef _HAS_VALIDATE_METHOD_
/**
*    QT does not admit DTD validation, and we really need it... this libraray is goint to be reimplemented in the future
*    to use libxml2, but for now we will use that lib to make the validation and may be other funzy actions with the xml
*/
#include <libxml2/libxml/xmlmemory.h>
#include <libxml2/libxml/xmlversion.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/valid.h>

#endif


/**
Each page in the instalation process will request this object about the configuration in the
startup so we can make custom instalations if the data they request is empty then just
no configuration has been made so they work as they like.

The main problem here is the anidation, we need this in order to make a good XML file but
it seems that make the class deals with the file format, and we really don't wants that.
The "solution" is this:
The application can write/read wherever it likes and the behaivour or the object will change depending
of where it's writing on, so the problem go to xmlconfig (where it belongs)
When writing/reading in a tag with a value in it it just do it.
If the tag contents other tags but no a value it anidate (no write /read nothing)
so now we can access the new domfragment, inside this if we attempt to get data from outside it
will go out of the fragment again. So it result a semitransparent method
*/

XmlConfig::XmlConfig(
    const QString& _file,
    QIODevice::OpenMode mode):
    impl { new XmlConfigPrivate(_file, mode)}
{
    if (impl->ok()){
        doc = impl->domDocument();
        currentNode = doc.documentElement().firstChild();
    }

}

XmlConfig::XmlConfig(
    const QDomDocument &document,
    const QString& _file,
    QIODevice::OpenMode mode):
    impl {new XmlConfigPrivate(document, _file, mode)}
{

    if (impl->ok()){
        doc = impl->domDocument();
        currentNode = doc.documentElement().firstChild();
    }
}

XmlConfig::XmlConfig(
    const QString& xml_cfg_file,
    const QString& dtd_cfg_file,
    QIODevice::OpenMode mode):
    XmlConfig(xml_cfg_file)
{
    if ( !wellFormed() ||
        !validateXmlWithDTD(dtd_cfg_file, true)
    )
        throw XmlDtdValidationException{};
}

XmlConfig& XmlConfig::operator=(XmlConfig&& xml){
    if(impl != xml.impl) delete impl;

    impl            = xml.impl;
    doc             = xml.doc;
    currentNode     = xml.currentNode;
    domain          = xml.domain;
    string_domain   = xml.string_domain;
    stack_domain    = xml.stack_domain;

    xml.impl = nullptr;
    return *this;
}

XmlConfig::XmlConfig(XmlConfig&& xml)
    :impl{xml.impl}, doc{xml.doc},
    currentNode{xml.currentNode},
    domain{xml.domain},
    string_domain{xml.string_domain},
    stack_domain{xml.stack_domain}

{
    xml.impl = nullptr;
}

XmlConfig::~XmlConfig(){
    delete impl;
}

void XmlConfig::clear(){
    *this = XmlConfig();
}


//************************************************************************************
//PUBLIC
//************************************************************************************

bool XmlConfig::readXmlFromString(const QString& xml_str)
{
    QDomDocument tmpDoc;

    if (!tmpDoc.setContent(xml_str)) return false;
    QString fileName = impl->fileName();
    delete impl;
    impl = new XmlConfigPrivate{tmpDoc, fileName, QIODevice::ReadWrite};
    if (impl->ok()){
        doc = impl->domDocument();
        currentNode = doc.documentElement().firstChild();
    }

    string_domain.clear();
    return true;
}

QString XmlConfig::toString() const{
    return impl->toString();
}

#ifdef _HAS_VALIDATE_METHOD_
bool XmlConfig::validateXmlWithDTD(QString path, bool verbose){
    bool ret;
    xmlDocPtr xml2_document;
    xmlDtdPtr dtd;
    xmlValidCtxtPtr cvp;
    QString aux_string;

    //check if our xml is correct.
    if ( !wellFormed() || toString().isEmpty())
        return false;

    aux_string = toString().toUtf8();
    char *buffer = new char[aux_string.length()];
    memcpy(buffer, aux_string.toLatin1(), aux_string.length());
    xml2_document = xmlParseMemory(buffer,aux_string.length());
    delete[] buffer;

    if (!xml2_document){
        //problems obtaining the document at the xmllib
        return false;
    }
    //delete the document type if it already has anyone.
    dtd = xmlGetIntSubset(xml2_document);
    if (dtd){
        //there is an active document type (//should not happens by now)
        xmlUnlinkNode((xmlNodePtr) dtd);
        xmlFreeDtd(dtd);
    }
    dtd = xmlParseDTD(nullptr, (xmlChar *)(const char*) path.toLatin1());
    if (!dtd){
        //wrong dtd file;
        xmlFreeDoc(xml2_document);
        return false;
    }

    if ( ! (cvp = xmlNewValidCtxt() ) ){
        //can't allocate validation context.
        xmlFreeDoc(xml2_document);
        return false;
    }

    //set validation context.
    if (verbose){
        cvp->userData = (void *) stderr;
        cvp->error = (xmlValidityErrorFunc) fprintf;
        cvp->warning = (xmlValidityWarningFunc) fprintf;
    }
    ret = (bool) xmlValidateDtd(cvp,xml2_document,dtd);

    xmlFreeValidCtxt(cvp);
    xmlFreeDtd(dtd);
    xmlFreeDoc(xml2_document);

    return ret;
}

bool XmlConfig::validateXmlWithInternalDTD(bool verbose){
    bool ret;
    xmlDocPtr xml2_document;
    xmlDtdPtr dtd;
    xmlValidCtxtPtr cvp;
    QString aux_string;

    //check if our xml is correct.
    if (   wellFormed() || toString().isEmpty() ) return false;
    aux_string = toString().toUtf8();
    char *buffer = new char[aux_string.length()];
    memcpy(buffer,aux_string.toLatin1(),aux_string.length());
    xml2_document = xmlParseMemory(buffer,aux_string.length());
    delete[] buffer;
    if (!xml2_document){
        //problems obtaining the document at the xmllib
        return false;
    }
    //delete the document type it it already has anyone.
    dtd = xmlGetIntSubset(xml2_document);
    if (!dtd){        //there is an active document type (//should not happens by now)
        xmlFreeDoc(xml2_document);
        return false;
    }

    if (!(cvp = xmlNewValidCtxt())){
        //can't allocate validation context.
        xmlFreeDoc(xml2_document);
        return false;
    }
    //stablish validation context.
    if (verbose){
        cvp->userData = (void *) stderr;
        cvp->error = (xmlValidityErrorFunc) fprintf;
        cvp->warning = (xmlValidityWarningFunc) fprintf;
    }

    ret = (bool) xmlValidateDocument(cvp,xml2_document);
    xmlFreeValidCtxt(cvp);
    xmlFreeDtd(dtd);
    xmlFreeDoc(xml2_document);

    return ret;
}

#endif

bool XmlConfig::wellFormed(){
    return ( impl->ok());
}

QStringList XmlConfig::list( const QString& domain){

    QStringList result;
    QString name;
    int how_many=0;

    QDomNode i = temporyDomainPrivate(domain);

    // FIXME this can be made faster

    while( !i.isNull() ){
        QDomElement temp = i.toElement(); // try to convert the node to an element.
        name=temp.nodeName();
        how_many=result.contains(name); //if its the first time we put it in the list comes without []
        if (how_many){    //how_many allways 0/1  the next times we put  [] to let the program easily use this
            do{         //if there's more than one, we check [1], [2] ...till the first available
                name = temp.nodeName();
                name += "[";
                name += QString::number(how_many);
                name += "]";
                how_many++;
            }
            while (result.contains(name)); //if result contains name we need a higher one
        }

        result += name;
        i = i.nextSibling();
    }
    return(result);
}

int XmlConfig::howManyTags(const QString& tag, const QString& domain){
    int how_many{0};
    QDomNode node = temporyDomainPrivate(domain);

    //till here shared code
    while( !node.isNull()){
        if( String::compare(node.nodeName(), tag.toLower()))
            how_many++;
        node = node.nextSibling();
    }

    return how_many;
}

int XmlConfig::howManyAttributes(const QString& tag){
    QDomElement elmt = nodeFromTagPrivate(tag).toElement();

    if (elmt.isNull())
        return 0;
    else
        return elmt.attributes().length();
}

bool XmlConfig::save(const QString& file){
    return impl->save(file);
}

QString XmlConfig::fileName(){
    return impl->fileName();
}

void XmlConfig::debug(const QString& section, int ident){

    if (section.isNull() || section.isEmpty())
        impl->debug(doc, ident);
    else
        impl->debug(temporyDomainPrivate(section), ident);
}

QIODevice::OpenMode XmlConfig::openMode(){
    return impl->openedMode();
}


//************************************************************************************
// Information access APIs
//************************************************************************************


/**We have 2 API, i hope the first one is the one we'll use allways, but we have
a second API just in case it sucks works with the first one
The second one will die if we don't use it so please use the functions:
readString readInt writeString writeInt over
*Simple methods.
*/
//************************************************************************************
//API1
//************************************************************************************


//*********************************
// ELEMENTS
//*********************************


QString XmlConfig::readString(const QString& tag){
    return nodeFromTagPrivate(tag).firstChild().toText().nodeValue();
}

int XmlConfig::readInt(const QString& tag){
    return readString(tag).toInt();
}

bool XmlConfig::write(const QString& tag,const QString& value){

    bool result = false;
    QDomNode where = nodeFromTagPrivate(tag);

    if (!where.firstChild().toText().isNull()){
        where.firstChild().toText().setNodeValue(value);
        result = true;
    }

    return result;
}

bool XmlConfig::write(const QString& tag, int value){
    QString t;
    return write(tag,t.setNum(value));
}

bool XmlConfig::doWrite(const QString& tag, const QString& value){

    bool result=true;
    //we can't write, maybe because it don't exist or because is a element with child element instead of data
    if( !write(tag,value)){ //if (this->howManyTags(tag)==0) FIXME
        result = createElementRecursivePrivate(tag, value, nullptr, false);
    }

    return result;
}

bool XmlConfig::doWrite(const QString& tag,int value){
    QString t;
    return doWrite(tag,t.setNum(value));
}

bool XmlConfig::createElement(const QString& node_name, const QString& value, const QString& comment){
    return createElementRecursivePrivate(node_name, value,comment,false);
}

bool XmlConfig::createElementSetDomain(const QString& node_name, const QString& value, const QString& comment){
    return createElementRecursivePrivate(node_name, value,comment,true);
}

//*********************************
// ATTRIBUTES
//*********************************

QString XmlConfig::readAttributeName(const QString& tag,int attr){

    QString result;
    QDomElement dom_elmt = nodeFromTagPrivate(tag).toElement();

    if (!dom_elmt.isNull())
        result = dom_elmt.attributes().item(attr).nodeName();

    return result;
}

QString XmlConfig::readAttribute(const QString& tag,const QString& attr){

    QString result;
    QDomElement dom_elt = nodeFromTagPrivate(tag).toElement();

    if (!dom_elt.isNull())
        result = dom_elt.attribute(attr);

    return result;
}

bool XmlConfig::writeAttribute(const QString& tag,const QString& attr,const QString& value){

    bool result { false};
    QDomElement dom_elmt = nodeFromTagPrivate(tag).toElement();

    if (!dom_elmt.isNull() && dom_elmt.hasAttribute(attr)){
        dom_elmt.setAttribute(attr, value);
        result = true;
    }
    return result;
}

bool XmlConfig::deleteAttribute(const QString& tag,const QString& attr){

    bool result { false};
    QDomElement dom_elmt = nodeFromTagPrivate(tag).toElement();

    if (!dom_elmt.isNull() && dom_elmt.hasAttribute(attr) ){
        dom_elmt.removeAttribute(attr);
        result = true;
    }
    return result;
}

bool XmlConfig::createAttribute(const QString& tag,const QString& attr, const QString& value){

    bool result {false};
    QDomElement e = nodeFromTagPrivate(tag).toElement();
    if (!e.isNull()){
        e.setAttribute(attr,value);
        result = true;
    }

    return result;
}

bool XmlConfig::createAttributeHere(const QString& attr, const QString& value){

    bool result {false};
    QDomElement e = domain.toElement();

    if (!e.isNull()){
        e.setAttribute(attr,value);
        result=true;
    }

    return result;
}

//*********************************
// DOMAINS
//*********************************

bool XmlConfig::doSetDomain(const QString& domain, bool overwrite_domain){

    bool result{ true};

    if (!setDomain(domain,overwrite_domain)){
        createElement(domain, QString(), nullptr);
        if(!setDomain(domain, overwrite_domain))
            result = false;
    }

    return result;
}

//They only limit the scope of our search so we don't have to deal with the entire doc
//This is much like the *Simple API so we can have the benefits of one and the other
bool XmlConfig::setDomain(const QString& domain, bool overwrite_domain){
    bool result = true;
    QDomNode temp;

    if (overwrite_domain){  //just delete the current domain and make a new one
        this->domain = temp;
        this->domain = nodeFromTagPrivate(domain);
        if (!this->domain.isNull())
            string_domain = domain;
        else
            result=false;
    }
    else{
        QDomNode temp = this->nodeFromTagPrivate(domain); //as we are in a jail we have a path relative to the domain
        if (!temp.isNull()){
            this->domain = temp;
            if ( !string_domain.isEmpty() )  //if already we have a string domain we put the . after what we
                string_domain += "."; //have already
            string_domain += domain;
            if (string_domain.at(0) == '.')
                string_domain = string_domain.right(string_domain.length()-1);
        }
        else
            result = false;
    }

    return result;
}

bool XmlConfig::releaseDomain(const QString& domain, bool recursive){
    QDomNode temp = this->domain;

    if (!temp.isNull()){
        if (!recursive){ //not recursive, once it mach we return
            while (!String::compare(temp.nodeName(), domain)){
                if ( temp == doc.documentElement() )
                    return false;

                temp = temp.parentNode();
                string_domain = String::stringOff(string_domain);
            }

            this->domain = temp.parentNode(); //I hope this will work. and won't change the dom tree
            string_domain = String::stringOff(string_domain);
            return true;
        }

        if (recursive){ //we want to free has much as possible, we look till the end where match further
            int times = 0, i = 1;
            while( temp!=doc.documentElement()){
                if (String::compare(temp.nodeName(), domain))
                    times = i;
                temp = temp.parentNode();
                i++;
            }

            if (!times) //we are not in that domain
                return false;
            while ( times >=1 ){
                this->domain = this->domain.parentNode();
                string_domain=String::stringOff(string_domain);
                times--;
            }
            return true;
        }//recursive
    }//temp!=null
    return false;
}

bool XmlConfig::delDomain (bool recursive){
    bool result = false;
    if ( !domain.isNull() ){
        if (recursive){
            domain = doc.documentElement();
            string_domain = QString();
        }
        else
            if (domain != doc.documentElement())
            {
                domain = domain.parentNode();
                string_domain = String::stringOff(string_domain);
            }
        result = true;
    }

    return result;
}

QString XmlConfig::getDomain(){
    return string_domain ;
}

void XmlConfig::pushDomain(){
    stack_domain.push(string_domain);
}

void XmlConfig::popDomain(){
    if( stack_domain.isEmpty() ) return;
    string_domain = stack_domain.pop();
    domain = doc.documentElement();
    domain = nodeFromTagPrivate(string_domain);
}

//*********************************
// ADVANCED
//*********************************

bool XmlConfig::merge (
    XmlConfig *xml_src,
    const QString& src,
    const QString& dst)
{
    return copyPrivate(xml_src, src, dst, true);
}

bool XmlConfig::copy (
    XmlConfig *xml_src,
    const QString& src,
    const QString& dst)
{
    return copyPrivate(xml_src, src, dst, false);
}

bool XmlConfig::move (
    XmlConfig *xml_src,
    const QString& src,
    const QString& dst)
{
    bool result=false;
    if (copy (xml_src,src,dst))
    {
        xml_src->deleteElement(src);
        result=true;
    }
    return result;
}

QStringList XmlConfig::findNode(
    const QString& tag,
    const QString& domain,
    bool recursive)
{
    return findNode(QRegExp(QRegExp::escape(tag)),domain,recursive);
}

QStringList XmlConfig::findNode(
    const QRegExp& tag,
    const QString& domain,
    bool recursive)
{
    QStringList result;
    QDomNode i;
    QDomElement e;
    QStringList list = this->list(domain);
    i=temporyDomainPrivate(domain);
    int j=0;
    QString new_domain;
    QString my_domain=domain;

    QRegExp exp{tag};

    for ( ; !i.isNull(); i = i.nextSibling(), j++) {
        e=i.toElement();
        if (e.isNull())
            continue;

        if (my_domain.isNull())//the domain is null, we don't want
            new_domain = list.at(j);
        else
            new_domain=my_domain + "." + list.at(j);

        if (exp.exactMatch(e.tagName())) { //search exactMach //if the tag of the node match the expresion
            if (recursive)
                result << new_domain; //the parent domain of this node
            else
                result << e.tagName();
        }
        if (recursive)
            result += findNode( tag, new_domain);
    }

    return result;
}

QStringList XmlConfig::findValue(
    const QString& value,
    const QString& domain)
{
    QStringList result;
    QDomNode i;
    QDomElement e;
    QStringList list = this->list(domain);
    i = temporyDomainPrivate(domain);
    int j = 0;
    QString new_domain;
    QString my_domain=domain;

    for ( ; !i.isNull(); i = i.nextSibling(), j++){
        e = i.toElement();
        if( !e.isNull() ){
            if (my_domain.isNull() ){
                new_domain = list.at(j);
            }
            else{
                new_domain = my_domain+"."+ list.at(j);
            }

            if (String::compare(e.firstChild().nodeValue(),value))
                result << new_domain;

            result += findValue( value, new_domain);
        }
    }

    return result;
}

QStringList XmlConfig::findValue(
    const QRegExp& value,
    const QString& domain)
{
    QStringList result;
    QDomNode i;
    QDomElement e;
    QStringList list=this->list(domain);
    i = temporyDomainPrivate(domain);
    int j = 0;
    QString new_domain;
    QString my_domain=domain;

    QRegExp exp {value};
    for ( ; !i.isNull(); i = i.nextSibling(), j++){
        e = i.toElement();
        if( !e.isNull() ){
            if(my_domain.isNull()) //the domain is null, we don't want
                new_domain = list.at(j);
            else
                new_domain=my_domain + "." +list.at(j);
            if (exp.exactMatch(e.firstChild().nodeValue())) //search exactMach //if the tag of the node match the expresion
                result << new_domain;
            result += findValue( value, new_domain);
        }
    }

    return result;
}

QStringList XmlConfig::findNodeValue(
    const QString& node,
    const QString& value,
    const QString& domain)
{
    QStringList result;
    QStringList nodes = findNode(node,domain);

    for ( auto it = nodes.begin(); it != nodes.end(); ++it ) {
        if (String::compare(nodeFromTagPrivate(*it).nodeValue(),value))
            result += *it;
    }

    return result;
}

const QDomDocument& XmlConfig::getDocument() const
{
    return doc ;
}

bool XmlConfig::createComment(
    const QString& tag,
    const QString& comment)
{
    std::cerr << "using a non-implemented method createComment with args " << (const char*)(tag.toLatin1()) << (const char*)(comment.toLatin1());
    return false;
}

//************************************************************************************
//************************************************************************************
//************************************************************************************
//PRIVATE
//************************************************************************************
//************************************************************************************
//************************************************************************************

QDomNode XmlConfig::nodeFromTagPrivate(const QString& tag)
{
    QDomNode result; // the result
    QString token;   // tokens separated with .

#ifdef MYDEBUG
    int i;  //	paranoic comprobation,
#endif
    int jump;        // disk[number] this variable is that number
    QString tagtemp=tag; // not mess with the parameter //maybe we need .toLower() here?

    //the API must work in the simplest case for the caller that means that
    //disk.partition == partition if partition is not defined outside disk ==
    // == disk[0].partition so we

    //WARNING: deleted paranoical checks, needed???
    // a library has to do it, there's quite a bit of performance impact

    //	int number_of_bracket=tag.contains('[');
    //	if (number_of_bracket!=tag.contains(']'))
    //	{
    //		cerr << "badformed nodeFromTag call" <<endl;
    //		return result; //we'll return a null domnode
    //	}
    //
    //  //more paranoic
    //  if((this->string_domain==0)||(string_domain.isEmpty()))
    //    this->domain=result;


    while (tagtemp.contains('.')){
#ifdef MYDEBUG
        if (result.isNull()){
            i++; //the first time is null, the next times it will have a value or it won't exit
            if (i==2)
                cerr << "bad tag, we were confused looking in the tree or tag don't exits" << endl;
        }
#endif
        //tag.find returns where is the first ocurrence, left the text till that position, and this-> find looks for the text
        //just believe in it
        token = tagtemp.left(tagtemp.indexOf('.'));
        token = discardBracketPrivate(token,&jump);
        result = findPrivate(token,result);

        if (result.isNull()) // because, once we can't find a parent, is a silly thing continue looking. even it's a bug
            return result;     // because in x.y.z we can just return the value of z when we mean x.y.z

        while(jump>0){
            jump--;  //now, we search between the brothers before search between the children
            result = findPrivate(token,result,false);
            if (result.isNull()) // because, once we can't find a parent, is a silly thing continue looking. even it's a bug
                return result;     // because in x.y.z we can just return the value of z when we mean x.y.z
        }

        tagtemp = tagtemp.right(tagtemp.length()-(tagtemp.indexOf('.')+1)); //+1== discard the x.
    }

    tagtemp = discardBracketPrivate(tagtemp,&jump);
    result = findPrivate(tagtemp,result);

    while( jump>0 ){
        jump--;
        result = findPrivate(tagtemp,result,false);
    }

    return(result);
}

QString XmlConfig::discardBracketPrivate(
    QString token,
    int *number)
{
    if (token.contains('[')){
        int digits = token.indexOf(']') - token.indexOf('['); //how many digits does the number have?
        QString temp = token.mid( token.indexOf('[') + 1, digits - 1);

        if (number) *number = temp.toInt();
        token=token.left( token.length() - ( token.indexOf(']') - token.indexOf('[') + 1 )); //we discard [n] n is a number with several digits
    }
    else if (number) *number = 0;

    return token;
}

QDomNode XmlConfig::findPrivate(
    const QString& tag,
    const QDomNode& from,
    bool anidate)
{
    QDomNode wall;
    int exist_children = 0; //if there's no more children we don't continue inwards

    //we must look between the childrens because this function will otherwise find in the case of
    // partition.geometry the brother of partition instead of his children so first go down one level

    if (from.isNull()) //null just means we search in the entire doc
    {
        if (!this->domain.isNull()) //the jail, if there's a jail we are under it
            //	if (!domain.firstChild().isNull())
            wall=domain.firstChild();
        //			else
        //				wall=domain;
        else                              //null just means we search in the entire doc
            if (anidate)
                wall = doc.documentElement().firstChild();
    }
    else
        if (anidate)
            wall=from.firstChild(); //we have a node from where to search
        else
            wall=from.nextSibling();

    QDomNode temp=wall;
    while(!temp.toComment().isNull())//skip comments
        temp=temp.nextSibling();    //we must be sure we skip comments

    //first we look in the same level
    while (!temp.toElement().isNull())
    {
        QString ein=temp.nodeName();
        if (String::compare(ein,tag))
            return(temp);  //we have found the solution
        if (!temp.firstChild().toElement().isNull())
            exist_children++;	// there's no element as children we won't find the solution so return null
        temp=temp.nextSibling(); //next older brother
        while(!temp.toComment().isNull())//skip comments
            temp=temp.nextSibling();
    }

    //WARNINGGG he comentao todo esto para eliminar el comportamiento recursivo
    //	if (!exist_children)
    //		return result; //result is a null node here, we return null to indicate we don't find it
    //	else
    //	{
    //	//no node in this level have this tag, we must go down one level and continue searching
    //		for (temp=wall; !temp.isNull(); temp=temp.nextSibling())
    //		{
    //			if (!temp.firstChild().toElement().isNull())//if it hasn't any children it can't have that tag
    //			{
    //
    //				result=this->find(tag,temp);  //recursively we look in the children
    //				if (!result.isNull()) //if the result is not null we have found the node
    //					return result;
    //			}
    //		}
    //		return result;
    //	}
    QDomNode result;
    return (result);
}

//FIXME this two methods can be merge in only one and make the program run faster
bool XmlConfig::createElementRecursivePrivate(
    const QString& node_name,
    const QString& value,
    const QString& comment,
    bool set_domain)
{
    if ( node_name.isNull() || node_name.isEmpty())
        return false;

    QDomNode node;
    QString temp=node_name;
    int how_many_nok=0;

    /*
    The node we'll create will be recursively, we create from the first node it's no created
    so we avoid creating twice nodes that already exist
  */


    while (!temp.isEmpty())     //here we know how many tags are ok
    {
        temp=String::stringOff(temp);
        how_many_nok++;
        node=nodeFromTagPrivate(temp);
        if (!node.isNull())
            break;
    }
    temp=node_name;
    for (int j=how_many_nok; j>0;j--) // how many new elements we have to create.
    {
        how_many_nok--;
        for (int i=how_many_nok; i>0;i--) // we take the next element to create in m.n.p notation
        {
            temp=String::stringOff(temp);
        }
        if (how_many_nok!=0)
        {
            if (this->createElementPrivate(temp,QString(),nullptr,false)==false) //the first times we just create the elements
                return false;
        }
        else
        {
            if (this->createElementPrivate(temp,value,comment,set_domain)==false)//here we create and add the value comment and set the domain
                return false;
        }
        temp=node_name;
    }
    return true;
    //  node=nodeFromTag(dst);
}


//FIXME this function suckssss, it repeats code several places etc..

/*
if value is null we won't be able to write to this element later this is this way because
in some way we have to distinguish between <node/> and <node></node> the problem here is that
if we allways leave the write to write we only have to ways to do it:
a) don't check anything in the write method what will lead to wrong xml when the programmer fails
b) allways make a text node here, this will be very easy but i don't want to polute the XML

So this is a bit unclear to the programmer, if value is null won't be writtable in the future
else it will.
*/

bool XmlConfig::createElementPrivate(
    const QString& node_name,
    const QString& value,
    const QString& comment,
    bool set_domain)
{
    QString tag=node_name;
    QString parent_name;
    QString tag_name;
    QString brother;
    QString temp;
    QDomNode where;

    //we split the command line in 2 : parent_name: everything before the last .
    //																tag_name: everything afther the last .
    if (tag.contains('.'))  //if there's no . there's no parent
        parent_name=tag.left(tag.lastIndexOf('.')); //we look for the parent of the new element we want to create
    if (tag.contains('.'))
        tag_name=tag.right(tag.length()-parent_name.length()-1);
    else
        tag_name=tag;

    QDomNode parent=this->nodeFromTagPrivate(parent_name); //the parent that will have the new element
    if (parent.isNull()){
        if (!this->domain.isNull())
            parent=this->domain;
        else
            parent=doc.documentElement();
    }

    tag_name=this->discardBracketPrivate(tag_name,nullptr); //the node with the name we want to write in the XML
    QDomElement tag_node=doc.createElement(tag_name);

    //the faster way to do it, if we have been requested a [n] (a specific number) we find his
    //sibling and just add it as a younger brother. If we have been requested to insert the first
    //node [0] or no [] in the end we can't find the younger brother so we just add t
    if (tag.at(tag.length()-1)==']'){
        int number= QString(tag.at(tag.length()-2)).toInt(); //the number of the brother
        if (number!=0){
            number--;
            brother=tag.left(tag.length()-2);  //the younger brother
            temp.setNum(number);
            brother=brother + temp +']';	//this MUST exist to be posible do it
            where = nodeFromTagPrivate(brother);  //yes, is he, the younger brother

            // if we have been requested to create [5] and [4] don't exist we just can't do it
            // without a bogus semantic.
            if (where.isNull()) return false;

            parent.insertAfter(tag_node,where);
        }
        else{ //the number is 0, this is a bit tricky

            brother=tag.left(tag.length()-2); //older brother

            brother=brother + "1" +']';	//this MUST exist to be posible do it

            where = nodeFromTagPrivate(brother);  //yes, is he, the older brother
            if (!where.isNull()) //the brother do exist, just add before it
                parent.insertBefore(tag_node,where);
            else { //the [1] don't exist, only exist [0] or nothing
                where = nodeFromTagPrivate(tag);		//we look for the [0]
                if (!where.isNull()){ // [0] already exist
                    QDomNode old_brother=parent.removeChild(where); //we remove it temporaly
                    parent.appendChild(tag_node);             //now it is the [0]
                    parent.insertAfter(old_brother, tag_node); //the old [0] is now [1]
                }
                else // [0] don't exist, so just append the child it will become the [0]
                    parent.appendChild(tag_node);
            }
        }
    }
    else
        //we rearch this point if we have been requested a tag with no [] at the end
        //we find the father and insert to it as the last child
        parent.appendChild(tag_node);	  //just add it to the end of the parent

    if(set_domain){
        if (tag_node.isNull())
            domain = tag_node;
        else
            //we set the domain to this node, this is necesary to
            //set everything correctly, it can be optimized
            setDomain(tagFromNodePrivate(tag_node),true);
    }

    if (comment!=nullptr){
        QDomComment c=doc.createComment(comment);
        tag_node.parentNode().insertBefore(tag_node,c);
    }

    if(!value.isNull()){
        QDomText text_node= doc.createTextNode(value);
        text_node.setNodeValue(value);
        tag_node.appendChild(text_node);
    }
    return true;
}

bool XmlConfig::deleteElement(const QString& tag)
{
    QDomNode node = nodeFromTagPrivate(tag);
    if (node.isNull())  return false;

    QDomNode parent=node.parentNode();
    if (parent.removeChild(node).isNull()) // it can't remove the child
        return false;
    return true;
}

bool XmlConfig::copyPrivate(
    XmlConfig *xml_src,
    const QString& src,
    const QString& dst,
    bool merge){

    QDomNode ta_el_cimbel;
    QString domain_src;
    QString my_domain;

    QDomNode node;
    QDomNode temp_node;
    QDomDocumentFragment data;
    QDomNode i;

    QDomNode parent_of_data;

    if (!xml_src)       //to avoid segfaults
        return false;
    QDomDocument doc_src = xml_src->getDocument();  //the settings of the src document
    domain_src = xml_src->getDomain();
    //xml_src->delDomain();

    my_domain = getDomain();    //we save our own settings
    QDomDocument my_doc = doc;

    //this->delDomain();
    doc = doc_src;           //we overwrite our settings with the src document's settings
    delDomain();
    setDomain(domain_src);

    node = nodeFromTagPrivate(src);//we look for in the src doc
    temp_node = node.cloneNode(true);
    data = temp_node.toDocumentFragment();

    doc = my_doc;        //we restore our settings
    delDomain();
    setDomain(my_domain);

    //here we have the data to be copied in data

    if ( dst.isNull() || dst.isEmpty()) {//it we don't have where to put the data we'll just throught them in the current place
        parent_of_data = domain;
        if (domain.isNull()) //we have no domain, the parent must be the root document
            parent_of_data = doc.documentElement();
    }
    else{
        node = nodeFromTagPrivate(dst);
        if (node.isNull()){ //maybe it doesn't exist then we create it
            createElement(dst); //this is the only one (we have just created it) so nodeFromTag return the correct
            parent_of_data=nodeFromTagPrivate(dst);
        }
        else { //we found the node where to copy or merge the data
            if (merge) // we want to merge to put the data inside the node requested
                parent_of_data=node;
            else{ //the node exist but we'll copy the data in a new node and copy inside it
                createElement(dst);      // here we have 2 cases, one of them is that a [] has been given
                if (dst.at(dst.length() - 1) == ']') //[] given
                    parent_of_data=nodeFromTagPrivate(dst);
                else{
                    //no [] given, we want it to enter the data in the last (just created node)
                    QString father,child;      //but nodefromTags will give us the first so we have to find the last
                    if (dst.contains('.')){ //we have domain and tag to look for
                        father = dst.left(dst.lastIndexOf('.'));
                        child = dst.right( dst.length() - father.length() -1 );
                    }
                    else{ //the tag is the what we look for and there's no domain
                        father = QString();
                        child = dst;
                    }
                    parent_of_data = nodeFromTagPrivate( dst + "[" + QString::number(howManyTags(child,father) - 1) +"]"); // our father is the last node
                }
            }
        }
    }

    //TODO en vez de temp_node -->data
    //here we copy all the children of this node, this will be automatic if data worked
    i = temp_node.firstChild();

    //�APA
    while (i!=temp_node.lastChild()){
        ta_el_cimbel = i.cloneNode(true);
        parent_of_data.appendChild(ta_el_cimbel);
        i = i.nextSibling();
    }

    parent_of_data.appendChild(temp_node.lastChild());
    return true;
}

QDomNode XmlConfig::temporyDomainPrivate(const QString& domain )
{
    QDomNode where;
    if ( !domain.isNull() && !domain.isEmpty() ){
        where = nodeFromTagPrivate(domain);
        if (where.isNull()) //the domain in the parameter is wrong
            return where;
    }
    else
        where = this->domain;

    if (where.isNull())
        where = doc.documentElement();

      return where.toElement().firstChild();
}

QString XmlConfig::tagFromNodePrivate(const QDomNode& node)
{
    QDomNode temp;
    QDomNode parent;
    int how_many=0;
    QString tag;
    QString partial_tag;
    QDomNode i;

    for(temp=node;temp!=doc.documentElement();temp=temp.parentNode())    {
        partial_tag=temp.nodeName();
        parent=temp.parentNode();
        if (!parent.isNull())
        {
            how_many=0;
            i=parent.firstChild();
            while( !i.isNull() ) {
                if (i==temp)
                    break;
                if (temp.nodeName()==i.nodeName()) //we want to know how many nodes with that name
                    how_many++;                      //are before our node so we can know the number
                i=i.nextSibling();
            }
            partial_tag=partial_tag+"["+QString::number(how_many)+"].";
            tag=tag.prepend(partial_tag);
        }
    }
    tag=tag.left(tag.length()-1); //the last .
    return tag;
}

/***************************************************************************
                          hlist.h  -  description
                             -------------------
    begin                : jue jul 10 2003
    copyright            : (C) 2003 by Napsis S.L.
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

#ifndef HLIST_H
#define HLIST_H

#include <QMap>
#include <QList>

/**
  *@author Carlos Manzanedo
  *@Modifier Gilles Bene
  */
//template<class T> class QList;
//template<class Key, class Value> class QMap;

//template <typename Value>
//using QPtrMap = QMap<QString, Value*>;

//template <typename Value>
//using QPtrList = QList<Value*>;

template  <typename T>
class HList{
public: 
    HList(){}
    ~HList(){ clear(); }

    auto begin(){ return list.begin(); }
    auto end(){ return list.end(); }

    int count() { return list.count(); }
    bool isEmpty(){ return list.isEmpty(); }
    int size(){ return list.size(); }
    int indexOf(T* const &item){ return list.indexOf(item); }
    int indexOf(const QString& key){ return indexOf(find(key)); }

    void clear(){
        map.clear();
        list.clear();
    }

    void insert(int pos, const T *item, const QString& key){
        list.insert(pos, item);
        map.insert(key, item);
    }

    void append(T* const &item, const QString& key){
        list.append(item);
        map.insert(key,item);
    }

    void prepend(const T *item, const QString& key){
        list.prepend(item);
        map.insert(key,item);
    }

    //Removes item at index. Return true if success, false otherwise
    bool remove(int index){
        T *list_item = list.at(index);
        return map.remove(map.key(list_item)) && list.removeOne(list.at(index)) ;
    }

    bool remove(const QString& key){
        return list.removeOne( map.value(key) ) && map.remove( key );
    }

    bool removeFirst(){ return remove(0); }

    bool removeLast(){ return remove(list.size() - 1); }

    //FIXME:
    //we don't actually understand the intention of this function
    // since it takes no parameters. In other not to break the existing API
    // We decided to assume it meant removing the first element.
    bool remove(){
        bool removed{true};
//        for( auto i = 0 ; i < size() ; ++i)
//            removed &= remove(i);
        clear();
        return  removed;
    }

    T* getFirst(){ return list.first(); }
    T* getLast(){ return list.last(); }
    T* at(int pos){ return list.at(pos); }
    int at(){ return list.at(0); }
    T* at(const QString& key ){ return find(key); }

    //     T* next(){ return list.next(); }
    //     T* previous(){ return list.previous(); }
    //     T* current(){  return list.current(); }

    T* find(const QString& key){   return map.value(key);    }

    T* take(const QString &key){
        T *dict_value =  map.value(key);
        remove(key);
        return dict_value;
    }

    void setAutoDelete(bool enabled){  autodelete = enabled;}
    T* operator[](const QString& key){return find(key);}
    T* operator[](int pos){ return list.at(pos);  }
    void statistics(){}

protected:
    bool moreThanOnce(const T *item){
        int c = 0;
        for( auto* aux_item : list ){
            if (aux_item == item) c++;
            if (c >= 2 ) return true;
        }
        return false;
    }

    QList<T*> list;
    QMap<QString, T*> map;
    bool autodelete{false};
};

#endif

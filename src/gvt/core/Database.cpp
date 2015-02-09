#include <iostream>
#include "gvt/core/Database.h"
#include "gvt/core/Debug.h"

using namespace gvt::core;

Database::Database()
{
}

DatabaseNode* Database::getItem(Uuid uuid) 
{
    return __nodes[uuid];
}

void Database::setItem(DatabaseNode* node) 
{
    __nodes[node->UUID()] = node;
    addChild(node->parentUUID(),node);
}

bool Database::hasNode(Uuid uuid) 
{
    return (__nodes.find(uuid) !=  __nodes.end());
}

bool Database::hasNode(DatabaseNode *node) 
{
    return (__nodes.find(node->UUID()) != __nodes.end());
}

ChildList& Database::getChildren(Uuid parent) 
{
    return __tree[parent];
}

void Database::addChild(Uuid parent, DatabaseNode *node) 
{
        __tree[parent].push_back(node);
}

void Database::removeItem(Uuid uuid) 
{
    if(__nodes[uuid] != NULL) {
        DatabaseNode* cnode = getItem(uuid);

        DEBUG_CERR("Removing item:");
        DEBUG(print(uuid, 0, std::cerr));

        ChildList* children = &__tree[uuid];
        ChildList::iterator it;

        DEBUG_CERR("removing children:");
        DEBUG(
            for(it = children->begin(); it != children->end(); it++)
            {
                print((*it)->UUID(),0,std::cerr);
            }
            );
        DEBUG_CERR("");

        int numChildren = children->size();
        for(int i=0;i<numChildren;i++) {
           removeItem((children->at(0))->UUID());
        }
        children = &__tree[cnode->parentUUID()];
        DEBUG(
        for(it = children->begin(); it != children->end(); it++)
            std::cerr << "tree item: " << uuid_toString((*it)->UUID()) << std::endl;
        );
        for(it = children->begin(); it != children->end(); ++it) {
            if ((*it)->UUID() == uuid) break;
        }
        Uuid puid = cnode->parentUUID();
        DEBUG_CERR( String("found tree item to remove from parent: ") + (*it)->name() + String(" ") + uuid_toString((*it)->UUID()) );
        if(it!= children->end()) children->erase(it);
        __nodes.erase(uuid);
//        nodes[uuid] = NULL;
        delete cnode;
    }
    else
    { DEBUG_CERR(String("ERROR: Could not find item to remove from database : ") + uuid_toString(uuid)); }
}


DatabaseNode* Database::getChildByName(Uuid parent, String name) 
{
    ChildList children = __tree[parent];
    for(ChildList::iterator it = children.begin(); it != children.end(); ++it) {       
        if((*it)->name() == name) return (*it);
    }
    return NULL;
}

void Database::print(const Uuid& parent, const int depth, std::ostream& os) 
{

    DatabaseNode* pnode = this->getItem(parent);
    if(!pnode) {
        DEBUG_CERR(String("Database::print - node not found: ") + uuid_toString(parent));
        return;
    }
    std::string offset = "";
    for(int i=0; i < depth; i++) offset += "-";
    os << offset << uuid_toString(pnode->UUID()) << " : " <<  pnode->name() << " : " << pnode->value() << std::endl;
    offset += "-";
    ChildList children = __tree[parent];
    for(ChildList::iterator it = children.begin(); it != children.end(); ++it) {
        DatabaseNode* node = (*it);
        os << offset << uuid_toString(node->UUID()) << " : " <<  node->name() << " : " << node->value() << std::endl;
    }
}

void Database::printtree(const Uuid& parent, const int depth, std::ostream& os) 
{
    DatabaseNode* pnode = this->getItem(parent);
    if(!pnode) {
        DEBUG_CERR(String("Database::printtree - node not found: ") + uuid_toString(parent));
        return;
    }
    std::string offset = "";
    for(int i=0; i < depth; i++) offset += "-";
    offset += "|";
    os << offset << uuid_toString(pnode->UUID()) << " : " <<  pnode->name() << " : " << pnode->value() << std::endl;
    ChildList children = __tree[parent];
    for(ChildList::iterator it = children.begin(); it != children.end(); ++it) {
        DatabaseNode* node = (*it);
        printtree(node->UUID(),depth+1,os);
    }
}



Variant Database::getValue(Uuid id)
{
    Variant val;
    DatabaseNode* node = getItem(id);
    if (node)
        val = node->value();
    return val;
}

void Database::setValue(Uuid id, Variant val)
{
    DatabaseNode* node = getItem(id);
    if (node)
        node->setValue(val);
}


Variant Database::getChildValue(Uuid parent, String child)
{
    Variant val;
    DatabaseNode* node = getChildByName(parent, child);
    if (node)
        val = node->value();
    return val;
}

void Database::setChildValue(Uuid parent, String child, Variant value)
{
    DatabaseNode* node = getChildByName(parent, child);
    if (node)
        node->setValue(value);
}

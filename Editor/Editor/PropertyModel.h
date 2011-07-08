#pragma once

#include "Editor/Core/Core.h"
#include <QtGui>

class Entity;
class Property;

class PropertyModel : public QAbstractTableModel {
public:
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;
    virtual int columnCount (const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data (const QModelIndex& index, int role) const;
    virtual QVariant headerData (int section, Qt::Orientation orientation,
                                int role) const;
    virtual Qt::ItemFlags flags (const QModelIndex& index) const;
    virtual bool setData (const QModelIndex& index, const QVariant& value,
                          int role);
    void SetEntity (Entity* e);
private:
    Entity* m_entity;
    vector<Property*> m_properties;
};

#include "Editor/Editor/PropertyModel.h"
#include "Game/Editor/Property.h"
#include "Game/Game/Entity.h"

int PropertyModel::rowCount (const QModelIndex& parent) const {
    return m_properties.size();
}

int PropertyModel::columnCount (const QModelIndex& parent) const {
    return 2;
}

QVariant PropertyModel::data (const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            return QString::fromStdString(m_properties[index.row()]->name());
        } else if (index.column() == 1) {
            string value;
            m_properties[index.row()]->Load(&value);
            return QString::fromStdString(value);
        }
    }

    return QVariant();
}

QVariant PropertyModel::headerData (int section, Qt::Orientation orientation,
                                    int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation != Qt::Horizontal)
        return QVariant();

    if (section == 0)
        return "Property";
    if (section == 1)
        return "Value";
    return QVariant();
}

Qt::ItemFlags PropertyModel::flags (const QModelIndex& index) const {
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool PropertyModel::setData (const QModelIndex& index, const QVariant& value,
                             int role) {
    if (index.isValid() && role == Qt::EditRole) {
        string valueStr = value.toString().toStdString();
        m_properties[index.row()]->Save(valueStr);
        emit(dataChanged(index, index));
        return true;
    }
    return false;
}

void PropertyModel::SetEntity (Entity* e) {
    m_entity = e;
    for (uint i = 0; i < m_properties.size(); i++)
        delete m_properties[i];
    m_properties.clear();
    if (m_entity != NULL)
        m_entity->GetProperties(&m_properties);
    emit(layoutChanged());
}

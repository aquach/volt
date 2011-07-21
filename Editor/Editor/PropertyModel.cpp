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

    Property* property = m_properties[index.row()];
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0) {
            return QString::fromStdString(property->name());
        } else if (index.column() == 1) {
            if (property->checkable())
                return "";
                
            string value;
            property->Load(&value);
            return QString::fromStdString(value);
        }
    }

    // Handle checkable properties.
    if (index.column() == 1 && role == Qt::CheckStateRole &&
        property->checkable()) {
        return property->checked() ? Qt::Checked : Qt::Unchecked;
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
    if (index.column() == 0)
        return Qt::ItemIsEnabled;
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if (m_properties[index.row()]->editable())
        flags |= Qt::ItemIsEditable;
    else
        flags ^= Qt::ItemIsEnabled;
        
    if (m_properties[index.row()]->checkable())
        flags |= Qt::ItemIsUserCheckable;
    return flags;
}

bool PropertyModel::setData (const QModelIndex& index, const QVariant& value,
                             int role) {
    if (!index.isValid())
        return false;

    if (role == Qt::CheckStateRole) {
        m_properties[index.row()]->setChecked(value.toBool());
    } else if (role == Qt::EditRole) {
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

void PropertyModel::OnPropertyActivated (const QModelIndex& index) {
    if (index.isValid()) {
        Property* property = m_properties[index.row()];
        // Special case for clicking on a ColorProperty.
        if (ColorProperty* colorProp = dynamic_cast<ColorProperty*>(property)) {
            Volt::Color current = colorProp->value();
            QColor c = QColorDialog::getColor(
                        QColor::fromRgbF(current.r, current.g, current.b, current.a),
                        NULL,
                        "",
                        QColorDialog::ShowAlphaChannel);
            colorProp->Set(Volt::Color::RGBA(c.red(), c.green(), c.blue(),
                                             c.alpha()));
            emit(layoutChanged());
        }
    }
}

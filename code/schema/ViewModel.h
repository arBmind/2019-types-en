#pragma once
#include "../strong/Strong.h"
#include "Abstract.h"
#include "Repository.h"
#include "names.h"

// Verdigris
#include "wobjectcpp.h"
#include "wobjectimpl.h"

#include <QAbstractListModel>
#include <QDebug>
#include <QObject>
#include <QVariant>

namespace view_model {

using namespace abstract;

struct ADL {};

template<class T>
using ToViewModel = decltype(toViewModel(ADL{}, Ptr<T>{}));

template<class... Ts>
class AllOfView : public QObject {
    W_OBJECT(AllOfView)

    using Repository = repository::ToRepository<AllOf<Ts...>>;
    Repository *m_repository{};

    template<size_t I>
    using PropertyAt = std::remove_reference_t<decltype(std::get<I>(std::declval<Repository>()))>;

public:
    AllOfView(Repository *repo, QObject *parent = {})
        : QObject(parent)
        , m_repository(repo) {}

private:
    template<size_t I>
    void propertyChanged() {
        W_CPP_SIGNAL_IMPL(decltype(&AllOfView::propertyChanged<I>), PropertyChangedSignals, I, 0);
    }

    template<size_t I, class = std::enable_if_t<(I < sizeof...(Ts))>>
    struct PropertyChangedSignals {
        using Property = PropertyAt<I>;
        constexpr static auto signalName = names::property_changed_name<Property>;
        constexpr static auto signal =
            w_cpp::makeSignalBuilder(signalName, &AllOfView::propertyChanged<I>).build();
    };
    W_CPP_SIGNAL(PropertyChangedSignals)

    template<size_t I>
    auto getPropertyValue() const -> QVariant {
        const auto &property = std::get<I>(*m_repository);
        using Property = PropertyAt<I>;
        if constexpr (std::is_enum_v<Property>)
            return static_cast<std::underlying_type_t<Property>>(property);
        else {
            using ValueType = decltype(std::declval<Property>().v);
            if constexpr (std::is_same_v<std::string, ValueType>)
                return QVariant(QString::fromStdString(property.v));
            else
                return QVariant::fromValue(property.v);
        }
    }
    template<size_t I>
    void setPropertyValue(QVariant variant) {
        auto &property = std::get<I>(*m_repository);
        using Property = PropertyAt<I>;
        if constexpr (std::is_enum_v<Property>) {
            using ValueType = std::underlying_type_t<Property>;
            property = static_cast<Property>(variant.value<ValueType>());
        }
        else {
            using ValueType = decltype(std::declval<Property>().v);
            if constexpr (std::is_same_v<std::string, ValueType>)
                property.v = variant.value<QString>().toStdString();
            else
                property.v = variant.value<ValueType>();
        }
        propertyChanged<I>(); // send notification for the property
    }
    template<size_t I>
    constexpr static auto property_name = names::property_name<PropertyAt<I>>;
    constexpr static auto qvariant_name = w_cpp::viewLiteral("QVariant");

    template<size_t I, class = std::enable_if_t<(I < sizeof...(Ts))>>
    struct RegisterProperties {
        constexpr static auto property =
            w_cpp::makeProperty<QVariant>(property_name<I>, qvariant_name)
                .setGetter(&AllOfView::getPropertyValue<I>)
                .setSetter(&AllOfView::setPropertyValue<I>)
                .setNotify(&AllOfView::propertyChanged<I>);
    };
    W_CPP_PROPERTY(RegisterProperties)
};
W_OBJECT_IMPL((AllOfView<Ts...>), template<class... Ts>)

template<class... Ts>
auto toViewModel(ADL, AllOf<Ts...> *) -> AllOfView<Ts...>;

template<class Id, class Entity>
class EntitySetModel : public QAbstractListModel {
    using T = EntitySet<Id, Entity>;
    using Repository = repository::ToRepository<T>;
    Repository *m_repository{};

public:
    EntitySetModel(Repository *repo, QObject *parent = {})
        : QAbstractListModel(parent)
        , m_repository(repo) {}

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override {
        Q_UNUSED(parent)
        if (std::distance(m_repository->begin(), m_repository->end()) <= row) {
            qWarning() << "EntityModel::index: " << row << "out of bounds;";
            return {};
        }
        auto i = m_repository->begin();
        std::advance(i, row);

        return createIndex(row, column, i->first.v);
    }
    int rowCount(const QModelIndex &parent = {}) const override {
        Q_UNUSED(parent)
        return m_repository->count();
    }
    QVariant data(const QModelIndex &index, int role) const override {
        Q_UNUSED(role)
        if (index.isValid()) return index.internalId();
        return {};
    }
};

template<class Id, class Entity>
class EntitySetView : public QObject {
    W_OBJECT(EntitySetView)

    using TT = EntitySet<Id, Entity>;
    using Repository = repository::ToRepository<TT>;
    Repository *m_repository{};

    using Model = EntitySetModel<Id, Entity>;
    Model *m_model{};

public:
    EntitySetView(Repository *repo, QObject *parent = {})
        : QObject(parent)
        , m_repository(repo)
        , m_model(new Model(m_repository, this)) {}

    auto repository() { return m_repository; }

    auto model() -> QObject * { return m_model; }
    W_INVOKABLE(model)

    auto entity(int qmlId) -> QObject * {
        auto id = Id{qmlId};

        if (!m_repository->contains(id)) return nullptr;
        auto &value = (*m_repository)[id];

        using EntityModel = ToViewModel<Entity>;
        return new EntityModel(&value);
    }
    W_INVOKABLE(entity)
};
// clang-format off
W_OBJECT_IMPL((EntitySetView<Id,Entity>), template<class Id, class Entity>)
// clang-format on

template<class Id, class Entity>
auto toViewModel(ADL, EntitySet<Id, Entity> *) //
    -> EntitySetView<Id, Entity>;

} // namespace view_model

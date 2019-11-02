#pragma once
#include "../strong/Strong.h"
#include "Recursive.h"
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

using namespace recursive;

// clang-format off
constexpr struct ADL {} adl = {};
// clang-format on

template<class T>
using ViewModelFor = decltype(viewModelFor(adl, ptr<T>));

template<class T>
auto makeQVariantFrom(const T &value) -> QVariant {
    if constexpr (std::is_enum_v<T>)
        return static_cast<std::underlying_type_t<T>>(value);
    else if constexpr (is_strong<T>) {
        if constexpr (std::is_same_v<std::string, decltype(value.v)>)
            return QString::fromStdString(value.v);
        else
            return QVariant::fromValue(value.v);
    }
}
template<class T>
auto makeValueFrom(const QVariant &variant) -> T {
    if constexpr (std::is_enum_v<T>) {
        using V = std::underlying_type_t<T>;
        return static_cast<T>(variant.value<V>());
    }
    else if constexpr (is_strong<T>) {
        using V = decltype(std::declval<T>().v);
        if constexpr (std::is_same_v<std::string, V>)
            return T{variant.value<QString>().toStdString()};
        else
            return T{variant.value<V>()};
    }
}

template<class T, class R>
auto makeViewModelFor(R &repo, QObject *parent) -> QObject * {
    if constexpr (is_recursive<T>) {
        return new ViewModelFor<T>(repo, parent);
    }
    return nullptr;
}

template<class... Ts>
class AllOfView : public QObject {
    W_OBJECT(AllOfView)

    using Repository = repository::RepositoryFor<AllOf<Ts...>>;
    Repository &m_repository{};
    template<size_t I>
    using RepositoryAt = std::remove_reference_t<decltype(std::get<I>(std::declval<Repository>()))>;

    using QObjectTuple = std::tuple<std::conditional_t<true, QObject *, Ts>...>;
    QObjectTuple m_views{};

    constexpr static auto qvariant_name = w_cpp::viewLiteral("QVariant");

    using TsTuple = std::tuple<Ts...>;
    template<size_t I>
    using PropertyAt = std::remove_reference_t<decltype(std::get<I>(std::declval<TsTuple>()))>;

    template<size_t I>
    constexpr static auto property_name = names::property_name<PropertyAt<I>>;

public:
    AllOfView(Repository &repo, QObject *parent = {})
        : QObject(parent)
        , m_repository(repo)
        , m_views(makeViewModelFor<Ts>( //
              std::get<repository::RepositoryFor<Ts>>(m_repository), this)...) {}

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
        if constexpr (is_recursive<PropertyAt<I>>) {
            return std::get<I>(m_views);
        }
        else {
            return makeQVariantFrom(std::get<I>(m_repository));
        }
    }
    template<size_t I>
    void setPropertyValue(QVariant variant) {
        if constexpr (is_recursive<PropertyAt<I>>) {
            // no set supported!
        }
        else {
            std::get<I>(m_repository) = makeValueFrom<PropertyAt<I>>(variant);
            propertyChanged<I>(); // send notification for the property
        }
    }

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
W_OBJECT_IMPL(AllOfView<Ts...>, template<class... Ts>)

template<class... Ts>
auto viewModelFor(ADL, AllOf<Ts...> *) -> AllOfView<Ts...>;

template<class Id, class Entity>
class EntitySetModel : public QAbstractListModel {
    using T = EntitySet<Id, Entity>;
    using Repository = repository::RepositoryFor<T>;
    Repository &m_repository{};

public:
    EntitySetModel(Repository &repo, QObject *parent = {})
        : QAbstractListModel(parent)
        , m_repository(repo) {}

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override {
        Q_UNUSED(parent)
        if (std::distance(m_repository.begin(), m_repository.end()) <= row) {
            qWarning() << "EntityModel::index: " << row << "out of bounds;";
            return {};
        }
        auto i = m_repository.begin();
        std::advance(i, row);

        return createIndex(row, column, i->first.v);
    }
    int rowCount(const QModelIndex &parent = {}) const override {
        Q_UNUSED(parent)
        return m_repository.count();
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
    using Repository = repository::RepositoryFor<TT>;
    Repository &m_repository{};

    using Model = EntitySetModel<Id, Entity>;
    Model *m_model{};

public:
    EntitySetView(Repository &repo, QObject *parent = {})
        : QObject(parent)
        , m_repository(repo)
        , m_model(new Model(m_repository, this)) {}

    auto repository() { return m_repository; }

    auto model() -> QObject * { return m_model; }
    W_INVOKABLE(model)

    auto entity(int qmlId) -> QObject * {
        auto id = Id{qmlId};

        if (!m_repository.contains(id)) return nullptr;
        auto &value = m_repository[id];

        using EntityModel = ViewModelFor<Entity>;
        return new EntityModel(value);
    }
    W_INVOKABLE(entity)
};
// clang-format off
W_OBJECT_IMPL((EntitySetView<Id,Entity>), template<class Id, class Entity>)
// clang-format on

template<class Id, class Entity>
auto viewModelFor(ADL, EntitySet<Id, Entity> *) //
    -> EntitySetView<Id, Entity>;

} // namespace view_model

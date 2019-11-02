#include "Command.h"
#include "PersonSchema.h"
#include "Processor.h"
#include "ViewModel.h"

#include "wobjectdefs.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

using namespace person;

using command::CommandFor;
using command::EntityCreate;
using processor::command_processor_for;
using repository::RepositoryFor;

using Command = CommandFor<Persons>;
using Repository = RepositoryFor<Persons>;
constexpr auto processCommand = command_processor_for<Persons>;
using ViewModel = view_model::ViewModelFor<person::Persons>;

constexpr auto W_ClassNameOverride(w_internal::W_TypeWrap<ViewModel> = {}) {
    return w_cpp::viewLiteral("ViewModel");
}
constexpr auto
W_ClassNameOverride(w_internal::W_TypeWrap<view_model::ViewModelFor<person::PersonData>> = {}) {
    return w_cpp::viewLiteral("PersonDataView");
}

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app{argc, argv};

    QQmlApplicationEngine engine{};

    qmlRegisterUncreatableType<ViewModel>("SchemaGui", 1, 0, "ViewModel", "restricted");
    qmlRegisterUncreatableType<view_model::ViewModelFor<person::PersonData>>(
        "SchemaGui", 1, 0, "PersonDataView", "restricted");

    auto repo = Repository{};
    using CreateCmd = EntityCreate<PersonData>;
    processCommand(CreateCmd{Name{"Bjarne Stroustrup"}, Role::Teacher}, repo);
    processCommand(CreateCmd{Name{"Albert Einstein"}, Role::Teacher}, repo);
    processCommand(CreateCmd{Name{"Gyro Gearloose"}, Role::Student}, repo);

    auto model = ViewModel{repo};

    auto context = engine.rootContext();
    context->setContextProperty("view_model", &model);

    engine.load(QStringLiteral("qrc:/schema_gui.qml"));
    return app.exec();
}

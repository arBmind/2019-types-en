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
using PersonDataView = view_model::ViewModelFor<person::PersonData>;

constexpr auto w_explicitObjectName(PersonDataView *) {
    return w_cpp::viewLiteral("PersonDataView");
}

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app{argc, argv};

    QQmlApplicationEngine engine{};
    qmlRegisterUncreatableType<PersonDataView>("SchemaGui", 1, 0, "PersonDataView", "restricted");

    auto repo = Repository{};
    using CreateCmd = EntityCreate<PersonData>;
    processCommand(CreateCmd{Name{"Bjarne Stroustrup"}, Role::Teacher}, repo);
    processCommand(CreateCmd{Name{"Albert Einstein"}, Role::Teacher}, repo);
    processCommand(CreateCmd{Name{"Gyro Gearloose"}, Role::Student}, repo);

    auto model = ViewModel{repo};
    qmlRegisterSingletonInstance("SchemaGui", 1, 0, "ViewModel", &model);

    engine.load(QStringLiteral("qrc:/schema_gui.qml"));
    return app.exec();
}

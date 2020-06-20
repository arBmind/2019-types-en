import qbs

Project {
    minimumQbsVersion: "1.7.1"

    Product {
        name: "cpp2a"

        Export {
            Depends { name: "cpp" }
            cpp.cxxLanguageVersion: "c++2a"
            // cpp.cxxFlags: {
            //     if (qbs.toolchain.contains('msvc')) return ["/await", "/permissive-"];
            //     if (qbs.toolchain.contains('clang')) return ["-fcoroutines-ts"];
            // }
            // cpp.cxxStandardLibrary: {
            //     if (qbs.toolchain.contains('clang')) return "libc++";
            // }
            // cpp.staticLibraries: {
            //     if (qbs.toolchain.contains('clang')) return ["c++", "c++abi"];
            // }
        }
    }

    StaticLibrary {
        name: "schema"
        Depends { name: "cpp2a" }
        files: [
            "Command.cpp",
            "Command.h",
            "Compute.cpp",
            "Compute.h",
            "NoAdlStorage.cpp",
            "NoAdlStorage.h",
            "PersonCompute.cpp",
            "PersonCompute.h",
            "PersonSchema.cpp",
            "PersonSchema.h",
            "Processor.cpp",
            "Processor.h",
            "Recursive.cpp",
            "Recursive.h",
            "Repository.cpp",
            "Repository.h",
            "Storage.cpp",
            "Storage.h",
        ]

        Export { Depends { name: "cpp2a" } }
    }

    Application {
        name: "schema_test"
        consoleApplication: true
        Depends { name: "schema" }
        files: [
            "SchemaTest.cpp",
        ]
    }

    Product {
        name: "Verdigris"
        files: [
            "wobjectcpp.h",
            "wobjectdefs.h",
            "wobjectimpl.h",
        ]
    }

    StaticLibrary {
        name: "view_model"

        files: [
            "ViewModel.cpp",
            "ViewModel.h",
            "names.h",
        ]

        Depends { name: "schema" }
        Depends { name: "Qt.qml" }

        Export {
            Depends { name: "schema" }
            Depends { name: "Qt.qml" }
        }
    }
    Application {
        name: "schema_gui"
        // consoleApplication: true

        files: [
            "schema_gui.cpp",
            "schema_gui.qml",
            "schema_gui.qrc",
        ]

        Depends { name: "view_model" }
        Depends { name: "Qt.quick" }
    }
}

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
        name: "strong"
        Depends { name: "cpp2a" }
        files: [
            "Strong.cpp",
            "Strong.h",
        ]

        Export { Depends { name: "cpp2a" } }
    }
    Application {
        name: "strong_test"
        consoleApplication: true
        Depends { name: "strong" }
        files: [
            "StrongTest.cpp",
        ]
    }
}

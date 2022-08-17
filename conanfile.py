from conan import ConanFile
from conan.tools.cmake import CMake


class KieJsonConan(ConanFile):
    name = "kie_json"
    version = "0.1.0"

    # Optional metadata
    license = "MIT"
    author = "Kie"
    url = "https://github.com/Kidsunbo/kie_json"
    description = "A simple json serialization and deserialization library power by Boost.PFR."
    topics = ("Reflection", "Json", "Serialization")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "include/*", "test/*", "CMakeLists.txt", "LICENSE", "README.md"
    no_copy_source = True

    generators = "CMakeToolchain", "CMakeDeps"
    requires = "boost/1.79.0", "nlohmann_json/3.10.5", "gtest/cci.20210126"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        self.folders.build = "build"
        self.folders.generators = "build/conan"

    def package(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()


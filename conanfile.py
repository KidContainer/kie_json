from conan import ConanFile
from conan.tools.cmake import CMake


class KieJsonConan(ConanFile):
    name = "kie_json"
    version = "0.1.0"

    # Optional metadata
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of KieJson here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "*"
    no_copy_source = True

    generators = "CMakeToolchain", "CMakeDeps"
    requires = "boost/1.78.0", "nlohmann_json/3.10.5", "gtest/cci.20210126"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        self.folders.build = "build"
        self.folders.generators = "build/conan"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", "include", "include")
        self.copy("*.hpp", "include", "include")
        self.copy("LICENSE", "license")

    def package_id(self):
        self.info.header_only()


    def package_info(self):
        self.cpp_info.libs = ["kie_json"]

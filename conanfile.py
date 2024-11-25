from conan import ConanFile


class Recipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps", "VirtualRunEnv"

    def layout(self):
        self.folders.generators = "conan"

    def requirements(self):
        self.requires("fmt/11.0.2")
        self.requires("sfml/2.6.1")
        self.requires("argparse/3.1")
        self.requires("boost/1.86.0")

    def configure(self):
        self.options["boost"].without_test=True

    def build_requirements(self):
        self.test_requires("catch2/3.7.0")
        self.test_requires("benchmark/1.9.0");

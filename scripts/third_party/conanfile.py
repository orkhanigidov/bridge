from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, cmake_layout


class ConanApplication(ConanFile):
    package_type = "application"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"
    default_options = {"rttr*:with_rtti": True}

    def layout(self):
        cmake_layout(self)
        self.folders.generators = "generators"
        self.folders.build = "."

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def requirements(self):
        requirements = self.conan_data.get('requirements', [])
        for requirement in requirements:
            self.requires(requirement)

<div align="center">

# xstd

`xstd` is a C++ library that provides extensions for the standard template library (STL) to be used by ensketch projects.

![top-language-badge](https://img.shields.io/github/languages/top/ensketch/xstd.svg?style=for-the-badge)
![code-size-badge](https://img.shields.io/github/languages/code-size/ensketch/xstd.svg?style=for-the-badge)
![repo-size-badge](https://img.shields.io/github/repo-size/ensketch/xstd.svg?style=for-the-badge)
[![license-badge](https://img.shields.io/github/license/ensketch/xstd.svg?style=for-the-badge&color=blue)](#copyright-and-license)

![GitHub commit activity](https://img.shields.io/github/commit-activity/y/ensketch/xstd?style=for-the-badge)
![GitHub last commit (by committer)](https://img.shields.io/github/last-commit/ensketch/xstd?style=for-the-badge)
![GitHub tag (with filter)](https://img.shields.io/github/v/tag/ensketch/xstd?style=for-the-badge)

[![cppget.org](https://img.shields.io/website/https/cppget.org/libensketch-xstd.svg?down_message=offline&label=cppget.org&style=for-the-badge&up_color=blue&up_message=online)](https://cppget.org/libensketch-xstd)

</div>

## Requirements and Dependencies

`xstd` uses the [standard `build2` CI service][ci.cppget.org] to build and test its source code with a [variety of build configurations][ci.cppget.org-build-configs].
For a thorough list of requirements, see the [`manifest`][manifest] file.
To see detailed information about the current build status, [set up `xstd` for development and run `bdep ci`](#how-to-set-up-xstd-for-development).
<!-- To see the build status of already published package versions, see [the package's build status report]. -->
Here is a short list of essential requirements and dependencies:

**Architecture:**
- `x86_64` | `aarch64`

**Operating System:**
- Linux
<!-- | MacOS | Windows | FreeBSD -->

**C++ Compiler:**
- GCC 14 | Clang 18
<!-- | MinGW | MSVC | Emscripten -->
- `libstdc++` | `libc++`

**Build Toolchain:**
- [`build2`][build2]

<!-- **Automatically handled Dependencies:** -->
<!-- **Manually handled Requirements:** -->

<!-- ## Introduction -->

## Getting Started

`xstd` relies on [the `build2` toolchain][build2].
You need to make sure it is installed on your system.
If it is not available via your system's package manager, follow the instructions in [The `build2` Installation Manual][build2-install] to build it from source and install it.
It is also a good idea to read the [toolchain's documentation][build2-docs].
You can start with [The `build2` Toolchain Introduction][build2-intro] and then read [The `build2` Build System Manual][build2-build-system] if you want to know more.

### Usage in `build2`-Based Projects

1. Add this repository to the `repositories.manifest` of your `build2` project.

```
:
role: prerequisite
location: https://github.com/ensketch/xstd.git
```

2. State the package dependency with an [optional version constraint](https://build2.org/bpkg/doc/build2-package-manager-manual.xhtml#package-version-constraint) in the `manifest`s of all `build2` packages in your project that shall use this library.

```
depends: libensketch-xstd ^0.1.0
```

3. Import the library in each `buildfile` that contains at least one target that depends on the library and declare it as a dependency.

```
import xstd = libensketch-xstd%lib{ensketch-xstd}
exe{myexe}: {hxx cxx}{**} $xstd
```

## Configuration

`xstd` does not include any publicly accessible configuration options.

## Documentation

This `README.md` file currently contains all project documentation.
In the future, some sections may be extracted.

### Usage, Examples, and Tutorials

Currently, there are no examples and/or tutorials available.

### FAQs and HOWTOs

#### How to install or use `xstd` in projects not based on `build2`?

In other words: *How to make a `build2` package available on your system for projects that do not use `build2`?*

The instructions provided here are extracted from [The `build2` Toolchain Introduction: Package Consumption][build2-intro-consumption], focusing on the essential steps.
We strongly recommend that you peruse the manual section to understand the use of more advanced consumption strategies and workflows.
After installing the library, you can import it into your project using the provided `pkg-config` file or by manually specifying the include and library paths.
For more information on using unpackaged dependencies, even in build strategies that do not rely on `build2`, see [The `build2` Toolchain Introduction: Using Unpackaged Dependencies][build2-intro-unpackaged-dependencies].

1. If you haven't already, create a build configuration for C/C++-based `build2` projects. You can use this for all your needed `build2` packages by starting from step `2`.

```
bpkg create -d build2-packages cc \
  config.cxx=g++ \
  config.cc.coptions="-O3 -march=native" \
  config.install.root=/usr/local \
  config.install.sudo=sudo
```

2. Fetch the repository's packages and build them.

```
bpkg build https://github.com/ensketch/xstd.git
```

3. Install the built packages.

```
bpkg install libensketch-xstd
```

4. Optionally, uninstall the built packages when you do not need them any longer.

```
bpkg uninstall libensketch-xstd
```

5. To upgrade the package, fetch all new information and uninstall the older version. Afterwards, build the new version and install it.

```
bpkg fetch
bpkg status

bpkg uninstall libensketch-xstd
bpkg build libensketch-xstd
bpkg install libensketch-xstd
```

#### How to set up `xstd` for development?

The essential steps for preparing this repository for development are taken from [the `build2` documentation][build2-docs] and are shown in short.
Please note that you should be familiar with the build2 toolchain.
For more information, see [The build2 Toolchain Introduction][build2-intro].

1. Create a directory for project-specific files. Clone the repository into this folder and run the development script to set up the development environment.

```
mkdir ensketch && cd ensketch
git clone --recurse https://github.com/ensketch/xstd.git
cd xstd
.develop/init.sh
```

2. Use `bdep` to create the initial build configuration. If not stated otherwise, it will be placed within the previously created project-specific directory. For example, the command below shows how to create and initialize a configuration named `gcc-release` with installation and distribution support.

```
bdep init -C @gcc-release cc \
  config.cxx=g++ \
  config.cxx.coptions="-O3 -march=native" \
  config.install.root=../.install \
  config.dist.root=../.dist
```

3. Let's dive into development. Below are key steps and common commands to guide you in the process.

  + Use `bdep` to print the status of project packages and their immediate dependencies.

```
bdep status -i
```

  + Use `b` to build and test the source code.

```
b                      # Build all the code.
b test                 # Only build and run the tests.
b {clean update}       # Clean build all the code.
b {clean update test}  # Clean build all the code and run the tests.
```

  + Similarly, to build and test the source in a specific build configuration with `bdep`, run the following commands.

```
bdep update @gcc-release
bdep test @gcc-release
bdep clean @gcc-release
```

  + Use `b` to test the installation of the project. In the example configuration `gcc-release` created above, the installation directory `.install` is part of the development folder.

```
b install
b uninstall
```

  + Use `b` to test the distribution of the project's packages. In the example configuration `gcc-release` created above, the directory for distributions `.dist` is part of the development folder.

```
b dist: libensketch-xstd
```

  + After thoroughly testing the aforementioned steps, make a commit and leverage [`build2`'s standard CI service][ci.cppget.org] to simultaneously build and test the project across a [range of build configurations][ci.cppget.org-build-configs].

```
bdep ci
```

### API Reference

We're sorry, there is no API reference documentation currently available.
This is due to the lack of automated generation for modern C++ libraries.
For now, we encourage you to explore the project's source files.
The comments should help you understand the API and how it's implemented.

### Background and Discussions

Currently, there are no background documentation or discussions available.

## Roadmap

Currently, there is no roadmap available.

## Changelog

All notable changes to this project will be documented in [`CHANGELOG.md`](CHANGELOG.md).

The format is based on [Keep a Changelog][keep-a-changelog] and [Common Changelog][common-changelog], and this project adheres to [Semantic Versioning][semantic-versioning].

## Contributing

`xstd` is open-source, and we appreciate your contributions.
Please use our [GitHub issue tracking page][project-issues] to report bugs, request new features, ask questions, and suggest other improvements.
If you want to contribute, consult our [contribution guidelines][ensketch-contributing].
Also, please reference an open issue when submitting a pull request.

## Code of Conduct

All contributors are expected to adhere to [our established code of conduct][ensketch-code-of-conduct].

## Contact

Email us at ensketch@mailbox.org with any questions or comments about `xstd`.
You may also tell us about your project or specific use case if you use `xstd`.
We're happy to hear from you.

## Copyright and License

The copyright for the code is held by the contributors of the code.
The revision history in the version control system is the primary source of authorship information for copyright purposes.
Please see individual source files for appropriate copyright notices.
For more information, see the file [`AUTHORS.md`](AUTHORS.md).

`xstd` is free software, distributed under the terms of the GNU General
Public License as published by the Free Software Foundation,
version 3 of the License (or any later version).  For more information,
see the [GNU General Public License][GPLv3] or the file [`COPYING.md`](COPYING.md).

`xstd` is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

Copyright years on `xstd` source files may be listed using range notation, e.g., 1987-2012, indicating that every year in the range, inclusive, is a copyrightable year that could otherwise be listed individually.

Copying and distribution of this file, with or without modification, are permitted in any medium without royalty provided the copyright notice and this notice are preserved.
This file is offered as-is, without any warranty.

## References and Other Resources

- [Ensketch's Default Contributing Guidelines][ensketch-contributing]
- [Ensketch's Default Code of Conduct][ensketch-code-of-conduct]

- [`build2` | C/C++ Build Toolchain][build2]
<!-- - [`cppget.org` | Standard `build2` Package Repository][cppget.org] -->
<!-- - [`ci.cppget.org` | Standard `build2` CI Service][ci.cppget.org] -->
<!-- - [`build2` | Installation][build2-install] -->
<!-- - [`build2` | Documentation][build2-docs] -->
<!-- - [The `build2` Toolchain Introduction][build2-intro] -->
<!-- - [The `build2` Build System][build2-build-system] -->
<!-- - [The `build2` Package Manager][build2-package-manager] -->
<!-- - [The `build2` Toolchain Introduction: Package Consumption][build2-intro-consumption] -->
<!-- - [The `build2` Toolchain Introduction: Using Unpackaged Dependencies][build2-intro-unpackaged-dependencies] -->

- [GNU Licenses][GNU-licenses]
<!-- - [GNU General Public License Version 3][GPLv3] -->

- [Keep a Changelog][keep-a-changelog]
- [Common Changelog][common-changelog]
- [Semantic Versioning][semantic-versioning]

[manifest]: libensketch-xstd/manifest (libensketch-xstd build2 Package Manifest)
[project-issues]: https://github.com/ensketch/xstd/issues (Project Issues)

[ensketch-code-of-conduct]: https://github.com/ensketch/.github/blob/main/CODE_OF_CONDUCT.md (Ensketch's Default Code of Conduct)
[ensketch-contributing]: https://github.com/ensketch/.github/blob/main/CONTRIBUTING.md (Ensketch's Default Contributing Guidelines)

[build2]: https://build2.org (build2 | C/C++ Build Toolchain)
[build2-install]: https://build2.org/install.xhtml (build2 | Installation)
[build2-docs]: https://build2.org/doc.xhtml (build2 | Documentation)
[build2-intro]: https://build2.org/build2-toolchain/doc/build2-toolchain-intro.xhtml (The build2 Toolchain Introduction)
[build2-build-system]: https://build2.org/build2/doc/build2-build-system-manual.xhtml (The build2 Build System)
[build2-package-manager]: https://build2.org/bpkg/doc/build2-package-manager-manual.xhtml (The build2 Package Manager)
[build2-intro-consumption]: https://build2.org/build2-toolchain/doc/build2-toolchain-intro.xhtml#guide-consume-pkg (The build2 Toolchain Introduction: Package Consumption)
[build2-intro-unpackaged-dependencies]: https://build2.org/build2-toolchain/doc/build2-toolchain-intro.xhtml#guide-unpackaged-deps (The build2 Toolchain Introduction: Using Unpackaged Dependencies)

[cppget.org]: https://cppget.org/ (Standard build2 Package Repositories)
[ci.cppget.org]: https://ci.cppget.org/ (Standard build2 CI Service)
[ci.cppget.org-build-configs]: https://ci.cppget.org/?build-configs (Build Configurations in Standard build2 CI Service)

[GNU-licenses]: https://www.gnu.org/licenses/ (GNU Licenses)
[GPLv3]: https://www.gnu.org/licenses/gplv3.html (GNU General Public License Version 3)

[keep-a-changelog]: https://keepachangelog.com/ (keep a changelog)
[common-changelog]: https://common-changelog.org/ (Common Changelog)
[semantic-versioning]: https://semver.org/ (Semantic Versioning)

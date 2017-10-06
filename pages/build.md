[Back to Main page](http://DragonJoker.github.com/Castor3D)

|         Server     | Result |
|:------------------:|--------|
|  Travis CI (Linux) | [![Build Status](https://travis-ci.org/DragonJoker/Castor3D.svg?branch=development)](https://travis-ci.org/DragonJoker/Castor3D) |
| AppVeyor (Windows) | [![Build Status](https://ci.appveyor.com/api/projects/status/github/DragonJoker/castor3d?branch=development&svg=true)](https://ci.appveyor.com/project/DragonJoker/castor3d) |


Building Castor3D
=================

Castor3D uses CMake to build project files.
You just need to launch CMake to build your preferential project file.

Tested compilers are:
- Visual Studio 2017
- gcc 5.3
- gcc 6.1

Projects
--------

**Main projects**

- CastorUtils depends on FreeImage and FreeType, and Zlib.
- Castor3D is based on CastorUtils.
- GuiCommon depends on wxWidgets (from 2.9.5, currently 3.0).
- CastorViewer depends on GuiCommon, Castor3D and wxWidgets.

**Plug-ins**

- Renderers
  - GlRenderSystem depends on OpenGL (not GLU).

- Importers
  - ASE: Depend on Castor3D.
  - 3DS: Depend on Castor3D.
  - LWO: Depend on Castor3D.
  - MD2: Depend on Castor3D.
  - MD3: Depend on Castor3D.
  - ASSIMP: Depends on Castor3D and Assimp.
  - PLY: Depend on Castor3D. 
  - OBJ: Depend on Castor3D. 
  - FBX: Depend on Castor3D. 

- Dividers
  - Loop: Depend on Castor3D.
  - Phong: Depend on Castor3D.
  - PN-Triangles: Depend on Castor3D.

- PostEffects
  - Bloom: Depend on Castor3D.
  - GrayScale: Depend on Castor3D.
  - FXAA: Depend on Castor3D.

- Generators
  - DiamondSquareTerrain: Depend on Castor3D.

- Generic
  - CastorGUI: Depend on Castor3D.

- ToneMappings
  - LinearToneMapping: Depend on Castor3D.
  - HaarmPieterDuikerToneMapping: Depend on Castor3D.
  - HejlBurgessDawsonToneMapping: Depend on Castor3D.
  - ReinhardToneMapping: Depend on Castor3D.

**Test projects**

- CastorTest: Base library for testing, depends on CastorUtils.
- CastorUtilsTest: Allows you to run unit and performance tests for CastorUtils, depends on CastorUtils and CastorTest.
- TestRenderSystem: A dummy render system plugin, used to run tests on Castor3D without any renderer, depends on Castor3D.
- Castor3DTest: Allows you to run unit tests for Castor3D, depends on Castor3D and CastorTest.
- GlRenderSystemTest: Allows you to run unit tests for GlRenderSystem, depends on Castor3D, GlRenderSystem and CastorTest.

**Other**

- ImgConverter : Allows you to convert any image file type to XPM or ICO, depends on wxWidgets.

Documentation for CastorUtils and Castor3D can be generated using Doxygen.

Options
-------

On GNU/Linux the default install directory is in /usr/local.

You can change this directory by changing the CMAKE_INSTALL_PREFIX value.

The sample, tool and test applications are installed in install_dir/bin/

The main libraries (CastorUtils and Castor3D) are installed in install_dir/lib/ on GNU/Linux, and in install_dir/bin/ on Windows.

The plug-ins are installed in install_dir/lib/Castor3D/

The build is configurable through CMake using the following options:
- **PROJECTS_BINARIES_OUTPUT_DIR**

    Used to specify the root directory for compiled binaries.
    Inside this folder, the bin, lib, and share directories will be created,
    and binaries will be put in the right folders.
    Some files will also be copied in share directory, that are used by the
    projects.
    On Windows, the external DLLs will be copied in bin folder, if they are
    found.

- **PROJECTS_USE_PRECOMPILED_HEADERS**

    Used to enable/disable the use of Precompiled Headers for the compilation.

- **PROJECTS_USE_PRETTY_PRINTING**

    If it is enabled, and if A-Style is found, new test projects will be
    created, allowing the run of A-Style over each project's source file.

- **CASTOR_BUILD_CASTOR3D**

    This special option allows you to disable build of Castor3D, and every
    other project depending on it, resulting in the build of CastorUtils and
    CastorUtilsTest only.

- **CASTOR_BUILD_PLUGINS**

    Another special option allowing you to disable the build of plug-ins.

- **CASTOR_BUILDGRP_DIVIDER**

    Enables the build of Divider plug-ins.

- **CASTOR_BUILDGRP_GENERIC**

    Enables the build of Generic plug-ins.

- **CASTOR_BUILDGRP_IMPORTER**

    Enables the build of Importer plug-ins.

- **CASTOR_BUILDGRP_INTEROP**

    Enables the build of Bindings.

- **CASTOR_BUILDGRP_POSTFX**

    Enables the build of PostEffect plug-ins.

- **CASTOR_BUILDGRP_SAMPLE**

    Enables the build of sample applications.

- **CASTOR_BUILDGRP_SETUP**

    Allows you to build setup projects.

- **CASTOR_BUILDGRP_TECHNIQUE**

    Allows you to build Technique plug-ins (note that DirectRenderTechnique is
    always built).

- **CASTOR_BUILDGRP_TEST**

    Allows you to build test applications.

- **CASTOR_BUILDGRP_TONEMAPS**

    Allows you to build ToneMapping plug-ins (note that LinearToneMapping is
    always built).

- **CASTOR_BUILDGRP_TOOL**

    Enables the build of tools ()applications and tests).

- **CASTOR_BUILD_DIVIDER_(NAME)**

    Enables the build of (NAME) divider plug-in.

- **CASTOR_BUILD_GENERIC_(NAME)**

    Enables the build of (NAME) generic plug-in.

- **CASTOR_BUILD_IMPORTER_(NAME)**
    Enables the build of (NAME) importer plug-in.

- **CASTOR_BUILD_INTEROP_(NAME)**

    Enables the build of (NAME) binding.

- **CASTOR_BUILD_POSTFX_(NAME)**

    Enables the build of (NAME) post-effect plug-in.

- **CASTOR_BUILD_RENDERER_(NAME)**

    Enables the build of (NAME) renderer plug-in.

- **CASTOR_BUILD_SAMPLE_(NAME)**

    Enables the build of (NAME) sample application.

- **CASTOR_BUILD_TECHNIQUE_(NAME)**

    Enables the build of (NAME) technique plug-in.

- **CASTOR_BUILD_TONEMAP_(NAME)**

    Enables the build of (NAME) tone mapping plug-in.

- **CASTOR_BUILD_TEST_(NAME)**

    Enables the build of (NAME) test application.

- **CASTOR_FORCE_ALL_IMPORTERS**

    Forces the build of every importer plug-in, even though AssimpImporter is
    built.

- **CASTOR_USE_SSE2**

    Enables use of SSE2 instructions for Point4f and Matrix4x4f operations.
adwaita-qt
==========

A native style to bend Qt4 and Qt5 applications to look like they belong into GNOME Shell.

![Widget Factory](/screenshots/widgets.png)

![KCalc](/screenshots/kcalc.png)

## How to compile

The project uses the standard CMake buildsystem.

It is advised to override the `LIB_SUFFIX` CMake variable to handle the `/usr/lib` vs `/usr/lib64` installation correctly.

So for example, when packaging a 64bit package, the process would be similar to this:

```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DLIB_SUFFIX=64 ..
make
make install
```

## Usage

After install, you'll be able to either set the theme as your default via your DE's tools (like `systemsettings` or `qt-config`) or start your qt applications with the `-style adwaita` parameter.

# magnifying-glass

Simple full screen magnifier for Windows.

**⚠️ ATTENTION!!!**

This program does not supporting mouse cursor position correction after zoom.
So, you must to use it for screen scale only without mouse clicks.
Max zoom factor: X16

## 🛠 Dependencies

This project hasn't external dependencies. Clear STL and Windows API.

## 🖼️ Program appearance

<p align="center">
	<img align="center" src="https://github.com/ClassZak/magnifying-glass/blob/master/screenshot1.png"/>
</p>
<p align="center">
	<img align="center" src="https://github.com/ClassZak/magnifying-glass/blob/master/screenshot2.png"/>
</p>

## 🏗 🚀 Building and launching

1. Clone and build

```batch
git clone https://github.com/ClassZak/magnifying-glass
cd magnifying-glass
cmake --preset=<chosen_by_you_preset>
cmake --build build/
```

Actual presets you can see in CMakePresets.txt

2. Launch
```batch
.\build\magnifying_glass.exe
```

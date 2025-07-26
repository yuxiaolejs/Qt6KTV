#!/bin/bash
rm -rf ./build

export QT_PATHH=/home/unics/Qt/6.9.1/gcc_64

mkdir -p build
pushd build
cmake .. -DCMAKE_PREFIX_PATH=$QT_PATHH
make -j$(nproc)
wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
chmod +x linuxdeploy-*.AppImage
cp ../icon.png ./icon.png

export PATH=$QT_PATHH/bin:$PATH
export LD_LIBRARY_PATH=$QT_PATHH/lib:$LD_LIBRARY_PATH
export QT_PLUGIN_PATH=$QT_PATHH/plugins
export QML2_IMPORT_PATH=$QT_PATHH/qml

cat <<EOF > VideoPlayer.desktop
[Desktop Entry]
Type=Application
Name=VideoPlayer
Exec=VideoPlayer
Icon=icon
Categories=Utility;
EOF

./linuxdeploy-x86_64.AppImage \
  --appdir AppDir \
  -e VideoPlayer \
  -d VideoPlayer.desktop \
  -i icon.png \
  --output appimage \
  --plugin qt

popd
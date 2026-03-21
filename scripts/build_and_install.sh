#!/bin/zsh

# OmniProjector macOS Build & Install Script
# 腳本說明：自動編譯插件並安裝到當前使用者的 OBS 插件目錄

# 1. 取得專案根目錄
PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
export PROJECT_ROOT
cd "$PROJECT_ROOT"

# 2. 定義安裝路徑
OBS_PLUGIN_DIR="$HOME/Library/Application Support/obs-studio/plugins"
PLUGIN_NAME="omni-projector"

echo "🚀 開始編譯 OmniProjector..."

# 3. 執行 CMake 配置 (使用 macOS preset)
cmake --preset macos

if [ $? -ne 0 ]; then
    echo "❌ CMake 配置失敗"
    exit 1
fi

# 4. 執行編譯 (使用 macOS preset)
cmake --build --preset macos

if [ $? -ne 0 ]; then
    echo "❌ 編譯失敗"
    exit 1
fi

echo "✅ 編譯成功！正在安裝到 OBS..."

# 5. 建立插件目錄（如果不存在）
mkdir -p "$OBS_PLUGIN_DIR"

# 6. 尋找產出的 .plugin 檔 (Ninja 通常直接在 binaryDir 下)
SOURCE_BUNDLE="$PROJECT_ROOT/build_macos/$PLUGIN_NAME.plugin"

if [ ! -d "$SOURCE_BUNDLE" ]; then
    # 嘗試在子目錄中尋找
    SOURCE_BUNDLE=$(find "$PROJECT_ROOT/build_macos" -name "$PLUGIN_NAME.plugin" -type d | head -n 1)
fi

if [ -z "$SOURCE_BUNDLE" ] || [ ! -d "$SOURCE_BUNDLE" ]; then
    echo "❌ 找不到編譯出的插件包: $SOURCE_BUNDLE"
    exit 1
fi

# 7. 備份舊插件並安裝新插件
rm -rf "$OBS_PLUGIN_DIR/$PLUGIN_NAME.plugin"
cp -R "$SOURCE_BUNDLE" "$OBS_PLUGIN_DIR/"

echo "🎉 安裝完成！"
echo "📍 插件路徑: $OBS_PLUGIN_DIR/$PLUGIN_NAME.plugin"
echo ""
echo "💡 如何使用："
echo "1. 重啟 OBS Studio。"
echo "2. 在頂部選單選擇「檢視 (View)」->「浮動視窗 (Docks)」。"
echo "3. 點擊「OmniProjector」開啟矩陣切換面板。"

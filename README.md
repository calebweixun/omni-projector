# OmniProjector - OBS Studio 插件

一個強大且自動化的 OBS 投影管理工具，讓您可以一鍵開啟或管理多個投影視窗。

## 功能特色

*   **自動化投影**：預先設定好「來源/場景」與「顯示器」的對應關係。
*   **一鍵開啟 (Project All)**：點擊一下即可按照設定，將所有選定的畫面投影到指定螢幕。
*   **一鍵全關 (Close All)**：快速關閉所有開啟中的投影視窗，保持桌面整潔。
*   **靈活配置**：
    *   支援 **輸出 (Program)** 畫面投影。
    *   自動分類 **場景 (Scenes)** 與 **來源 (Sources)**。
    *   刪除設定時，自動關閉該筆投影視窗。
*   **跨平台支援**：支援 Windows 與 macOS。

## 使用方式

1.  **開啟面板**：安裝後，在 OBS 頂部選單的「視窗 (Windows)」->「停駐視窗 (Docks)」中找到 `OmniProjector`。
2.  **添加對應**：
    *   點擊「＋ 添加投影對應 (Add)」。
    *   從左側下拉選單選擇要投影的內容（輸出畫面、場景或來源）。
    *   從右側下拉選單選擇目標螢幕。
3.  **執行投影**：
    *   單筆：點擊該列右側的「Go」。
    *   批量：點擊底部的「全部開啟 (Project All)」。
4.  **管理與關閉**：
    *   點擊單筆右側的「X」可刪除設定並關閉該投影。
    *   點擊底部的「一鍵全關 (Close All)」可關閉所有投影。

## 安裝說明

### 下載預編譯版本
1.  前往本專案的 [Releases](https://github.com/calebweixun/omni-projector/releases) 頁面。
2.  下載適合您系統的壓縮檔。
3.  將插件檔案放入 OBS 的 `plugins` 目錄中。

### 自行編譯
本專案基於 `obs-plugintemplate`，使用 CMake 進行建構。

```bash
# 複製專案
git clone https://github.com/calebweixun/omni-projector.git
cd omni-projector

# 使用 CMake 編譯 (具體步驟依系統而定)
cmake -B build
cmake --build build
```

## 開發者資訊
本插件旨在解決多螢幕直播環境下，手動開啟多個投影視窗的繁瑣流程。
如有任何問題或功能需求，歡迎提交 Issue。

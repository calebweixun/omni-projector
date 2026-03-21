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
*   **面板管理**：支援從 OBS 「停駐視窗」選單或「工具」選單重新開啟面板。
*   **跨平台支援**：支援 Windows 與 macOS。

## 使用方式

1.  **開啟面板**：
    *   方式 A：在 OBS 頂部選單的「視窗 (Windows)」->「停駐視窗 (Docks)」中勾選 `OmniProjector`。
    *   方式 B：若面板不小心關閉且在停駐選單找不到，可點擊「工具 (Tools)」-> `OmniProjector 投影配置`。
2.  **添加對應**：
    *   點擊「＋ 添加投影對應 (Add)」。
    *   從左側下拉選單選擇投影內容（輸出畫面、場景或來源）。
    *   從右側下拉選單選擇目標顯示器。
3.  **執行投影**：
    *   單筆：點擊該列右側的「Go」。
    *   批量：點擊底部的「全部開啟 (Project All)」。
4.  **管理與關閉**：
    *   點擊單筆右側的「X」可刪除設定並自動關閉該投影視窗。
    *   點擊底部的「一鍵全關 (Close All)」可關閉所有本插件開啟的投影畫面。

## 版本更新與下載 (Release)

本專案配置了自動化編譯 (CI/CD)，若要發布正式版本：
1.  **推送標籤**：在 Git 中推送符合語義化版本格式的標籤（例如 `1.0.0` 或 `0.2.1-beta1`）。
    ```bash
    git tag 1.0.0
    git push origin 1.0.0
    ```
2.  **自動發布**：GitHub Actions 會自動偵測到標籤，編譯所有平台的二進位檔，並自動在 GitHub 倉庫建立一個 **Draft Release**（草稿發佈）。
3.  **確認發布**：前往 GitHub 的 Releases 頁面，確認草稿無誤後點擊「Publish release」，安裝檔就會公開供大眾下載。

## 自行編譯
本專案基於 `obs-plugintemplate`，使用 CMake 進行建構。

```bash
git clone https://github.com/calebweixun/omni-projector.git
cd omni-projector

# macOS (需要 Xcode)
cmake -G Xcode -B build
cmake --build build --config Release
```

## 開發者資訊
本插件旨在解決多螢幕直播環境下，手動開啟多個投影視窗的繁瑣流程。
如有任何問題或功能需求，歡迎提交 Issue。

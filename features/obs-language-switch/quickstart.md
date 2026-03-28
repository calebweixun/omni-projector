# Quickstart: 在本地測試 OBS 語言切換支援

前提：已安裝 OBS，並能以開發模式載入本地建置的 plugin。

1. 建置並安裝插件

```bash
./scripts/build_and_install.sh
# 或使用現有 CMake 流程：
mkdir -p build && cd build
cmake .. && make -j && ./scripts/build_and_install.sh
```

2. 啟動 OBS 並載入插件

- 開啟 OBS，確認 `omni-projector` 插件已載入（插件清單）。

3. 測試模式：跟隨 OBS

- 在插件設定中選擇 `languageMode = follow_obs`。
- 在 OBS 偏好設定中切換語言（例如從英文切到繁體中文），觀察插件介面字串是否更新。

4. 測試模式：覆寫

- 在插件設定中選擇 `languageMode = override` 並設定 `selectedLanguage`（例如 `zh-TW`），保存設定並重新載入插件（或等待即時更新），確認介面顯示所選語言。

5. 測試缺漏匯出

- 在設定頁面匯出缺漏清單，檢查輸出檔案（CSV/JSON），確認列出了缺少的翻譯鍵。

6. 進一步測試（開發）

- 若 OBS 無語言變更事件，使用本地 prototype（見 research.md 的建議）測試 Qt 層或輪詢方法的效果與延遲。
# Quickstart: 測試 obs-language-switch 功能（本地）

Prerequisites:
- 已安裝 OBS（可用於測試的版本）。
- 開發環境：macOS，已安裝 CMake 與相依編譯工具。

Build & Install (快速流程):

1. 在專案根目錄執行打包/安裝腳本（此專案含腳本）：

   ./scripts/build_and_install.sh

   - 若無此腳本可用，改用標準 CMake 流程：

   mkdir -p build_macos && cd build_macos
   cmake ..
   make -j
   手動將生成的插件檔案安裝到 OBS 插件資料夾（視平台而定）。

2. 啟動 OBS，載入 OmniProjector 插件。

Testing steps:

- 跟隨 OBS 模式 (follow_obs)
  1. 在插件設定中將 `languageMode` 設為 `跟隨 OBS`。
  2. 在 OBS 的設定中改變語言（例如從英文改為另一語系）。
  3. 確認插件 UI 在 2 秒內更新（或顯示更新狀態）。若無法即時更新，請查看 OBS 前端事件支援（見 research.md 的 A1）。

- 覆寫模式 (override)
  1. 在插件設定中將 `languageMode` 設為 `覆寫`，選擇 `selectedLanguage`。
  2. 儲存設定並重啟 OBS 或重新載入插件（視實作情況），確認 UI 使用選擇語系。

- 缺漏匯出
  1. 在狀態鍵值缺漏時使用匯出功能（設定或管理介面）產生缺漏清單（JSON/CSV）。
  2. 檢視匯出檔案以確認列出缺漏鍵。

Notes:
- 若 research.md 中的 A1 未解決，請採用備選的偵測方法（Qt 語系事件或短輪詢）。

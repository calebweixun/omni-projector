# Implementation Plan: obs-language-switch

## Summary

分解實作任務以支援 OBS 原生語言切換，初步估時 3–5 人日（含設計、實作與測試）。

## Tasks

1. Confirm OBS language-change API (Research) — 0.5d
   - 查閱 OBS plugin API，確認是否有 runtime 語言變更事件或 callback。
   - 若無，評估 Qt signal vs. 輪詢的可行性與延遲。

2. Design localization loader (Design) — 0.5d
   - 定義鍵優先與合併流程（OBS > plugin），鍵對應規則與回退行為。
   - 定義本地 translation bundle 存放格式與載入路徑。

3. Implement PluginSettings UI (Impl) — 0.5d
   - 新增 `languageMode` 與 `selectedLanguage` 設定 UI，並持久化至現有設定儲存。

4. Implement runtime loader & apply (Impl) — 1.0d
   - 在接收到語言變更（或偵測到變化）時載入 bundle，更新所有 `UIComponent` 文本並重繪 UI。
   - 處理回退與缺漏記錄。

5. Implement missing-keys export (Impl) — 0.5d
   - 產生 `LocalizationReport`，支援 CSV/JSON 匯出，提供設定 UI 的匯出按鈕。

6. Tests & QA (Test) — 0.5–1.0d
   - 實機測試跟隨 OBS 與覆寫模式、缺漏匯出、重載行為、edge cases（rapid changes）。

7. Documentation (Docs) — 0.5d
   - 更新 README / changelog，加入 quickstart 與維運註記。

## Dependencies

- 需確認 OBS 是否提供語言變更事件（Task 1）。
- 需現有設定儲存機制支援新增欄位（PluginSettings）。
# Implementation Plan: obs-language-switch

Branch: obs-language-switch

Summary: 實作 OBS 原生語言跟隨與覆寫設定、合併翻譯來源、缺漏回報匯出與相關 UI。

Tasks:

1. Confirm OBS language-change capability (A1) — investigate OBS frontend API / upstream docs. (owner: eng)
2. Design merged localization loader: load OBS bundle (if available) + plugin bundle, merge with precedence. (owner: eng)
3. Implement `PluginSettings` UI: `languageMode` + `selectedLanguage` + export path. (owner: eng)
4. Implement runtime detection and update flow:
   - If OBS supports event → subscribe and refresh UI strings.
   - Else → implement Qt-based listener or short polling with debounce. (owner: eng)
5. Implement missing-keys logger and local export (JSON/CSV). (owner: eng)
6. Add unit/integration tests: settings persistence, merge precedence, export format. (owner: qa)
7. Documentation: update README / quickstart + release notes. (owner: docs)
8. Add translations / update translation workflow and maintainers. (owner: i18n)

Est. effort: 3–5 dev days (investigate A1 + implement core flows).

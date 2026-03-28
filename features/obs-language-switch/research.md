# Research: OBS 語言切換支援 決策紀錄

## 決策摘要

- 決策: 採用合併優先策略（OBS > 插件），若 OBS 未提供特定翻譯鍵則使用插件內建翻譯。理由：兼顧與 OBS 主介面一致性與插件字串完整性。
- 維護策略: 採混合維護——插件 repo 維護專屬字串與補缺，OBS upstream 維護通用字串；建立定期同步檢查流程。
- 缺漏回報: 預設提供本地可匯出的缺漏清單（CSV/JSON），以便人工翻譯流程；允許額外的回報選項由設定開啟（需用戶同意）。

## 重要未解/風險

- 未解: 是否存在 OBS 原生「語言變更」事件或 API（需確認 OBS upstream 文件或查詢 OBS plugin API）。若不存在，可能需透過 Qt 層面的訊號或輪詢觀察 `app` 語系設定。
- 風險: 若 OBS 的翻譯鍵命名與插件不同步，合併策略需明確鍵對應表與回退流程。

## 建議後續研究任務

1. 查閱 OBS plugin API 文件：搜尋是否有語言變更事件或對應 callback（Target: 確認是否可監聽 runtime 語言變更）。
2. 檢查現有 OBS 與插件翻譯鍵命名規則，比對常用字串，產生鍵對應建議表。 
3. 實作一個輕量 prototype：在本地 OBS 環境中測試以三種方法偵測語言變更：OBS event、Qt 設定變更通知、輪詢系統/OBS 設定；評估延遲與穩定性。
4. 設計缺漏匯出的格式（建議 CSV + JSON）與 UI 流程（設定頁面匯出按鈕）。

## 決策記錄範例

- Decision: Merge-priority localization
  - Rationale: Maintain UI consistency with OBS while ensuring plugin completeness.
  - Alternatives considered: Plugin-only; OBS-only (rejected due to coverage and update latency).
# Research: obs-language-switch — 決策與說明

Decision: 翻譯來源採用「合併優先」策略（OBS 原生 > 插件內建），並提供管理員可在設定中強制以插件翻譯為準（覆寫）。

Rationale: 符合規格 FR-005，能在大多數情況下利用 OBS 已有翻譯覆蓋通用字串，同時保留插件對專屬字串的控制與修補機制；管理員覆寫可解決 OBS 翻譯不符合需求時的情況。

Alternatives considered:
- 強制使用插件翻譯（會造成與 OBS 翻譯不同步的維運負擔）
- 只使用 OBS 翻譯（插件專屬字串缺失風險高）

Decision: 缺漏回報採用「本地可匯出清單（file export）」為預設，並提供手動上傳或外部整合點（未實作）作為選項。

Rationale: 符合 FR-005b 與隱私考量（預設不自動上傳）。

Decision: 語言變更偵測策略 — 標記為 NEEDS CLARIFICATION（需向 OBS upstream 或文件確認是否有明確的語言變更事件）。

Rationale: 現有 repo 展示使用 `OBS_MODULE_USE_DEFAULT_LOCALE`（參見 src/plugin-main.cpp），且 OBS 提供豐富的前端事件列表，但在本地檔案與 frontend API 文件中找不到明確的 `LANGUAGE_CHANGED` 前端事件名稱。若 OBS 有直接事件，應使用 `obs_frontend_add_event_callback` 處理變更；若沒有，備選方案為監聽 Qt 的語言/翻譯相關信號或在前端完成後觸發（例如在 OBS 完成 loading 後或透過 Qt 的 `QCoreApplication::installTranslator` 相關機制），或在 UI thread 執行短暫的輪詢檢查語系改變。

Action items from this research:
- A1: 向 OBS upstream 或官方文件確認：是否存在明確的「語言變更」前端事件或 API，可供插件即時接收（RESOLVE required）。
- A2: 若 A1 無法確認或不存在，採用備選實作：透過 Qt 層面的語系變更通知或在前端 UI 組件上實作輪詢/檢查（實作時需注意效能與 race-condition）。
- A3: 設計翻譯合併邏輯與覆寫設定 UI，並定義缺漏清單匯出格式（CSV/JSON）。

Unresolved: A1（是否有 OBS 原生語言變更事件） — 標記為 NEEDS CLARIFICATION。

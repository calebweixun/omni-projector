# Feature Specification: 支援 OBS 原生語言切換

**Feature Branch**: `obs-language-switch`  
**Created**: 2026-03-28  
**Status**: Draft  
**Input**: User description: "將插件介面調整為支持 OBS 原生語言切換功能"

## User Scenarios & Testing *(mandatory)*

### User Story 1 - 跟隨 OBS 語言 (Priority: P1)

作為一個使用者，當我改變 OBS 的語言設定時，我希望插件介面自動切換到相同語系，而不需重啟 OBS 或插件。

**Why this priority**: 提供一致的使用者體驗，減少混淆與設定成本。

**Independent Test**: 在已載入插件的 OBS 中，切換 OBS 主程式語言為另一語言，觀察插件文字在 2 秒內更新為目標語言（若設定為「跟隨 OBS」）。

**Acceptance Scenarios**:
1. **Given** 插件已載入且設定為「跟隨 OBS」， **When** 使用者在 OBS 設定中改變語言， **Then** 插件所有可翻譯的 UI 字串應在不重啟的情況下更新為 OBS 的語言。
2. **Given** 插件載入中且部分字串無對應翻譯， **When** 語言切換發生， **Then** 這些字串應回退到預設語言（例如英文），並可記錄缺失清單以供補齊。

---

### User Story 2 - 明確覆寫語言 (Priority: P2)

作為一個使用者，我希望在插件設定中有一個「語言模式」選項，可以選擇「跟隨 OBS」或「覆寫並選擇特定語言」，以便在需要時使用特定語系。

**Why this priority**: 支援高需求使用者與多語系工作流程，並確保回退機制。

**Independent Test**: 在插件設定中切換為「覆寫」，選擇另一語言，重啟或不重啟後確認介面顯示所選語言並且該選擇被儲存。

**Acceptance Scenarios**:
1. **Given** 使用者將語言模式設定為「覆寫」， **When** 選擇特定語言並儲存， **Then** 插件 UI 應使用該語言，且在重啟或重新載入插件後仍維持該選擇。

---

### User Story 3 - 翻譯缺漏回報與補齊 (Priority: P3)

作為一個維運者，我希望系統能在翻譯缺漏或不完整時給予清楚回報，方便追蹤並補上缺失字串。

**Why this priority**: 維持完整性與品質，減少使用者遇到混雜語言的情況。

**Independent Test**: 在包含已知缺漏的翻譯集下載入插件，驗證缺漏被記錄且可匯出或查看缺漏清單。

**Acceptance Scenarios**:
1. **Given** 某語言缺少翻譯鍵， **When** 該語系被選擇或跟隨 OBS， **Then** 系統應使用回退語言並將缺漏鍵加入回報或日誌中。

---

### Edge Cases

- OBS 在執行中頻繁切換語言（rapid changes）時的效能與一致性。
- 插件在某些平台或 OBS 版本上無法接收到語言變更通知的處理方式。
- 使用者選擇的覆寫語言在系統上並未安裝或不支援時的回退行為。

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: 插件 MUST 提供一個 `語言模式` 設定，選項至少包含 `跟隨 OBS` 與 `覆寫`（選擇特定語言）。
- **FR-002**: 當設定為 `跟隨 OBS` 時，插件 MUST 在 OBS 語言變更後自動更新介面字串，無需重啟插件或 OBS。
- **FR-003**: 當設定為 `覆寫` 且使用者選擇語言時，該選擇 MUST 被永久儲存並於後續啟動維持。
- **FR-004**: 插件 MUST 為缺少翻譯的字串提供可預期的回退語言（預設為英文），並記錄缺漏以便維運。
- **FR-005**: 插件 MUST 定義翻譯來源策略（例如優先使用 OBS 原生語系資源或使用插件內建翻譯）[NEEDS CLARIFICATION: 翻譯來源策略未指定 - 是否優先使用 OBS 提供的翻譯資源，或維持插件內部翻譯檔？]
- **FR-006**: 插件 MUST 在介面上提供語言切換相關的使用者提示或狀態（例如目前模式與所選語言）。
- **FR-007**: 在不支援某語系或無法取得翻譯時，插件 MUST 以可理解的方式回退並在設定或日誌中顯示問題狀態。

### Key Entities *(include if feature involves data)*

- **PluginSettings**: { languageMode: enum[follow_obs, override], selectedLanguage: string }
- **LocalizationBundle**: { languageCode: string, translations: map[key:string -> value:string] }
- **UIComponent**: 各可本地化元素（label、tooltip、button text）

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: 在設定為「跟隨 OBS」時，對於 95% 的常用 UI 字串，語言切換後在 2 秒內完成更新（或明確顯示為已更新）。
- **SC-002**: 使用者能在插件設定內切換到「覆寫」並選擇語言，且該選擇於 100% 的啟動情況下被儲存並套用。
- **SC-003**: 若翻譯缺漏，至少 90% 的缺漏能被自動記錄為可供維運匯出的清單。
- **SC-004**: 實作後用戶對語言一致性的問題回報在 30 天內下降至少 50%（相對於未實作前的基線，若可量化）。

## Assumptions

- OBS 對外提供變更語言的事件或 API，供第三方插件偵聽（若不存在，則需透過其他可靠機制偵測語言變更）。
- 插件已具備基礎的翻譯機制與翻譯鍵（或可延伸至支援新的語系檔）。
- 使用者接受在部分情況下以英文作為預設回退語言。
- [Assumption] 本次需求不包含新增大量翻譯工作的內容；若需新增多語翻譯，需另提翻譯任務。

## Notes / Next Steps

- 若接受此規格，下一步為確認 **FR-005 翻譯來源策略**（見上方 NEEDS CLARIFICATION），並列出優先支援之語系清單與現有翻譯覆蓋率評估。

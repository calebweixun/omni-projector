# Data Model: obs-language-switch

## Entities

- **PluginSettings**
  - languageMode: enum [follow_obs, override]
  - selectedLanguage: string (ISO code, nullable if follow_obs)
  - lastAppliedLanguage: string (ISO code)
  - exportMissingPath: string (user-specified path)

- **LocalizationBundle**
  - languageCode: string (ISO code)
  - translations: map<string, string> (key -> localized string)
  - source: enum [obs, plugin]

- **LocalizationReport**
  - generatedAt: timestamp
  - missingKeys: list<string>
  - bundleLanguage: string
  - exportFormat: enum [csv, json]

- **UIComponent**
  - id: string (唯一鍵)
  - defaultTextKey: string
  - currentText: string

## Relationships

- `PluginSettings.selectedLanguage` 對應 `LocalizationBundle.languageCode`。
- `LocalizationBundle` 與多個 `UIComponent` 關聯（bundle 提供 component 的 `currentText`）。

## Validation Rules

- `selectedLanguage` 必須為有效 ISO 語言代碼或空值（若 `languageMode == follow_obs`）。
- `LocalizationBundle.translations` 中每個 key 必須為非空字串。
- 匯出報告 `missingKeys` 可為空（表示無缺漏）。

## State transitions

- Mode 切換：`follow_obs` <-> `override`（當從 `follow_obs` 切換到 `override` 並選擇語言時，`selectedLanguage` 被存入設定）。
- 語言變更：收到語言變更事件後，載入對應 `LocalizationBundle`，套用到 `UIComponent`，若缺漏則記錄 `LocalizationReport`。
# Data Model: obs-language-switch

Entities:

- PluginSettings
  - languageMode: enum[follow_obs, override] (required)
  - selectedLanguage: string | null (ISO language code, required when languageMode == override)
  - exportMissingPath: string | null (optional, path to local export)

- LocalizationBundle
  - languageCode: string (ISO, e.g. en-US)
  - translations: map<string,string> (key -> localized string)
  - source: enum[obs, plugin] (origin of translations)

- LocalizationReport
  - generatedAt: timestamp
  - languageCode: string
  - missingKeys: array<string>
  - format: enum[json,csv]

- UIComponent (conceptual)
  - id: string
  - textKey: string
  - currentText: string

Relationships:
- PluginSettings holds current `languageMode` and (optionally) `selectedLanguage`.
- LocalizationBundle objects are loaded/merged at runtime; merge precedence: OBS bundle (if present) overrides plugin bundle keys.

Validation rules:
- `languageMode` MUST be one of `follow_obs` or `override`.
- If `languageMode == override`, `selectedLanguage` MUST be non-empty and a valid language code supported by available bundles.
- `LocalizationBundle.translations` keys MUST be stable identifiers used across UI components.

State transitions:
- On settings change to `follow_obs`: component text keys should refresh from merged bundles.
- On settings change to `override`: load plugin bundles for `selectedLanguage`, apply merge, persist `PluginSettings`.

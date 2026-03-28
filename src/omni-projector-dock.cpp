#include "omni-projector-dock.hpp"
#include "omni-projector-manager.hpp"
#include "localization.hpp"
#include <obs-frontend-api.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QScrollArea>
#include <QMainWindow>
#include <QScreen>
#include <QGuiApplication>

OmniProjectorDock::OmniProjectorDock(QWidget *parent) : QDockWidget(parent)
{
	setObjectName("OmniProjectorDock");
	std::string title = LocalizationManager::Translate("dock.title");
	if (title.empty())
		title = "OmniProjector";
	setWindowTitle(QString::fromStdString(title));

	QWidget *centralWidget = new QWidget(this);
	QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

	// 頂部：添加按鈕與重新整理
	QHBoxLayout *topLayout = new QHBoxLayout();
	QPushButton *addBtn = new QPushButton(QString::fromStdString(
		LocalizationManager::Translate("button.add").empty() ? "＋ 添加投影對應 (Add)"
								     : LocalizationManager::Translate("button.add")));
	QPushButton *refreshBtn =
		new QPushButton(QString::fromStdString(LocalizationManager::Translate("button.refresh").empty()
							       ? "↺ 重新整理 (Refresh)"
							       : LocalizationManager::Translate("button.refresh")));
	topLayout->addWidget(addBtn);
	topLayout->addWidget(refreshBtn);
	mainLayout->addLayout(topLayout);

	// 中間：捲動區域
	QScrollArea *scrollArea = new QScrollArea(this);
	scrollArea->setWidgetResizable(true);
	QWidget *listWidget = new QWidget();
	listLayout = new QVBoxLayout(listWidget);
	listLayout->setAlignment(Qt::AlignTop);
	scrollArea->setWidget(listWidget);
	mainLayout->addWidget(scrollArea);

	// 底部：批次控制
	QHBoxLayout *bottomLayout = new QHBoxLayout();
	QPushButton *projectAllBtn =
		new QPushButton(QString::fromStdString(LocalizationManager::Translate("button.project_all").empty()
							       ? "全部開啟 (Project All)"
							       : LocalizationManager::Translate("button.project_all")));
	QPushButton *stopAllBtn =
		new QPushButton(QString::fromStdString(LocalizationManager::Translate("button.stop_all").empty()
							       ? "一鍵全關 (Close All)"
							       : LocalizationManager::Translate("button.stop_all")));
	bottomLayout->addWidget(projectAllBtn);
	bottomLayout->addWidget(stopAllBtn);
	mainLayout->addLayout(bottomLayout);

	setWidget(centralWidget);

	// 初始載入
	RefreshMappings();

	// 綁定事件
	connect(addBtn, &QPushButton::clicked, [this]() {
		OmniProjectorManager::Get().AddMapping("", 0);
		RefreshMappings();
	});

	connect(refreshBtn, &QPushButton::clicked, [this]() { RefreshMappings(); });

	connect(projectAllBtn, &QPushButton::clicked, []() { OmniProjectorManager::Get().ProjectAll(); });

	connect(stopAllBtn, &QPushButton::clicked, []() { OmniProjectorManager::Get().StopAllProjections(); });
}

OmniProjectorDock::~OmniProjectorDock() {}

void OmniProjectorDock::RefreshMappings()
{
	// 清空舊列表
	QLayoutItem *item;
	while ((item = listLayout->takeAt(0)) != nullptr) {
		if (item->widget())
			delete item->widget();
		delete item;
	}

	const auto &mappings = OmniProjectorManager::Get().GetMappings();
	auto sourceGroups = OmniProjectorManager::Get().GetAvailableSources();
	auto screens = QGuiApplication::screens();
	int monitorCount = screens.size();

	for (int i = 0; i < (int)mappings.size(); ++i) {
		const auto &entry = mappings[i];

		QWidget *row = new QWidget();
		QHBoxLayout *rowLayout = new QHBoxLayout(row);
		rowLayout->setContentsMargins(5, 2, 5, 2);

		QComboBox *sourceCombo = new QComboBox();

		// 加入「輸出」與分隔線
		sourceCombo->addItem("輸出");
		sourceCombo->insertSeparator(sourceCombo->count());

		// 加入場景
		for (const auto &s : sourceGroups.scenes)
			sourceCombo->addItem(QString::fromStdString(s));
		sourceCombo->insertSeparator(sourceCombo->count());

		// 加入來源
		for (const auto &s : sourceGroups.sources)
			sourceCombo->addItem(QString::fromStdString(s));

		sourceCombo->setCurrentText(QString::fromStdString(entry.source_name));

		QComboBox *monitorCombo = new QComboBox();
		for (int m = 0; m < monitorCount; ++m) {
			QString name = screens[m]->name();
			monitorCombo->addItem(QString("[%1] %2").arg(m).arg(name));
		}
		monitorCombo->setCurrentIndex(entry.monitor_index);

		QPushButton *goBtn =
			new QPushButton(QString::fromStdString(LocalizationManager::Translate("button.go").empty()
								       ? "Go"
								       : LocalizationManager::Translate("button.go")));
		QPushButton *delBtn =
			new QPushButton(QString::fromStdString(LocalizationManager::Translate("button.del").empty()
								       ? "X"
								       : LocalizationManager::Translate("button.del")));

		rowLayout->addWidget(sourceCombo);
		rowLayout->addWidget(monitorCombo);
		rowLayout->addWidget(goBtn);
		rowLayout->addWidget(delBtn);

		listLayout->addWidget(row);

		// 更新與存檔事件
		connect(sourceCombo, &QComboBox::currentTextChanged, [i, sourceCombo, monitorCombo]() {
			OmniProjectorManager::Get().UpdateMapping(i, sourceCombo->currentText().toStdString(),
								  monitorCombo->currentIndex());
		});

		connect(monitorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
			[i, sourceCombo, monitorCombo](int index) {
				OmniProjectorManager::Get().UpdateMapping(i, sourceCombo->currentText().toStdString(),
									  index);
			});

		connect(goBtn, &QPushButton::clicked, [sourceCombo, monitorCombo]() {
			std::string sourceName = sourceCombo->currentText().toStdString();
			int mIdx = monitorCombo->currentIndex();
			OmniProjectorManager::Get().StartProjection(sourceName, mIdx);
		});

		connect(delBtn, &QPushButton::clicked, [this, i]() {
			OmniProjectorManager::Get().RemoveMapping(i);
			RefreshMappings();
		});
	}
}

static OmniProjectorDock *g_omniProjectorDock = nullptr;

static void ShowOmniProjectorDock(void *data)
{
	(void)data;
	if (g_omniProjectorDock) {
		g_omniProjectorDock->show();
		g_omniProjectorDock->raise();
	}
}

void OmniProjectorDock::Register()
{
	QMainWindow *mainWindow = static_cast<QMainWindow *>(obs_frontend_get_main_window());
	g_omniProjectorDock = new OmniProjectorDock(mainWindow);

	// 使用 OBS 前端 API 註冊 Dock，讓它能出現在「停駐視窗」選單
	// ID 必須與 ObjectName("OmniProjectorDock") 完全一致
	obs_frontend_add_custom_qdock("OmniProjectorDock", g_omniProjectorDock);

	// 加入到「工具 (Tools)」選單，當作備用的開啟方式
	obs_frontend_add_tools_menu_item("OmniProjector 投影配置", ShowOmniProjectorDock, nullptr);
}

void OmniProjectorDock::RefreshAll()
{
	if (g_omniProjectorDock) {
		g_omniProjectorDock->RefreshMappings();
	}
}

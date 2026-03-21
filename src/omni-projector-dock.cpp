#include "omni-projector-dock.hpp"
#include "omni-projector-manager.hpp"
#include <obs-frontend-api.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QScrollArea>
#include <QMainWindow>
#include <QScreen>
#include <QGuiApplication>

OmniProjectorDock::OmniProjectorDock(QWidget *parent)
    : QDockWidget(parent)
{
    setObjectName("OmniProjectorDock");
    setWindowTitle("OmniProjector");
    
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // 頂部：添加按鈕
    QPushButton *addBtn = new QPushButton("＋ 添加投影對應 (Add Mapping)");
    mainLayout->addWidget(addBtn);
    
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
    QPushButton *projectAllBtn = new QPushButton("全部開啟 (Project All)");
    QPushButton *stopAllBtn = new QPushButton("一鍵全關 (Close All)");
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
    
    connect(projectAllBtn, &QPushButton::clicked, []() {
        OmniProjectorManager::Get().ProjectAll();
    });
    
    connect(stopAllBtn, &QPushButton::clicked, []() {
        OmniProjectorManager::Get().StopAllProjections();
    });
}

OmniProjectorDock::~OmniProjectorDock()
{
}

void OmniProjectorDock::RefreshMappings()
{
    // 清空舊列表
    QLayoutItem *item;
    while ((item = listLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }
    
    auto screens = QGuiApplication::screens();
    int monitorCount = screens.size();
    
    for (int i = 0; i < (int)mappings.size(); ++i) {
        const auto& entry = mappings[i];
        
        QWidget *row = new QWidget();
        QHBoxLayout *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(5, 2, 5, 2);
        
        QComboBox *sourceCombo = new QComboBox();
        for (const auto& s : sources) sourceCombo->addItem(QString::fromStdString(s));
        sourceCombo->setCurrentText(QString::fromStdString(entry.source_name));
        
        QComboBox *monitorCombo = new QComboBox();
        for (int m = 0; m < monitorCount; ++m) {
            QString name = screens[m]->name();
            monitorCombo->addItem(QString("[%1] %2").arg(m).arg(name));
        }
        monitorCombo->setCurrentIndex(entry.monitor_index);
        
        QPushButton *goBtn = new QPushButton("Go");
        QPushButton *delBtn = new QPushButton("Del");
        delBtn->setFixedWidth(40);
        
        rowLayout->addWidget(sourceCombo);
        rowLayout->addWidget(monitorCombo);
        rowLayout->addWidget(goBtn);
        rowLayout->addWidget(delBtn);
        
        listLayout->addWidget(row);
        
        // 更新與存檔事件
        connect(sourceCombo, &QComboBox::currentTextChanged, [i, sourceCombo, monitorCombo]() {
            OmniProjectorManager::Get().UpdateMapping(i, sourceCombo->currentText().toStdString(), monitorCombo->currentIndex());
        });
        
        connect(monitorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [i, sourceCombo, monitorCombo](int index) {
            OmniProjectorManager::Get().UpdateMapping(i, sourceCombo->currentText().toStdString(), index);
        });
        
        connect(goBtn, &QPushButton::clicked, [sourceCombo, monitorCombo]() {
            std::string sourceName = sourceCombo->currentText().toStdString();
            int mIdx = monitorCombo->currentIndex();
            obs_source_t *source = obs_get_source_by_name(sourceName.c_str());
            if (source) {
                OmniProjectorManager::Get().StartProjection(source, mIdx);
                obs_source_release(source);
            }
        });
        
        connect(delBtn, &QPushButton::clicked, [this, i]() {
            OmniProjectorManager::Get().RemoveMapping(i);
            RefreshMappings();
        });
    }
}

void OmniProjectorDock::Register()
{
    QMainWindow *mainWindow = static_cast<QMainWindow*>(obs_frontend_get_main_window());
    OmniProjectorDock *dock = new OmniProjectorDock(mainWindow);
    obs_frontend_add_dock(dock, "OmniProjector", nullptr, nullptr);
}

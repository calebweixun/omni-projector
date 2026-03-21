#include "omni-projector-dock.hpp"
#include "omni-projector-manager.hpp"
#include <obs-frontend-api.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QMainWindow>

OmniProjectorDock::OmniProjectorDock(QWidget *parent)
    : QDockWidget(parent)
{
    setObjectName("OmniProjectorDock");
    setWindowTitle("OmniProjector Matrix Switcher");
    
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // 矩陣表格
    QTableWidget *table = new QTableWidget(this);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"顯示器 (Monitor)", "內容來源 (Source)"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    int monitorCount = OmniProjectorManager::Get().GetMonitorCount();
    std::vector<std::string> sources = OmniProjectorManager::Get().GetAvailableSources();
    
    table->setRowCount(monitorCount);
    for (int i = 0; i < monitorCount; ++i) {
        // Monitor Label
        table->setItem(i, 0, new QTableWidgetItem(QString("Monitor %1").arg(i + 1)));
        
        // Source Selector + Action
        QWidget *cellWidget = new QWidget();
        QHBoxLayout *cellLayout = new QHBoxLayout(cellWidget);
        cellLayout->setContentsMargins(2, 2, 2, 2);
        
        QComboBox *combo = new QComboBox();
        for (const auto& s : sources) {
            combo->addItem(QString::fromStdString(s));
        }
        
        QPushButton *btn = new QPushButton("投影 (Project)");
        
        cellLayout->addWidget(combo);
        cellLayout->addWidget(btn);
        
        table->setCellWidget(i, 1, cellWidget);
        
        // 接連按鈕事件 (這裡可以使用 Lambda)
        connect(btn, &QPushButton::clicked, [i, combo]() {
            std::string sourceName = combo->currentText().toStdString();
            obs_source_t *source = obs_get_source_by_name(sourceName.c_str());
            if (source) {
                OmniProjectorManager::Get().StartProjection(source, i);
                obs_source_release(source);
            }
        });
    }
    
    mainLayout->addWidget(table);
    
    // 全場控制按鈕
    QPushButton *stopBtn = new QPushButton("停止所有投影 (Stop All)");
    mainLayout->addWidget(stopBtn);
    
    setWidget(centralWidget);
}

OmniProjectorDock::~OmniProjectorDock()
{
}

void OmniProjectorDock::Register()
{
    QMainWindow *mainWindow = static_cast<QMainWindow*>(obs_frontend_get_main_window());
    OmniProjectorDock *dock = new OmniProjectorDock(mainWindow);
    obs_frontend_add_dock(dock, "OmniProjector", nullptr, nullptr);
}

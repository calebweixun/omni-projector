#pragma once

#include <obs-frontend-api.h>
#include <QDockWidget>
#include <QVBoxLayout>

class OmniProjectorDock : public QDockWidget {
    Q_OBJECT

public:
    explicit OmniProjectorDock(QWidget *parent = nullptr);
    ~OmniProjectorDock() override;

    static void Register();

private:
    void RefreshMappings();
    QVBoxLayout *listLayout;
};

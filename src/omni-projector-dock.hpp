#pragma once

#include <obs-frontend-api.h>
#include <QDockWidget>

class OmniProjectorDock : public QDockWidget {
    Q_OBJECT

public:
    explicit OmniProjectorDock(QWidget *parent = nullptr);
    ~OmniProjectorDock() override;

    static void Register();
};

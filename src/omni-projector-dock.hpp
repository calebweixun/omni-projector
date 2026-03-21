#pragma once

#include <QDockWidget>
#include <QVBoxLayout>

class OmniProjectorDock : public QDockWidget {
	Q_OBJECT

public:
	explicit OmniProjectorDock(QWidget *parent = nullptr);
	~OmniProjectorDock();

	static void Register();

private:
	QVBoxLayout *listLayout;
	void RefreshMappings();
};

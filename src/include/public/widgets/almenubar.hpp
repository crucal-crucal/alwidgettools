#pragma once

#include <QMenuBar>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALMenu;

class CALWIDGETTOOLS_EXPORT CALMenuBar : public QMenuBar {
	Q_OBJECT

public:
	explicit CALMenuBar(QWidget* parent = nullptr);
	~CALMenuBar() override;

	QAction* addMenu(QMenu* menu);
	CALMenu* addMenu(const QString& title);
	CALMenu* addMenu(const QIcon& icon, const QString& title);
	CALMenu* addMenu(const ALIcon::AweSomeIcon& awesomeicon, const QString& title);

	QAction* addAweSomeIconAction(const ALIcon::AweSomeIcon& awesomeicon);
	QAction* addAweSomeIconAction(const ALIcon::AweSomeIcon& awesomeicon, const QString& text);
	QAction* addAweSomeIconAction(const ALIcon::AweSomeIcon& awesomeicon, const QString& text, const QKeySequence& shortcut);
};
} // namespace AL
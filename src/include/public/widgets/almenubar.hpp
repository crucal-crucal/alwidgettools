#pragma once

#include <QMenuBar>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALMenu;
class CALIconType;

class CALWIDGETTOOLS_EXPORT CALMenuBar : public QMenuBar {
	Q_OBJECT

public:
	explicit CALMenuBar(QWidget* parent = nullptr);
	~CALMenuBar() override;

	QAction* addMenu(QMenu* menu);
	CALMenu* addMenu(const QString& title);
	CALMenu* addMenu(const QIcon& icon, const QString& title);
	CALMenu* addMenu(const std::shared_ptr<CALIconType>& icon_type, const QString& title);
	QAction* addALIconAction(const std::shared_ptr<CALIconType>& icon_type);
	QAction* addALIconAction(const std::shared_ptr<CALIconType>& icon_type, const QString& text);
	QAction* addALIconAction(const std::shared_ptr<CALIconType>& icon_type, const QString& text, const QKeySequence& shortcut);
};
}

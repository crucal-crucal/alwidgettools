#pragma once

#include <QToolBar>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALToolBarPrivate;
class CALIconType;

class CALWIDGETTOOLS_EXPORT CALToolBar : public QToolBar {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALToolBar)

public:
	explicit CALToolBar(QWidget* parent = nullptr);
	explicit CALToolBar(const QString& title, QWidget* parent = nullptr);
	~CALToolBar() override;

	void setToolBarSpacing(int spacing) const;
	[[nodiscard]] int getToolBarSpacing() const;

	QAction* addALIconAction(const std::shared_ptr<CALIconType>& icon_type, const QString& text);
	QAction* addALIconAction(const std::shared_ptr<CALIconType>& icon_type, const QString& text, const QKeySequence& shortcut);

protected:
	const QScopedPointer<CALToolBarPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
}

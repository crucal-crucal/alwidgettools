#pragma once

#include <QToolBar>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALToolBarPrivate;

class CALWIDGETTOOLS_EXPORT CALToolBar : public QToolBar {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALToolBar)

public:
	explicit CALToolBar(QWidget* parent = nullptr);
	explicit CALToolBar(const QString& title, QWidget* parent = nullptr);
	~CALToolBar() override;

	void setToolBarSpacing(int spacing) const;
	[[nodiscard]] int getToolBarSpacing() const;

	QAction* addAweIconAction(const ALIcon::AweSomeIcon& awesomeicon, const QString& text);
	QAction* addAweIconAction(const ALIcon::AweSomeIcon& awesomeicon, const QString& text, const QKeySequence& shortcut);

protected:
	const QScopedPointer<CALToolBarPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
}

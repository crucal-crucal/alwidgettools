#pragma once

#include <QDockWidget>
#include <QLabel>
#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALText;
class CALIconButton;
class CALDockWidget;

class CALDockWidgetTitleBar final : public QWidget {
	Q_OBJECT

public:
	explicit CALDockWidgetTitleBar(QWidget* parent = nullptr);
	~CALDockWidgetTitleBar() override;

	Q_SLOT void slotFloatButtonClicked() const;
	Q_SLOT void slotCloseButtonClicked() const;

	void changeFloatButtonIcon(bool isFloating) const;

protected:
	friend class CALDockWidgetPrivate;

	void paintEvent(QPaintEvent* event) override;

private:
	ALThemeType::ThemeMode m_themeMode{};
	CALDockWidget* m_dockWidget{ nullptr };
	QLabel* m_iconLabel{ nullptr };
	CALText* m_titleLabel{ nullptr };
	CALIconButton* m_floatButton{ nullptr };
	CALIconButton* m_closeButton{ nullptr };

	void setVisibleFromFeatures(const QDockWidget::DockWidgetFeatures& features) const;
};
}

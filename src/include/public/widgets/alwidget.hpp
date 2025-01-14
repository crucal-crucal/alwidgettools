#pragma once

#include <QWidget>

#include "alappbar.hpp"
#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALWidgetPrivate;

class CALWIDGETTOOLS_EXPORT CALWidget : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALWidget)
	Q_PROPERTY(bool isStayTop READ getIsStayTop WRITE setIsStayTop NOTIFY sigIsStayTopChanged)
	Q_PROPERTY(bool isFixedSize READ getIsFixedSize WRITE setIsFixedSize NOTIFY sigIsFixedSizeChanged)
	Q_PROPERTY(bool isDefaultClosed READ getIsDefaultClosed WRITE setIsDefaultClosed NOTIFY sigIsDefaultClosedChanged)
	Q_PROPERTY(int appBarHeight READ getAppBarHeight WRITE setAppBarHeight NOTIFY sigAppBarHeightChanged)
	Q_PROPERTY(QColor appBarBackground READ getAppBarBackground WRITE setAppBarBackground NOTIFY sigAppBarBackgroundChanged)
	AL_TAKEOVER_NATIVEEVENT_H

public:
	explicit CALWidget(QWidget* parent = nullptr);
	~CALWidget() override;

	void moveToCenter();

	void setWindowButtonFlag(const ALAppBarType::ButtonFlag& buttonFlag, bool isEnable = true);
	void setWindowButtonFlags(const ALAppBarType::ButtonFlags& buttonFlags);
	[[nodiscard]] ALAppBarType::ButtonFlags getWindowButtonFlags() const;

	void setIsStayTop(bool isStayTop);
	[[nodiscard]] bool getIsStayTop() const;

	void setIsFixedSize(bool isFixedSize);
	[[nodiscard]] bool getIsFixedSize() const;

	void setIsDefaultClosed(bool isDefaultClosed);
	[[nodiscard]] bool getIsDefaultClosed() const;

	void setAppBarHeight(int appBarHeight);
	[[nodiscard]] int getAppBarHeight() const;

	void setAppBarBackground(const QColor& color);
	[[nodiscard]] QColor getAppBarBackground() const;

Q_SIGNALS:
	Q_SIGNAL void sigIsStayTopChanged();
	Q_SIGNAL void sigIsFixedSizeChanged();
	Q_SIGNAL void sigIsDefaultClosedChanged();
	Q_SIGNAL void sigAppBarHeightChanged();
	Q_SIGNAL void sigAppBarBackgroundChanged();
	Q_SIGNAL void sigRouteBackButtonClicked();
	Q_SIGNAL void sigNavigationButtonClicked();
	Q_SIGNAL void sigThemeChangeButtonClicked();
	Q_SIGNAL void sigCloseButtonClicked();

protected:
	const QScopedPointer<CALWidgetPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
}

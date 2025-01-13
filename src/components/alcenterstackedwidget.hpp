#pragma once

#include <QStackedWidget>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALCenterStackedWidget final : public QStackedWidget {
	Q_OBJECT

public:
	explicit CALCenterStackedWidget(QWidget* parent = nullptr);
	~CALCenterStackedWidget() override;

	Q_SLOT void slotThemeModeChanged(const ALThemeType::ThemeMode& mode);

	void setIsTransparent(bool isTransparent);
	[[nodiscard]] bool getIsTransparent() const;

	void setIsHasRadius(bool isHasRadius);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	ALThemeType::ThemeMode m_themeMode{};
	bool m_isTransparent{};
	bool m_isHasRadius{ true };
};
} // namespace AL
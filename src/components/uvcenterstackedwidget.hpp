#pragma once

#include <QStackedWidget>

#include "uvwidgettoolsdef.hpp"

class CUVCenterStackedWidget final : public QStackedWidget {
	Q_OBJECT

public:
	explicit CUVCenterStackedWidget(QWidget* parent = nullptr);
	~CUVCenterStackedWidget() override;

	Q_SLOT void slotThemeModeChanged(const UVThemeType::ThemeMode& mode);

	void setIsTransparent(bool isTransparent);
	[[nodiscard]] bool getIsTransparent() const;

	void setIsHasRadius(bool isHasRadius);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	UVThemeType::ThemeMode m_themeMode{};
	bool m_isTransparent{};
	bool m_isHasRadius{ true };
};

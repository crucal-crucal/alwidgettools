#pragma once

#include "alsingleton.hpp"
#include "alwidgettoolsdef.hpp"

#define UVTheme CUVThemeManager::instance()
#define UVThemeColor(mode, color) UVTheme->getThemeColor(mode, color)

class QPainter;
class CUVThemeManagerPrivate;

class CALWIDGETTOOLS_EXPORT CUVThemeManager : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVThemeManager)
	Q_DECLARE_PRIVATE(CUVThemeManager)

public:
	static CUVThemeManager* instance();

	void setThemeMode(const ALThemeType::ThemeMode& mode);
	[[nodiscard]] ALThemeType::ThemeMode getThemeMode() const;
	void drawEffectShadow(QPainter* painter, const QRect& widgetRect, const int& shadowBorderWidth, const int& borderRadius);
	void setThemeColor(const ALThemeType::ThemeMode& mode, const ALThemeType::ThemeColor& color, const QColor& newColor);
	[[nodiscard]] const QColor& getThemeColor(const ALThemeType::ThemeMode& mode, const ALThemeType::ThemeColor& color) const;

Q_SIGNALS:
	Q_SIGNAL void sigThemeModeChanged(const ALThemeType::ThemeMode& mode);

protected:
	const QScopedPointer<CUVThemeManagerPrivate> d_ptr{ nullptr };

	friend class CUVSingleton<CUVThemeManager>;

private:
	explicit CUVThemeManager(QObject* parent = nullptr);
	~CUVThemeManager() override;
};

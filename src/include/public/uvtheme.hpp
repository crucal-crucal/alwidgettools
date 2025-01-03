#pragma once

#include "uvsingleton.hpp"
#include "uvwidgettoolsdef.hpp"

#define UVTheme CUVThemeManager::instance()
#define UVThemeColor(mode, color) UVTheme->getThemeColor(mode, color)

class QPainter;
class CUVThemeManagerPrivate;

class CUVWIDGETTOOLS_EXPORT CUVThemeManager : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVThemeManager)
	Q_DECLARE_PRIVATE(CUVThemeManager)

public:
	static CUVThemeManager* instance();

	void setThemeMode(const UVThemeType::ThemeMode& mode);
	[[nodiscard]] UVThemeType::ThemeMode getThemeMode() const;
	void drawEffectShadow(QPainter* painter, const QRect& widgetRect, const int& shadowBorderWidth, const int& borderRadius);
	void setThemeColor(const UVThemeType::ThemeMode& mode, const UVThemeType::ThemeColor& color, const QColor& newColor);
	[[nodiscard]] const QColor& getThemeColor(const UVThemeType::ThemeMode& mode, const UVThemeType::ThemeColor& color) const;

Q_SIGNALS:
	Q_SIGNAL void sigThemeModeChanged(const UVThemeType::ThemeMode& mode);

protected:
	const QScopedPointer<CUVThemeManagerPrivate> d_ptr{ nullptr };

	friend class CUVSingleton<CUVThemeManager>;

private:
	explicit CUVThemeManager(QObject* parent = nullptr);
	~CUVThemeManager() override;
};

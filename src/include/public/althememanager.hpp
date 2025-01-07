#pragma once

#include "alsingleton.hpp"
#include "alwidgettoolsdef.hpp"

#define ALTheme CALThemeManager::instance()
#define ALThemeColor(mode, color) ALTheme->getThemeColor(mode, color)

class QPainter;

class CALThemeManagerPrivate;

class CALWIDGETTOOLS_EXPORT CALThemeManager : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALThemeManager)
	Q_DECLARE_PRIVATE(CALThemeManager)

public:
	static CALThemeManager* instance();

	void setThemeMode(const ALThemeType::ThemeMode& mode);
	[[nodiscard]] ALThemeType::ThemeMode getThemeMode() const;
	void drawEffectShadow(QPainter* painter, const QRect& widgetRect, const int& shadowBorderWidth, const int& borderRadius);
	void setThemeColor(const ALThemeType::ThemeMode& mode, const ALThemeType::ThemeColor& color, const QColor& newColor);
	[[nodiscard]] const QColor& getThemeColor(const ALThemeType::ThemeMode& mode, const ALThemeType::ThemeColor& color) const;

Q_SIGNALS:
	Q_SIGNAL void sigThemeModeChanged(const ALThemeType::ThemeMode& mode);

protected:
	const QScopedPointer<CALThemeManagerPrivate> d_ptr{ nullptr };

	friend class CALSingleton<CALThemeManager>;

private:
	explicit CALThemeManager(QObject* parent = nullptr);
	~CALThemeManager() override;
};

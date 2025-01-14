#pragma once

#include "alsingleton.hpp"
#include "alwidgettoolsdef.hpp"

class QPainter;

#define ALTheme AL::CALThemeManager::instance()
#define ALThemeColor(mode, color) ALTheme->getThemeColor(mode, color)

/**
 * @brief \namespace AL
 */
namespace AL {
class CALThemeManagerPrivate;

class CALWIDGETTOOLS_EXPORT CALThemeManager : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALThemeManager)
	Q_DECLARE_PRIVATE(CALThemeManager)

public:
	static CALThemeManager* instance();

	/**
	 * @brief 设置当前主题模式
	 * @param mode \enum ALThemeType::ThemeMode
	 */
	void setThemeMode(const ALThemeType::ThemeMode& mode);
	[[nodiscard]] ALThemeType::ThemeMode getThemeMode() const;

	/**
	 * @brief 绘制高性能阴影效果
	 * @param painter 绘制使用的 QPainter 对象
	 * @param widgetRect 要绘制阴影的控件矩形
	 * @param shadowBorderWidth 阴影边框宽度
	 * @param borderRadius 阴影圆角半径
	 */
	void drawEffectShadow(QPainter* painter, const QRect& widgetRect, const int& shadowBorderWidth, const qreal& borderRadius);

	/**
	 * @brief 设置指定主题模式和颜色的值
	 * @param mode \enum ALThemeType::ThemeMode
	 * @param color 主题颜色类型 \enum ALThemeType::ThemeColor
	 * @param newColor 新的颜色值
	 */
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
}

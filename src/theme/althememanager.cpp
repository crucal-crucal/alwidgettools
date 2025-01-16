#include "althememanager.hpp"

#include <QPainter>
#include <QPainterPath>

#include "althememanager_p.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALThemeManagerPrivate
 * Internal class for CALThemeManager
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALThemeManagerPrivate::CALThemeManagerPrivate(CALThemeManager* q, QObject* parent) : QObject(parent), q_ptr(q) {
	themeMode = ALThemeType::Light;
}

CALThemeManagerPrivate::~CALThemeManagerPrivate() = default;

void CALThemeManagerPrivate::initColorList() {
	/// ScrollBar
	lightThemeColorList[ALThemeType::ScrollBarHandle] = QColor(0xA0, 0xA0, 0xA0);
	darkThemeColorList[ALThemeType::ScrollBarHandle] = QColor(0x9F, 0x9F, 0x9F);

	/// ToggleSwitch
	lightThemeColorList[ALThemeType::ToggleSwitchNoToggledCenter] = QColor(0x5A, 0x5A, 0x5A);
	darkThemeColorList[ALThemeType::ToggleSwitchNoToggledCenter] = QColor(0xD0, 0xD0, 0xD0);

	/// ThemeColor
	lightThemeColorList[ALThemeType::PrimaryNormal] = QColor(0x00, 0x67, 0xC0);
	darkThemeColorList[ALThemeType::PrimaryNormal] = QColor(0x4C, 0xC2, 0xFF);
	lightThemeColorList[ALThemeType::PrimaryHover] = QColor(0x19, 0x75, 0xC5);
	darkThemeColorList[ALThemeType::PrimaryHover] = QColor(0x47, 0xB1, 0xE8);
	lightThemeColorList[ALThemeType::PrimaryPress] = QColor(0x31, 0x83, 0xCA);
	darkThemeColorList[ALThemeType::PrimaryPress] = QColor(0x42, 0xA1, 0xD2);

	/// 通用颜色
	// base window
	lightThemeColorList[ALThemeType::WindowBase] = QColor(0xF3, 0xF3, 0xF3);
	darkThemeColorList[ALThemeType::WindowBase] = QColor(0x20, 0x20, 0x20);
	lightThemeColorList[ALThemeType::WindowCentralStackBase] = QColor(0xFF, 0xFF, 0xFF, 120);
	darkThemeColorList[ALThemeType::WindowCentralStackBase] = QColor(0x3E, 0x3E, 0x3E, 60);

	// popup window
	lightThemeColorList[ALThemeType::PopupBorder] = QColor(0xD6, 0xD6, 0xD6);
	darkThemeColorList[ALThemeType::PopupBorder] = QColor(0x47, 0x47, 0x47);
	lightThemeColorList[ALThemeType::PopupBorderHover] = QColor(0xCC, 0xCC, 0xCC);
	darkThemeColorList[ALThemeType::PopupBorderHover] = QColor(0x54, 0x54, 0x54);
	lightThemeColorList[ALThemeType::PopupBase] = QColor(0xFA, 0xFA, 0xFA);
	darkThemeColorList[ALThemeType::PopupBase] = QColor(0x2C, 0x2C, 0x2C);
	lightThemeColorList[ALThemeType::PopupHover] = QColor(0xF0, 0xF0, 0xF0);
	darkThemeColorList[ALThemeType::PopupHover] = QColor(0x38, 0x38, 0x38);

	// Dialog window
	lightThemeColorList[ALThemeType::DialogBase] = Qt::white;
	darkThemeColorList[ALThemeType::DialogBase] = QColor(0x1F, 0x1F, 0x1F);
	lightThemeColorList[ALThemeType::DialogLayoutArea] = QColor(0xF3, 0xF3, 0xF3);
	darkThemeColorList[ALThemeType::DialogLayoutArea] = QColor(0x20, 0x20, 0x20);

	// base color
	lightThemeColorList[ALThemeType::BasicText] = Qt::black;
	darkThemeColorList[ALThemeType::BasicText] = Qt::white;
	lightThemeColorList[ALThemeType::BasicTextInvert] = Qt::white;
	darkThemeColorList[ALThemeType::BasicTextInvert] = Qt::black;
	lightThemeColorList[ALThemeType::BasicDetailsText] = QColor(0x87, 0x87, 0x87);
	darkThemeColorList[ALThemeType::BasicDetailsText] = QColor(0xAD, 0xAD, 0xB0);
	lightThemeColorList[ALThemeType::BasicTextNoFocus] = QColor(0x86, 0x86, 0x8A);
	darkThemeColorList[ALThemeType::BasicTextNoFocus] = QColor(0x86, 0x86, 0x8A);
	lightThemeColorList[ALThemeType::BasicTextDisable] = QColor(0xB6, 0xB6, 0xB6);
	darkThemeColorList[ALThemeType::BasicTextDisable] = QColor(0xA7, 0xA7, 0xA7);
	lightThemeColorList[ALThemeType::BasicTextPress] = QColor(0x5A, 0x5A, 0x5D);
	darkThemeColorList[ALThemeType::BasicTextPress] = QColor(0xBB, 0xBB, 0xBF);
	lightThemeColorList[ALThemeType::BasicBorder] = QColor(0xE5, 0xE5, 0xE5);
	darkThemeColorList[ALThemeType::BasicBorder] = QColor(0x4B, 0x4B, 0x4B);
	lightThemeColorList[ALThemeType::BasicBorderDeep] = QColor(0xA8, 0xA8, 0xA8);
	darkThemeColorList[ALThemeType::BasicBorderDeep] = QColor(0x5C, 0x5C, 0x5C);
	lightThemeColorList[ALThemeType::BasicBorderHover] = QColor(0xDA, 0xDA, 0xDA);
	darkThemeColorList[ALThemeType::BasicBorderHover] = QColor(0x57, 0x57, 0x57);
	lightThemeColorList[ALThemeType::BasicBase] = QColor(0xFD, 0xFD, 0xFD);
	darkThemeColorList[ALThemeType::BasicBase] = QColor(0x34, 0x34, 0x34);
	lightThemeColorList[ALThemeType::BasicBaseDeep] = QColor(0xE6, 0xE6, 0xE6);
	darkThemeColorList[ALThemeType::BasicBaseDeep] = QColor(0x61, 0x61, 0x61);
	lightThemeColorList[ALThemeType::BasicDisable] = QColor(0xF5, 0xF5, 0xF5);
	darkThemeColorList[ALThemeType::BasicDisable] = QColor(0x2A, 0x2A, 0x2A);
	lightThemeColorList[ALThemeType::BasicHover] = QColor(0xF3, 0xF3, 0xF3);
	darkThemeColorList[ALThemeType::BasicHover] = QColor(0x40, 0x40, 0x40);
	lightThemeColorList[ALThemeType::BasicPress] = QColor(0xF7, 0xF7, 0xF7);
	darkThemeColorList[ALThemeType::BasicPress] = QColor(0x3A, 0x3A, 0x3A);
	lightThemeColorList[ALThemeType::BasicBaseLine] = QColor(0xD1, 0xD1, 0xD1);
	darkThemeColorList[ALThemeType::BasicBaseLine] = QColor(0x45, 0x45, 0x45);
	lightThemeColorList[ALThemeType::BasicHemline] = QColor(0x86, 0x86, 0x86);
	darkThemeColorList[ALThemeType::BasicHemline] = QColor(0x9A, 0x9A, 0x9A);
	lightThemeColorList[ALThemeType::BasicIndicator] = QColor(0x75, 0x7C, 0x87);
	darkThemeColorList[ALThemeType::BasicIndicator] = QColor(0x75, 0x7C, 0x87);
	lightThemeColorList[ALThemeType::BasicChute] = QColor(0xD6, 0xD6, 0xD6);
	darkThemeColorList[ALThemeType::BasicChute] = QColor(0x63, 0x63, 0x63);

	// base color alpha
	lightThemeColorList[ALThemeType::BasicAlternating] = QColor(0xEF, 0xEF, 0xEF, 160);
	darkThemeColorList[ALThemeType::BasicAlternating] = QColor(0x45, 0x45, 0x45, 125);
	lightThemeColorList[ALThemeType::BasicBaseAlpha] = QColor(0xFF, 0xFF, 0xFF, 160);
	darkThemeColorList[ALThemeType::BasicBaseAlpha] = QColor(0x45, 0x45, 0x45, 95);
	lightThemeColorList[ALThemeType::BasicBaseDeepAlpha] = QColor(0xCC, 0xCC, 0xCC, 160);
	darkThemeColorList[ALThemeType::BasicBaseDeepAlpha] = QColor(0x72, 0x72, 0x72, 95);
	lightThemeColorList[ALThemeType::BasicHoverAlpha] = QColor(0xCC, 0xCC, 0xCC, 60);
	darkThemeColorList[ALThemeType::BasicHoverAlpha] = QColor(0x4B, 0x4B, 0x4B, 75);
	lightThemeColorList[ALThemeType::BasicPressAlpha] = QColor(0xCC, 0xCC, 0xCC, 40);
	darkThemeColorList[ALThemeType::BasicPressAlpha] = QColor(0x4B, 0x4B, 0x4B, 55);
	lightThemeColorList[ALThemeType::BasicSelectedAlpha] = QColor(0xCC, 0xCC, 0xCC, 60);
	darkThemeColorList[ALThemeType::BasicSelectedAlpha] = QColor(0x4B, 0x4B, 0x4B, 75);
	lightThemeColorList[ALThemeType::BasicSelectedHoverAlpha] = QColor(0xCC, 0xCC, 0xCC, 40);
	darkThemeColorList[ALThemeType::BasicSelectedHoverAlpha] = QColor(0x4B, 0x4B, 0x4B, 55);

	// status color
	lightThemeColorList[ALThemeType::StatusDanger] = QColor(0xE8, 0x11, 0x23);
	darkThemeColorList[ALThemeType::StatusDanger] = QColor(0xE8, 0x11, 0x23);
}

/**
 * @brief \class CALThemeManager
 * @return pointer to the instance of CALTheme
 */
CALThemeManager* CALThemeManager::instance() {
	return CALSingleton<CALThemeManager>::instance();
}

void CALThemeManager::setThemeMode(const ALThemeType::ThemeMode& mode) {
	d_func()->themeMode = mode;
	Q_EMIT sigThemeModeChanged(mode);
}

ALThemeType::ThemeMode CALThemeManager::getThemeMode() const {
	return d_func()->themeMode;
}

void CALThemeManager::drawEffectShadow(QPainter* painter, const QRect& widgetRect, const int& shadowBorderWidth, const qreal& borderRadius) {
	Q_D(CALThemeManager);

	painter->save();

	painter->setRenderHint(QPainter::Antialiasing, true);
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	QColor color = d->themeMode == ALThemeType::Light ? QColor(0x70, 0x70, 0x70) : QColor(0x9C, 0x9B, 0x9E);
	for (int i = 0; i < shadowBorderWidth; ++i) {
		path.addRoundedRect(shadowBorderWidth - i, shadowBorderWidth - i, widgetRect.width() - (shadowBorderWidth - i) * 2, widgetRect.height() - (shadowBorderWidth - i) * 2, borderRadius + i, borderRadius + i);
		const int alpha = 1 * (shadowBorderWidth - i + 1);
		color.setAlpha(alpha > 255 ? 255 : alpha);
		painter->setPen(color);
		painter->drawPath(path);
	}

	painter->restore();
}

void CALThemeManager::setThemeColor(const ALThemeType::ThemeMode& mode, const ALThemeType::ThemeColor& color, const QColor& newColor) {
	Q_D(CALThemeManager);

	if (mode == ALThemeType::Light) {
		d->lightThemeColorList[color] = newColor;
	} else {
		d->darkThemeColorList[color] = newColor;
	}
}

const QColor& CALThemeManager::getThemeColor(const ALThemeType::ThemeMode& mode, const ALThemeType::ThemeColor& color) const {
	Q_D(const CALThemeManager);

	if (mode == ALThemeType::Light) {
		return d->lightThemeColorList[color];
	} else {
		return d->darkThemeColorList[color];
	}
}

CALThemeManager::CALThemeManager(QObject* parent): QObject(parent), d_ptr(new CALThemeManagerPrivate(this, this)) {
	d_func()->initColorList();
}

CALThemeManager::~CALThemeManager() = default;
}

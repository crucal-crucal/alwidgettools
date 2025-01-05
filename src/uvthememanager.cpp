#include "uvthememanager.hpp"

#include <QPainter>
#include <QPainterPath>

#include "uvthememanager_p.hpp"

/**
 * @brief \class CUVThemeManagerPrivate
 * Internal class for CUVThemeManager
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CUVThemeManagerPrivate::CUVThemeManagerPrivate(CUVThemeManager* q, QObject* parent) : QObject(parent), q_ptr(q) {
	themeMode = UVThemeType::Light;
}

CUVThemeManagerPrivate::~CUVThemeManagerPrivate() = default;

void CUVThemeManagerPrivate::initColorList() {
	/// ScrollBar
	lightThemeColorList[UVThemeType::ScrollBarHandle] = QColor(0xA0, 0xA0, 0xA0);
	darkThemeColorList[UVThemeType::ScrollBarHandle] = QColor(0x9F, 0x9F, 0x9F);

	/// ToggleSwitch
	lightThemeColorList[UVThemeType::ToggleSwitchNoToggledCenter] = QColor(0x5A, 0x5A, 0x5A);
	darkThemeColorList[UVThemeType::ToggleSwitchNoToggledCenter] = QColor(0xD0, 0xD0, 0xD0);

	/// ThemeColor
	lightThemeColorList[UVThemeType::PrimaryNormal] = QColor(0x00, 0x67, 0xC0);
	darkThemeColorList[UVThemeType::PrimaryNormal] = QColor(0x4C, 0xC2, 0xFF);
	lightThemeColorList[UVThemeType::PrimaryHover] = QColor(0x19, 0x75, 0xC5);
	darkThemeColorList[UVThemeType::PrimaryHover] = QColor(0x47, 0xB1, 0xE8);
	lightThemeColorList[UVThemeType::PrimaryPress] = QColor(0x31, 0x83, 0xCA);
	darkThemeColorList[UVThemeType::PrimaryPress] = QColor(0x42, 0xA1, 0xD2);

	/// 通用颜色
	// base window
	lightThemeColorList[UVThemeType::WindowBase] = QColor(0xF3, 0xF3, 0xF3);
	darkThemeColorList[UVThemeType::WindowBase] = QColor(0x20, 0x20, 0x20);
	lightThemeColorList[UVThemeType::WindowCentralStackBase] = QColor(0xFF, 0xFF, 0xFF, 120);
	darkThemeColorList[UVThemeType::WindowCentralStackBase] = QColor(0x3E, 0x3E, 0x3E, 60);

	// popup window
	lightThemeColorList[UVThemeType::PopupBorder] = QColor(0xD6, 0xD6, 0xD6);
	darkThemeColorList[UVThemeType::PopupBorder] = QColor(0x47, 0x47, 0x47);
	lightThemeColorList[UVThemeType::PopupBorderHover] = QColor(0xCC, 0xCC, 0xCC);
	darkThemeColorList[UVThemeType::PopupBorderHover] = QColor(0x54, 0x54, 0x54);
	lightThemeColorList[UVThemeType::PopupBase] = QColor(0xFA, 0xFA, 0xFA);
	darkThemeColorList[UVThemeType::PopupBase] = QColor(0x2C, 0x2C, 0x2C);
	lightThemeColorList[UVThemeType::PopupHover] = QColor(0xF0, 0xF0, 0xF0);
	darkThemeColorList[UVThemeType::PopupHover] = QColor(0x38, 0x38, 0x38);

	// Dialog window
	lightThemeColorList[UVThemeType::DialogBase] = Qt::white;
	darkThemeColorList[UVThemeType::DialogBase] = QColor(0x1F, 0x1F, 0x1F);
	lightThemeColorList[UVThemeType::DialogLayoutArea] = QColor(0xF3, 0xF3, 0xF3);
	darkThemeColorList[UVThemeType::DialogLayoutArea] = QColor(0x20, 0x20, 0x20);

	// base color
	lightThemeColorList[UVThemeType::BasicText] = Qt::black;
	darkThemeColorList[UVThemeType::BasicText] = Qt::white;
	lightThemeColorList[UVThemeType::BasicTextInvert] = Qt::white;
	darkThemeColorList[UVThemeType::BasicTextInvert] = Qt::black;
	lightThemeColorList[UVThemeType::BasicDetailsText] = QColor(0x87, 0x87, 0x87);
	darkThemeColorList[UVThemeType::BasicDetailsText] = QColor(0xAD, 0xAD, 0xB0);
	lightThemeColorList[UVThemeType::BasicTextNoFocus] = QColor(0x86, 0x86, 0x8A);
	darkThemeColorList[UVThemeType::BasicTextNoFocus] = QColor(0x86, 0x86, 0x8A);
	lightThemeColorList[UVThemeType::BasicTextDisable] = QColor(0xB6, 0xB6, 0xB6);
	darkThemeColorList[UVThemeType::BasicTextDisable] = QColor(0xA7, 0xA7, 0xA7);
	lightThemeColorList[UVThemeType::BasicTextPress] = QColor(0x5A, 0x5A, 0x5D);
	darkThemeColorList[UVThemeType::BasicTextPress] = QColor(0xBB, 0xBB, 0xBF);
	lightThemeColorList[UVThemeType::BasicBorder] = QColor(0xE5, 0xE5, 0xE5);
	darkThemeColorList[UVThemeType::BasicBorder] = QColor(0x4B, 0x4B, 0x4B);
	lightThemeColorList[UVThemeType::BasicBorderDeep] = QColor(0xA8, 0xA8, 0xA8);
	darkThemeColorList[UVThemeType::BasicBorderDeep] = QColor(0x5C, 0x5C, 0x5C);
	lightThemeColorList[UVThemeType::BasicBorderHover] = QColor(0xDA, 0xDA, 0xDA);
	darkThemeColorList[UVThemeType::BasicBorderHover] = QColor(0x57, 0x57, 0x57);
	lightThemeColorList[UVThemeType::BasicBase] = QColor(0xFD, 0xFD, 0xFD);
	darkThemeColorList[UVThemeType::BasicBase] = QColor(0x34, 0x34, 0x34);
	lightThemeColorList[UVThemeType::BasicBaseDeep] = QColor(0xE6, 0xE6, 0xE6);
	darkThemeColorList[UVThemeType::BasicBaseDeep] = QColor(0x61, 0x61, 0x61);
	lightThemeColorList[UVThemeType::BasicDisable] = QColor(0xF5, 0xF5, 0xF5);
	darkThemeColorList[UVThemeType::BasicDisable] = QColor(0x2A, 0x2A, 0x2A);
	lightThemeColorList[UVThemeType::BasicHover] = QColor(0xF3, 0xF3, 0xF3);
	darkThemeColorList[UVThemeType::BasicHover] = QColor(0x40, 0x40, 0x40);
	lightThemeColorList[UVThemeType::BasicPress] = QColor(0xF7, 0xF7, 0xF7);
	darkThemeColorList[UVThemeType::BasicPress] = QColor(0x3A, 0x3A, 0x3A);
	lightThemeColorList[UVThemeType::BasicBaseLine] = QColor(0xD1, 0xD1, 0xD1);
	darkThemeColorList[UVThemeType::BasicBaseLine] = QColor(0x45, 0x45, 0x45);
	lightThemeColorList[UVThemeType::BasicHemline] = QColor(0x86, 0x86, 0x86);
	darkThemeColorList[UVThemeType::BasicHemline] = QColor(0x9A, 0x9A, 0x9A);
	lightThemeColorList[UVThemeType::BasicIndicator] = QColor(0x75, 0x7C, 0x87);
	darkThemeColorList[UVThemeType::BasicIndicator] = QColor(0x75, 0x7C, 0x87);
	lightThemeColorList[UVThemeType::BasicChute] = QColor(0xD6, 0xD6, 0xD6);
	darkThemeColorList[UVThemeType::BasicChute] = QColor(0x63, 0x63, 0x63);

	// base color alpha
	lightThemeColorList[UVThemeType::BasicAlternating] = QColor(0xEF, 0xEF, 0xEF, 160);
	darkThemeColorList[UVThemeType::BasicAlternating] = QColor(0x45, 0x45, 0x45, 125);
	lightThemeColorList[UVThemeType::BasicBaseAlpha] = QColor(0xFF, 0xFF, 0xFF, 160);
	darkThemeColorList[UVThemeType::BasicBaseAlpha] = QColor(0x45, 0x45, 0x45, 95);
	lightThemeColorList[UVThemeType::BasicBaseDeepAlpha] = QColor(0xCC, 0xCC, 0xCC, 160);
	darkThemeColorList[UVThemeType::BasicBaseDeepAlpha] = QColor(0x72, 0x72, 0x72, 95);
	lightThemeColorList[UVThemeType::BasicHoverAlpha] = QColor(0xCC, 0xCC, 0xCC, 60);
	darkThemeColorList[UVThemeType::BasicHoverAlpha] = QColor(0x4B, 0x4B, 0x4B, 75);
	lightThemeColorList[UVThemeType::BasicPressAlpha] = QColor(0xCC, 0xCC, 0xCC, 40);
	darkThemeColorList[UVThemeType::BasicPressAlpha] = QColor(0x4B, 0x4B, 0x4B, 55);
	lightThemeColorList[UVThemeType::BasicSelectedAlpha] = QColor(0xCC, 0xCC, 0xCC, 60);
	darkThemeColorList[UVThemeType::BasicSelectedAlpha] = QColor(0x4B, 0x4B, 0x4B, 75);
	lightThemeColorList[UVThemeType::BasicSelectedHoverAlpha] = QColor(0xCC, 0xCC, 0xCC, 40);
	darkThemeColorList[UVThemeType::BasicSelectedHoverAlpha] = QColor(0x4B, 0x4B, 0x4B, 55);

	// status color
	lightThemeColorList[UVThemeType::StatusDanger] = QColor(0xE8, 0x11, 0x23);
	darkThemeColorList[UVThemeType::StatusDanger] = QColor(0xE8, 0x11, 0x23);
}

/**
 * @brief \class CUVThemeManager
 * @return pointer to the instance of CUVTheme
 */
CUVThemeManager* CUVThemeManager::instance() {
	return CUVSingleton<CUVThemeManager>::instance();
}

void CUVThemeManager::setThemeMode(const UVThemeType::ThemeMode& mode) {
	d_func()->themeMode = mode;
	Q_EMIT sigThemeModeChanged(mode);
}

UVThemeType::ThemeMode CUVThemeManager::getThemeMode() const {
	return d_func()->themeMode;
}

void CUVThemeManager::drawEffectShadow(QPainter* painter, const QRect& widgetRect, const int& shadowBorderWidth, const int& borderRadius) {
	Q_D(CUVThemeManager);

	painter->save();

	painter->setRenderHint(QPainter::Antialiasing, true);
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	QColor color = d->themeMode == UVThemeType::Light ? QColor(0x70, 0x70, 0x70) : QColor(0x9C, 0x9B, 0x9E);
	for (int i = 0; i < shadowBorderWidth; i++) {
		path.addRoundedRect(shadowBorderWidth - i, shadowBorderWidth - i, widgetRect.width() - (shadowBorderWidth - i) * 2, widgetRect.height() - (shadowBorderWidth - i) * 2, borderRadius + i, borderRadius + i);
		const int alpha = 1 * (shadowBorderWidth - i + 1);
		color.setAlpha(alpha > 255 ? 255 : alpha);
		painter->setPen(color);
		painter->drawPath(path);
	}

	painter->restore();
}

void CUVThemeManager::setThemeColor(const UVThemeType::ThemeMode& mode, const UVThemeType::ThemeColor& color, const QColor& newColor) {
	Q_D(CUVThemeManager);

	if (mode == UVThemeType::Light) {
		d->lightThemeColorList[color] = newColor;
	} else {
		d->darkThemeColorList[color] = newColor;
	}
}

const QColor& CUVThemeManager::getThemeColor(const UVThemeType::ThemeMode& mode, const UVThemeType::ThemeColor& color) const {
	Q_D(const CUVThemeManager);

	if (mode == UVThemeType::Light) {
		return d->lightThemeColorList[color];
	} else {
		return d->darkThemeColorList[color];
	}
}

CUVThemeManager::CUVThemeManager(QObject* parent): QObject(parent), d_ptr(new CUVThemeManagerPrivate(this, this)) {
	d_func()->initColorList();
}

CUVThemeManager::~CUVThemeManager() = default;

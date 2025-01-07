#include "uvscrollbarstyle.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStyleOptionSlider>
#include <QtMath>

#include "uvscrollbar.hpp"
#include "althememanager.hpp"

/**
 * @brief \class CUVScrollBarStyle
 * @param style pointer to the parent style
 */
CUVScrollBarStyle::CUVScrollBarStyle(QStyle* style): QProxyStyle(style) {
	m_isExpand = false;
	m_opacity = 0;
	m_sliderExtent = 2.4;
	m_themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CUVScrollBarStyle::~CUVScrollBarStyle() = default;

void CUVScrollBarStyle::drawComplexControl(const ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const {
	switch (control) {
		case QStyle::CC_ScrollBar: {
			if (const auto sopt = qstyleoption_cast<const QStyleOptionSlider*>(option)) {
				painter->save();
				painter->setRenderHint(QPainter::Antialiasing);
				painter->setPen(Qt::NoPen);
				const QRect scrollBarRect = sopt->rect;
				if (m_isExpand) {
					// 背景绘制
					painter->setOpacity(m_opacity);
					painter->setPen(Qt::NoPen);
					painter->setBrush(UVThemeColor(m_themeMode, ALThemeType::BasicBase));
					painter->drawRoundedRect(scrollBarRect, 6, 6);
					// 指示器绘制
					constexpr int sideLength = 8;
					painter->setBrush(UVThemeColor(m_themeMode, ALThemeType::ScrollBarHandle));
					if (sopt->orientation == Qt::Horizontal) {
						const QRect leftIndicatorRect = subControlRect(control, sopt, QStyle::SC_ScrollBarSubLine, widget);
						const QRect rightIndicatorRect = subControlRect(control, sopt, QStyle::SC_ScrollBarAddLine, widget);
						// 左三角
						const qreal centerLeftX = leftIndicatorRect.x() + leftIndicatorRect.width() / 2.0;
						const qreal centerRightX = rightIndicatorRect.x() + rightIndicatorRect.width() / 2.0;
						const qreal centerY = leftIndicatorRect.height() / 2.0;
						QPainterPath leftPath;
						leftPath.moveTo(centerLeftX - qCos(30 * M_PI / 180.0) * sideLength / 2, centerY);
						leftPath.lineTo(centerLeftX + qCos(30 * M_PI / 180.0) * sideLength / 2, centerY + sideLength / 2.0);
						leftPath.lineTo(centerLeftX + qCos(30 * M_PI / 180.0) * sideLength / 2, centerY - sideLength / 2.0);
						leftPath.closeSubpath();
						painter->drawPath(leftPath);

						// 右三角
						QPainterPath rightPath;
						rightPath.moveTo(centerRightX + qCos(30 * M_PI / 180.0) * sideLength / 2, centerY);
						rightPath.lineTo(centerRightX - qCos(30 * M_PI / 180.0) * sideLength / 2, centerY + sideLength / 2.0);
						rightPath.lineTo(centerRightX - qCos(30 * M_PI / 180.0) * sideLength / 2, centerY - sideLength / 2.0);
						rightPath.closeSubpath();
						painter->drawPath(rightPath);
					} else {
						const QRect upIndicatorRect = subControlRect(control, sopt, QStyle::SC_ScrollBarSubLine, widget);
						const QRect downIndicatorRect = subControlRect(control, sopt, QStyle::SC_ScrollBarAddLine, widget);
						const qreal centerToTop = (sideLength / 2.0) / qCos(30 * M_PI / 180.0);
						const qreal centerToBottom = (sideLength / 2.0) * qTan(30 * M_PI / 180.0);
						// 上三角
						const qreal centerX = upIndicatorRect.width() / 2.0;
						const qreal centerUpY = upIndicatorRect.center().y() + 2.0;
						const qreal centerDownY = downIndicatorRect.center().y() + 2.0;
						QPainterPath upPath;
						upPath.moveTo(centerX, centerUpY - centerToTop);
						upPath.lineTo(centerX + sideLength / 2.0, centerUpY + centerToBottom);
						upPath.lineTo(centerX - sideLength / 2.0, centerUpY + centerToBottom);
						upPath.closeSubpath();
						painter->drawPath(upPath);

						// 下三角
						QPainterPath downPath;
						downPath.moveTo(centerX, centerDownY + centerToBottom);
						downPath.lineTo(centerX + sideLength / 2.0, centerDownY - centerToTop);
						downPath.lineTo(centerX - sideLength / 2.0, centerDownY - centerToTop);
						downPath.closeSubpath();
						painter->drawPath(downPath);
					}
				}
				painter->setOpacity(1);
				// 滑块绘制
				QRectF sliderRect = subControlRect(control, option, SC_ScrollBarSlider, widget);
				painter->setBrush(UVThemeColor(m_themeMode, ALThemeType::ScrollBarHandle));
				if (sopt->orientation == Qt::Horizontal) {
					sliderRect.setRect(sliderRect.x(), sliderRect.bottom() - m_sliderMargin - m_sliderExtent, sliderRect.width(), m_sliderExtent);
				} else {
					sliderRect.setRect(sliderRect.right() - m_sliderMargin - m_sliderExtent, sliderRect.y(), m_sliderExtent, sliderRect.height());
				}
				painter->drawRoundedRect(sliderRect, m_sliderExtent / 2.0, m_sliderExtent / 2.0);
				painter->restore();
			}
			return;
		}
		default: {
			break;
		}
	}

	QProxyStyle::drawComplexControl(control, option, painter, widget);
}

int CUVScrollBarStyle::pixelMetric(const PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
	switch (metric) {
		case QStyle::PM_ScrollBarExtent: {
			return m_scrollBarExtent;
		}
		default: {
			break;
		}
	}

	return QProxyStyle::pixelMetric(metric, option, widget);
}

int CUVScrollBarStyle::styleHint(const StyleHint stylehint, const QStyleOption* opt, const QWidget* widget, QStyleHintReturn* returnData) const {
	if (stylehint == QStyle::SH_ScrollBar_LeftClickAbsolutePosition) {
		return true;
	}
	return QProxyStyle::styleHint(stylehint, opt, widget, returnData);
}

void CUVScrollBarStyle::startExpandAnimation(const bool isExpand) {
	if (isExpand) {
		m_isExpand = true;
		const auto opacityAnimation = new QPropertyAnimation(this, "opacity");
		connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=]() { m_scrollBar->update(); });
		opacityAnimation->setDuration(250);
		opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
		opacityAnimation->setStartValue(m_opacity);
		opacityAnimation->setEndValue(1);
		opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

		const auto extentAnimation = new QPropertyAnimation(this, "sliderExtent");
		extentAnimation->setDuration(250);
		extentAnimation->setEasingCurve(QEasingCurve::InOutSine);
		extentAnimation->setStartValue(m_sliderExtent);
		extentAnimation->setEndValue(m_scrollBarExtent - 2 * m_sliderMargin);
		extentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	} else {
		const auto opacityAnimation = new QPropertyAnimation(this, "opacity");
		connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() { m_isExpand = false; });
		connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=]() { m_scrollBar->update(); });
		opacityAnimation->setDuration(250);
		opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
		opacityAnimation->setStartValue(m_opacity);
		opacityAnimation->setEndValue(0);
		opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

		const auto extentAnimation = new QPropertyAnimation(this, "sliderExtent");
		extentAnimation->setDuration(250);
		extentAnimation->setEasingCurve(QEasingCurve::InOutSine);
		extentAnimation->setStartValue(m_sliderExtent);
		extentAnimation->setEndValue(2.4);
		extentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}
}

void CUVScrollBarStyle::setOpacity(const qreal opacity) {
	m_opacity = opacity;
}

qreal CUVScrollBarStyle::getOpacity() const {
	return m_opacity;
}

void CUVScrollBarStyle::setSliderExtent(const qreal sliderExtent) {
	m_sliderExtent = sliderExtent;
}

qreal CUVScrollBarStyle::getSliderExtent() const {
	return m_sliderExtent;
}

void CUVScrollBarStyle::setIsExpand(const bool isExpand) {
	m_isExpand = isExpand;
}

bool CUVScrollBarStyle::getIsExpand() const {
	return m_isExpand;
}

void CUVScrollBarStyle::setScrollBar(CUVScrollBar* scrollBar) {
	m_scrollBar = scrollBar;
}

CUVScrollBar* CUVScrollBarStyle::getScrollBar() const {
	return m_scrollBar;
}

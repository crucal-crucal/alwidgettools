#pragma once

#include <QProxyStyle>

#include "alwidgettoolsdef.hpp"

class CUVScrollBar;

class CUVScrollBarStyle final : public QProxyStyle {
	Q_OBJECT
	Q_PROPERTY(qreal opacity MEMBER m_opacity NOTIFY sigOpacityChanged)
	Q_PROPERTY(qreal sliderExtent MEMBER m_sliderExtent NOTIFY sigSliderExtentChanged)

public:
	explicit CUVScrollBarStyle(QStyle* style = nullptr);
	~CUVScrollBarStyle() override;

	void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;
	int styleHint(StyleHint stylehint, const QStyleOption* opt, const QWidget* widget, QStyleHintReturn* returnData) const override;
	void startExpandAnimation(bool isExpand);

	void setOpacity(qreal opacity);
	[[nodiscard]] qreal getOpacity() const;

	void setSliderExtent(qreal sliderExtent);
	[[nodiscard]] qreal getSliderExtent() const;

	void setIsExpand(bool isExpand);
	[[nodiscard]] bool getIsExpand() const;

	void setScrollBar(CUVScrollBar* scrollBar);
	[[nodiscard]] CUVScrollBar* getScrollBar() const;

signals:
	void sigOpacityChanged();
	void sigSliderExtentChanged();

private:
	ALThemeType::ThemeMode m_themeMode{};
	bool m_isExpand{};
	qreal m_opacity{};
	qreal m_sliderExtent{};
	CUVScrollBar* m_scrollBar{ nullptr };
	qreal m_sliderMargin{ 2.5 };
	int m_scrollBarExtent{ 10 };
};

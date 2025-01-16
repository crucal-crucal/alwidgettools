#pragma once

#include <QPushButton>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALAcrylicUrlCardPrivate;

class CALWIDGETTOOLS_EXPORT CALAcrylicUrlCard : public QPushButton {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALAcrylicUrlCard)
	Q_PROPERTY(int brushAlpha READ getBrushAlpha WRITE setBrushAlpha NOTIFY sigBrushAlphaChanged)
	Q_PROPERTY(int titlePixelSize READ getTitlePixelSize WRITE setTitlePixelSize NOTIFY sigTitlePixelSizeChanged)
	Q_PROPERTY(int subTitlePixelSize READ getSubTitlePixelSize WRITE setSubTitlePixelSize NOTIFY sigSubTitlePixelSizeChanged)
	Q_PROPERTY(int titleSpacing READ getTitleSpacing WRITE setTitleSpacing NOTIFY sigTitleSpacingChanged)
	Q_PROPERTY(int subTitleSpacing READ getSubTitleSpacing WRITE setSubTitleSpacing NOTIFY sigSubTitleSpacingChanged)
	Q_PROPERTY(qreal borderradius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(qreal mainOpacity READ getMainOpacity WRITE setMainOpacity NOTIFY sigMainOpacityChanged)
	Q_PROPERTY(qreal noiseOpacity READ getNoiseOpacity WRITE setNoiseOpacity NOTIFY sigNoiseOpacityChanged)
	Q_PROPERTY(qreal borderWidth READ getBorderWidth WRITE setBorderWidth NOTIFY sigBorderWidthChanged)
	Q_PROPERTY(qreal cardPixmapBorderRadius READ getCardPixmapBorderRadius WRITE setCardPixmapBorderRadius NOTIFY sigCardPixmapBorderRadiusChanged)
	Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY sigTitleChanged)
	Q_PROPERTY(QString subTitle READ getSubTitle WRITE setSubTitle NOTIFY sigSubTitleChanged)
	Q_PROPERTY(QString url READ getUrl WRITE setUrl NOTIFY sigUrlChanged)
	Q_PROPERTY(QPixmap cardPixmap READ getCardPixmap WRITE setCardPixmap NOTIFY sigCardPixmapChanged)
	Q_PROPERTY(QSize cardPixmapSize READ getCardPixmapSize WRITE setCardPixmapSize NOTIFY sigCardPixmapSizeChanged)

public:
	explicit CALAcrylicUrlCard(QWidget* parent = nullptr);
	~CALAcrylicUrlCard() override;

	void setBrushAlpha(int alpha);
	[[nodiscard]] int getBrushAlpha() const;

	void setTitlePixelSize(int size);
	[[nodiscard]] int getTitlePixelSize() const;

	void setSubTitlePixelSize(int size);
	[[nodiscard]] int getSubTitlePixelSize() const;

	void setTitleSpacing(int spacing);
	[[nodiscard]] int getTitleSpacing() const;

	void setSubTitleSpacing(int spacing);
	[[nodiscard]] int getSubTitleSpacing() const;

	void setBorderRadius(qreal radius);
	[[nodiscard]] qreal getBorderRadius() const;

	void setMainOpacity(qreal opacity);
	[[nodiscard]] qreal getMainOpacity() const;

	void setNoiseOpacity(qreal opacity);
	[[nodiscard]] qreal getNoiseOpacity() const;

	void setBorderWidth(qreal width);
	[[nodiscard]] qreal getBorderWidth() const;

	void setCardPixmapBorderRadius(qreal radius);
	[[nodiscard]] qreal getCardPixmapBorderRadius() const;

	void setTitle(const QString& title);
	[[nodiscard]] QString getTitle() const;

	void setSubTitle(const QString& subTitle);
	[[nodiscard]] QString getSubTitle() const;

	void setUrl(const QString& url);
	[[nodiscard]] QString getUrl() const;

	void setCardPixmap(const QPixmap& pixmap);
	[[nodiscard]] QPixmap getCardPixmap() const;

	void setCardPixmapSize(const QSize& size);
	void setCardPixmapSize(int width, int height);
	[[nodiscard]] QSize getCardPixmapSize() const;

Q_SIGNALS:
	Q_SIGNAL void sigBorderRadiusChanged();
	Q_SIGNAL void sigBrushAlphaChanged();
	Q_SIGNAL void sigTitlePixelSizeChanged();
	Q_SIGNAL void sigSubTitlePixelSizeChanged();
	Q_SIGNAL void sigTitleSpacingChanged();
	Q_SIGNAL void sigSubTitleSpacingChanged();
	Q_SIGNAL void sigCardPixmapBorderRadiusChanged();
	Q_SIGNAL void sigMainOpacityChanged();
	Q_SIGNAL void sigNoiseOpacityChanged();
	Q_SIGNAL void sigBorderWidthChanged();
	Q_SIGNAL void sigTitleChanged();
	Q_SIGNAL void sigSubTitleChanged();
	Q_SIGNAL void sigUrlChanged();
	Q_SIGNAL void sigCardPixmapChanged();
	Q_SIGNAL void sigCardPixmapSizeChanged();
	Q_SIGNAL void sigOpenError(const QString& errMsg);

protected:
	const QScopedPointer<CALAcrylicUrlCardPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
}

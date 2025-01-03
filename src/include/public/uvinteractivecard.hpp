#pragma once

#include <QPushButton>

#include "uvpushbutton.hpp"

class CUVInteractiveCardPrivate;

class CUVWIDGETTOOLS_EXPORT CUVInteractiveCard : public QPushButton {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVInteractiveCard)
	Q_PROPERTY(UVCardPixType::PixMode cardPixMode READ getCardPixMode WRITE setCardPixMode NOTIFY sigCardPixModeChanged)
	Q_PROPERTY(qreal borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(qreal cardPixmapBorderRadius READ getCardPixmapBorderRadius WRITE setCardPixmapBorderRadius NOTIFY sigCardPixmapBorderRadiusChanged)
	Q_PROPERTY(int titlePixelSize READ getTitlePixelSize WRITE setTitlePixelSize NOTIFY sigTitlePixelSizeChanged)
	Q_PROPERTY(int subTitlePixelSize READ getSubTitlePixelSize WRITE setSubTitlePixelSize NOTIFY sigSubTitlePixelSizeChanged)
	Q_PROPERTY(int titleSpacing READ getTitleSpacing WRITE setTitleSpacing NOTIFY sigTitleSpacingChanged)
	Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY sigTitleChanged)
	Q_PROPERTY(QString subTitle READ getSubTitle WRITE setSubTitle NOTIFY sigSubTitleChanged)
	Q_PROPERTY(QPixmap cardPixmap READ getCardPixmap WRITE setCardPixmap NOTIFY sigCardPixmapChanged)
	Q_PROPERTY(QSize cardPixmapSize READ getCardPixmapSize WRITE setCardPixmapSize NOTIFY sigCardPixmapSizeChanged)

public:
	explicit CUVInteractiveCard(QWidget* parent = nullptr);
	~CUVInteractiveCard() override;

	void setCardPixmapSize(int width, int height);

	void setCardPixMode(const UVCardPixType::PixMode& mode);
	[[nodiscard]] UVCardPixType::PixMode getCardPixMode() const;

	void setBorderRadius(qreal radius);
	[[nodiscard]] qreal getBorderRadius() const;

	void setCardPixmapBorderRadius(qreal radius);
	[[nodiscard]] qreal getCardPixmapBorderRadius() const;

	void setTitlePixelSize(int pixelSize);
	[[nodiscard]] int getTitlePixelSize() const;

	void setSubTitlePixelSize(int pixelSize);
	[[nodiscard]] int getSubTitlePixelSize() const;

	void setTitleSpacing(int spacing);
	[[nodiscard]] int getTitleSpacing() const;

	void setTitle(const QString& title);
	[[nodiscard]] QString getTitle() const;

	void setSubTitle(const QString& subTitle);
	[[nodiscard]] QString getSubTitle() const;

	void setCardPixmap(const QPixmap& pixmap);
	[[nodiscard]] QPixmap getCardPixmap() const;

	void setCardPixmapSize(const QSize& size);
	[[nodiscard]] QSize getCardPixmapSize() const;

Q_SIGNALS:
	Q_SIGNAL void sigCardPixModeChanged();
	Q_SIGNAL void sigBorderRadiusChanged();
	Q_SIGNAL void sigTitlePixelSizeChanged();
	Q_SIGNAL void sigSubTitlePixelSizeChanged();
	Q_SIGNAL void sigTitleSpacingChanged();
	Q_SIGNAL void sigCardPixmapBorderRadiusChanged();
	Q_SIGNAL void sigTitleChanged();
	Q_SIGNAL void sigSubTitleChanged();
	Q_SIGNAL void sigCardPixmapChanged();
	Q_SIGNAL void sigCardPixmapSizeChanged();

protected:
	const QScopedPointer<CUVInteractiveCardPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};

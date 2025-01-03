#pragma once

#include <QPushButton>

#include "uvwidgettoolsdef.hpp"

using namespace UVIcon;

class CUVAwesomeButtonPrivate;

class CUVWIDGETTOOLS_EXPORT CUVAwesomeButton : public QPushButton {
	Q_OBJECT
	Q_DISABLE_COPY(CUVAwesomeButton)
	Q_DECLARE_PRIVATE(CUVAwesomeButton)

	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(QColor lightHoverColor READ getLightHoverColor WRITE setLightHoverColor NOTIFY sigLightHoverColorChanged)
	Q_PROPERTY(QColor darkHoverColor READ getDarkHoverColor WRITE setDarkHoverColor NOTIFY sigDarkHoverColorChanged)
	Q_PROPERTY(QColor lightIconColor READ getLightIconColor WRITE setLightIconColor NOTIFY sigLightIconColorChanged)
	Q_PROPERTY(QColor darkIconColor READ getDarkIconColor WRITE setDarkIconColor NOTIFY sigDarkIconColorChanged)
	Q_PROPERTY(QColor ligntSelectedColor READ getLigntSelectedColor WRITE setLigntSelectedColor NOTIFY sigLigntSelectedColorChanged)
	Q_PROPERTY(QColor darkSelectedColor READ getDarkSelectedColor WRITE setDarkSelectedColor NOTIFY sigDarkSelectedColorChanged)
	Q_PROPERTY(bool isSelected READ getIsSelected WRITE setIsSelected NOTIFY sigIsSelectedChanged)

public:
	explicit CUVAwesomeButton(const CUVAweSomeIcon& iconType, QWidget* parent = nullptr);
	explicit CUVAwesomeButton(const CUVAweSomeIcon& iconType, int pixelSize, QWidget* parent = nullptr);
	explicit CUVAwesomeButton(const CUVAweSomeIcon& iconType, int pixelSize, int fixedWidth, int fixedHeight, QWidget* parent = nullptr);
	~CUVAwesomeButton() override;

	void setAwesomeIcon(const CUVAweSomeIcon& iconType);
	[[nodiscard]] CUVAweSomeIcon getAwesomeIcon() const;

	void setBorderRadius(int radius);
	[[nodiscard]] int getBorderRadius() const;

	void setLightHoverColor(const QColor& color);
	[[nodiscard]] QColor getLightHoverColor() const;

	void setDarkHoverColor(const QColor& color);
	[[nodiscard]] QColor getDarkHoverColor() const;

	void setLightIconColor(const QColor& color);
	[[nodiscard]] QColor getLightIconColor() const;

	void setDarkIconColor(const QColor& color);
	[[nodiscard]] QColor getDarkIconColor() const;

	void setLigntSelectedColor(const QColor& color);
	[[nodiscard]] QColor getLigntSelectedColor() const;

	void setDarkSelectedColor(const QColor& color);
	[[nodiscard]] QColor getDarkSelectedColor() const;

	void setIsSelected(bool isSelected);
	[[nodiscard]] bool getIsSelected() const;

	void setToolTip(const QString& tooltip);

signals:
	void sigBorderRadiusChanged(int radius);
	void sigLightHoverColorChanged(const QColor& color);
	void sigDarkHoverColorChanged(const QColor& color);
	void sigLightIconColorChanged(const QColor& color);
	void sigDarkIconColorChanged(const QColor& color);
	void sigLigntSelectedColorChanged(const QColor& color);
	void sigDarkSelectedColorChanged(const QColor& color);
	void sigIsSelectedChanged(bool isSelected);

protected:
	const QScopedPointer<CUVAwesomeButtonPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};

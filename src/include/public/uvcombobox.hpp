#pragma once

#include <QComboBox>

#include "uvwidgettoolsdef.hpp"

class CUVComboBoxPrivate;

class CUVWIDGETTOOLS_EXPORT CUVComboBox : public QComboBox {
	Q_OBJECT
	Q_DISABLE_COPY(CUVComboBox)
	Q_DECLARE_PRIVATE(CUVComboBox)

	Q_PROPERTY(int BorderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)

public:
	explicit CUVComboBox(QWidget* parent = nullptr);
	~CUVComboBox() override;

	void setBorderRadius(int borderRadius);
	[[nodiscard]] int getBorderRadius() const;

	void setNomalColor(const QColor& color);
	[[nodiscard]] QColor getNomalColor() const;

	void setMouseHoverColor(const QColor& color);
	[[nodiscard]] QColor getMouseHoverColor() const;

	void setMouseSelectedColor(const QColor& color);
	[[nodiscard]] QColor getMouseSelectedColor() const;

	void setBorderColor(const QColor& color);
	[[nodiscard]] QColor getBorderColor() const;

signals:
	void sigBorderRadiusChanged();

protected:
	const QScopedPointer<CUVComboBoxPrivate> d_ptr{ nullptr };

	void showPopup() override;
	void hidePopup() override;
};

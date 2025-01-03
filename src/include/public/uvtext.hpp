#pragma once

#include <QLabel>

#include "uvwidgettoolsdef.hpp"

class CUVTextPrivate;

class CUVWIDGETTOOLS_EXPORT CUVText : public QLabel {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVText)

public:
	explicit CUVText(QWidget* parent = nullptr);
	explicit CUVText(const QString& text, QWidget* parent = nullptr);
	explicit CUVText(const QString& text, const int& pixelSize, QWidget* parent = nullptr);
	~CUVText() override;

	void setIsWrapAnyWhere(const bool& isWrapAnyWhere);
	[[nodiscard]] bool getIsWrapAnyWhere() const;

	void setTextPixelSize(const int& pixelSize);
	[[nodiscard]] int getTextPixelSize() const;

	void setTextPointSize(const int& pointSize);
	[[nodiscard]] int getTextPointSize() const;

	void setTextStyle(const UVTextType::TextStyle& textStyle);
	[[nodiscard]] UVTextType::TextStyle getTextStyle() const;

protected:
	const QScopedPointer<CUVTextPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};

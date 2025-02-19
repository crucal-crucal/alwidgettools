#pragma once

#include <QLabel>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALTextPrivate;

class CALWIDGETTOOLS_EXPORT CALText : public QLabel {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALText)
	Q_PROPERTY(ALIcon::AweSomeIcon aweSomeIcon READ getAweSomeIcon WRITE setAweSomeIcon NOTIFY sigAweSomeIconChanged)
	Q_PROPERTY(ALIcon::FluentIcon fluentIcon READ getFluentIcon WRITE setFluentIcon NOTIFY sigFluentIconChanged)

public:
	explicit CALText(QWidget* parent = nullptr);
	explicit CALText(const QString& text, QWidget* parent = nullptr);
	explicit CALText(const QString& text, const int& pixelSize, QWidget* parent = nullptr);
	~CALText() override;

	void setIsWrapAnyWhere(const bool& isWrapAnyWhere);
	[[nodiscard]] bool getIsWrapAnyWhere() const;

	void setTextPixelSize(const int& pixelSize);
	[[nodiscard]] int getTextPixelSize() const;

	void setTextPointSize(const int& pointSize);
	[[nodiscard]] int getTextPointSize() const;

	void setTextStyle(const ALTextType::TextStyle& textStyle);
	[[nodiscard]] ALTextType::TextStyle getTextStyle() const;

	void setAweSomeIcon(const ALIcon::AweSomeIcon& icon);
	[[nodiscard]] ALIcon::AweSomeIcon getAweSomeIcon() const;

	void setFluentIcon(const ALIcon::FluentIcon& icon);
	[[nodiscard]] ALIcon::FluentIcon getFluentIcon() const;

Q_SIGNALS:
	void sigAweSomeIconChanged();
	void sigFluentIconChanged();

protected:
	const QScopedPointer<CALTextPrivate> d_ptr{ nullptr };

	void paintEvent(QPaintEvent* event) override;
};
}

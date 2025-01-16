#pragma once

#include <QToolButton>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALMenu;
class CALToolButtonPrivate;

class CALWIDGETTOOLS_EXPORT CALToolButton : public QToolButton {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALToolButton)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(bool isSelected READ getIsSelected WRITE setIsSelected NOTIFY sigSelectedChanged)

public:
	explicit CALToolButton(QWidget* parent = nullptr);
	~CALToolButton() override;

	void setBorderRadius(int borderRadius);
	[[nodiscard]] int getBorderRadius() const;

	void setIsSelected(bool isSelected);
	[[nodiscard]] bool getIsSelected() const;

	void setIsTransparent(bool isTransparent);
	[[nodiscard]] bool getIsTransparent() const;

	void setMenu(CALMenu* menu);
	void setAweSomeIcon(const ALIcon::AweSomeIcon& awewomeicon);
	void setFluentIcon(const ALIcon::FluentIcon& fluenticon);

	void setToolTip(const QString& tooltip);

signals:
	void sigBorderRadiusChanged();
	void sigSelectedChanged();

protected:
	const QScopedPointer<CALToolButtonPrivate> d_ptr{ nullptr };

	bool eventFilter(QObject* watched, QEvent* event) override;
};
}

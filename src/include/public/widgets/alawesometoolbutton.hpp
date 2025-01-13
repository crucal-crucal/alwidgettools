#pragma once

#include <QToolButton>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALMenu;
class CALAwesomeToolButtonPrivate;

class CALWIDGETTOOLS_EXPORT CALAwesomeToolButton : public QToolButton {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALAwesomeToolButton)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(bool isSelected READ getIsSelected WRITE setIsSelected NOTIFY sigSelectedChanged)

public:
	explicit CALAwesomeToolButton(QWidget* parent = nullptr);
	~CALAwesomeToolButton() override;

	void setBorderRadius(int borderRadius);
	[[nodiscard]] int getBorderRadius() const;

	void setIsSelected(bool isSelected);
	[[nodiscard]] bool getIsSelected() const;

	void setIsTransparent(bool isTransparent);
	[[nodiscard]] bool getIsTransparent() const;

	void setMenu(CALMenu* menu);
	void setAweSomeIcon(const ALIcon::AweSomeIcon& icon);

	void setToolTip(const QString& tooltip);

signals:
	void sigBorderRadiusChanged();
	void sigSelectedChanged();

protected:
	const QScopedPointer<CALAwesomeToolButtonPrivate> d_ptr{ nullptr };

	bool eventFilter(QObject* watched, QEvent* event) override;
};
} // namespace AL
#pragma once

#include <QToolButton>

#include "uvwidgettoolsdef.hpp"

class CUVMenu;
class CUVAwesomeToolButtonPrivate;

class CUVWIDGETTOOLS_EXPORT CUVAwesomeToolButton : public QToolButton {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVAwesomeToolButton)
	Q_PROPERTY(int BorderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(bool isSelected READ getIsSelected WRITE setIsSelected NOTIFY sigSelectedChanged)

public:
	explicit CUVAwesomeToolButton(QWidget* parent = nullptr);
	~CUVAwesomeToolButton() override;

	void setBorderRadius(int borderRadius);
	[[nodiscard]] int getBorderRadius() const;

	void setIsSelected(bool isSelected);
	[[nodiscard]] bool getIsSelected() const;

	void setIsTransparent(bool isTransparent);
	[[nodiscard]] bool getIsTransparent() const;

	void setMenu(CUVMenu* menu);
	void setAweSomeIcon(const UVIcon::CUVAweSomeIcon& icon);

	void setToolTip(const QString& tooltip);

signals:
	void sigBorderRadiusChanged();
	void sigSelectedChanged();

protected:
	const QScopedPointer<CUVAwesomeToolButtonPrivate> d_ptr{ nullptr };

	bool eventFilter(QObject* watched, QEvent* event) override;
};

#pragma once

#include <QLineEdit>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALLineEditPrivate;

class CALWIDGETTOOLS_EXPORT CALLineEdit : public QLineEdit {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALLineEdit)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(bool isClearButtonEnable READ isClearButtonEnable WRITE setIsClearButtonEnable NOTIFY sigIsClearButtonEnableChanged)

public:
	explicit CALLineEdit(QWidget* parent = nullptr);
	~CALLineEdit() override;

	void setBorderRadius(int borderRadius);
	[[nodiscard]] int getBorderRadius() const;

	void setIsClearButtonEnable(bool isEnable);
	[[nodiscard]] bool isClearButtonEnable() const;

Q_SIGNALS:
	void sigBorderRadiusChanged();
	void sigIsClearButtonEnableChanged();
	void sigFocusIn(const QString& text);
	void sigFocusOut(const QString& text);
	void sigWmFocusOut(const QString& text);

protected:
	const QScopedPointer<CALLineEditPrivate> d_ptr{ nullptr };

	void focusInEvent(QFocusEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void contextMenuEvent(QContextMenuEvent* event) override;
};
}

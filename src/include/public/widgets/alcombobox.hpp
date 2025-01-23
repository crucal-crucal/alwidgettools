#pragma once

#include <QComboBox>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALComboBoxPrivate;

class CALWIDGETTOOLS_EXPORT CALComboBox : public QComboBox {
	Q_OBJECT
	Q_DISABLE_COPY(CALComboBox)
	Q_DECLARE_PRIVATE(CALComboBox)

	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)

public:
	explicit CALComboBox(QWidget* parent = nullptr);
	~CALComboBox() override;

	void setBorderRadius(int borderRadius);
	[[nodiscard]] int getBorderRadius() const;

Q_SIGNALS:
	Q_SIGNAL void sigBorderRadiusChanged();

protected:
	const QScopedPointer<CALComboBoxPrivate> d_ptr{ nullptr };

	void showPopup() override;
	void hidePopup() override;
};
}

#pragma once

#include <QComboBox>

#include "alwidgettoolsdef.hpp"

class CUVComboBoxPrivate;

class CALWIDGETTOOLS_EXPORT CUVComboBox : public QComboBox {
	Q_OBJECT
	Q_DISABLE_COPY(CUVComboBox)
	Q_DECLARE_PRIVATE(CUVComboBox)

	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)

public:
	explicit CUVComboBox(QWidget* parent = nullptr);
	~CUVComboBox() override;

	void setBorderRadius(int borderRadius);
	[[nodiscard]] int getBorderRadius() const;

signals:
	void sigBorderRadiusChanged();

protected:
	const QScopedPointer<CUVComboBoxPrivate> d_ptr{ nullptr };

	void showPopup() override;
	void hidePopup() override;
};

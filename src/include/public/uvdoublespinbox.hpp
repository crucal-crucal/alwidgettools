#pragma once

#include <QDoubleSpinBox>

#include "uvwidgettoolsdef.hpp"

class CUVDoubleSpinBoxPrivate;

class CUVWIDGETTOOLS_EXPORT CUVDoubleSpinBox : public QDoubleSpinBox {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVDoubleSpinBox)

public:
	explicit CUVDoubleSpinBox(QWidget* parent = nullptr);
	~CUVDoubleSpinBox() override;

protected:
	const QScopedPointer<CUVDoubleSpinBoxPrivate> d_ptr{ nullptr };

	void contextMenuEvent(QContextMenuEvent* event) override;
};

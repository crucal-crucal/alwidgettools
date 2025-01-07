#pragma once

#include <QSpinBox>

#include "alwidgettoolsdef.hpp"

class CUVSpinBoxPrivate;

class CALWIDGETTOOLS_EXPORT CUVSpinBox : public QSpinBox {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVSpinBox)

public:
	explicit CUVSpinBox(QWidget* parent = nullptr);
	~CUVSpinBox() override;

protected:
	const QScopedPointer<CUVSpinBoxPrivate> d_ptr{ nullptr };

	void contextMenuEvent(QContextMenuEvent* event) override;
};

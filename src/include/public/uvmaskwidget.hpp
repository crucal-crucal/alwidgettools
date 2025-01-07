#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class CALWIDGETTOOLS_EXPORT CUVMaskWidget : public QWidget {
	Q_OBJECT
	Q_PROPERTY(int maskAlpha READ getMaskAlpha WRITE setMaskAlpha NOTIFY sigMaskAlphaChanged)

public:
	explicit CUVMaskWidget(QWidget* parent = nullptr);
	~CUVMaskWidget() override;

	void doMaskAnimation(int endValue);

	void setMaskAlpha(const int alpha) { m_MaskAlpha = alpha; }

	[[nodiscard]] int getMaskAlpha() const { return m_MaskAlpha; }

signals:
	void sigMaskAlphaChanged(int alpha);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	int m_MaskAlpha{};
};

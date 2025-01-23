#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALWIDGETTOOLS_EXPORT CALMaskWidget : public QWidget {
	Q_OBJECT
	Q_PROPERTY(int maskAlpha READ getMaskAlpha WRITE setMaskAlpha NOTIFY sigMaskAlphaChanged)

public:
	explicit CALMaskWidget(QWidget* parent = nullptr);
	~CALMaskWidget() override;

	void doMaskAnimation(int endValue);

	void setMaskAlpha(const int alpha) { m_MaskAlpha = alpha; }

	[[nodiscard]] int getMaskAlpha() const { return m_MaskAlpha; }

Q_SIGNALS:
	Q_SIGNAL void sigMaskAlphaChanged(int alpha);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	int m_MaskAlpha{};
};
}

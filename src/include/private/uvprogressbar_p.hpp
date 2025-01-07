#pragma once

#include <QColor>

#include "alwidgettoolsdef.hpp"

class CUVProgressBar;
class CUVProgressBarDelegate;

class CUVProgressBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVProgressBarPrivate)
	Q_DECLARE_PUBLIC(CUVProgressBar)

public:
	explicit CUVProgressBarPrivate(CUVProgressBar* q, QObject* parent = nullptr);
	~CUVProgressBarPrivate() override;

	void init();

protected:
	CUVProgressBar* const q_ptr{ nullptr };
	CUVProgressBarDelegate* delegate{ nullptr };

private:
	ALProgressType::ProgressType progressType{ ALProgressType::IndeterminateProgress };
	ALProgressType::ProgressTextPosition position{ ALProgressType::Right };
	bool showProgerssText{ false };
	QColor progressColor{};
	QColor backgroundColor{};
	qreal borderRadius{ 0.0 };

	void calculateRects(QRect& progressRect, QRect& textRect, const ALProgressType::ProgressTextPosition& position) const;
};

class CUVProgressBarDelegate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVProgressBarDelegate)

	Q_PROPERTY(qreal offset WRITE setOffset READ getOffset)

public:
	explicit CUVProgressBarDelegate(CUVProgressBar* parent);
	~CUVProgressBarDelegate() override;

	void setOffset(qreal offset);
	[[nodiscard]] qreal getOffset() const;

private:
	CUVProgressBar* const m_progress{ nullptr };
	qreal m_offset{ 0.0 };
};

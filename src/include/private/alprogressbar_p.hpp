#pragma once

#include <QColor>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALProgressBar;
class CALProgressBarDelegate;

class CALProgressBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALProgressBarPrivate)
	Q_DECLARE_PUBLIC(CALProgressBar)

public:
	explicit CALProgressBarPrivate(CALProgressBar* q, QObject* parent = nullptr);
	~CALProgressBarPrivate() override;

	void init();

protected:
	CALProgressBar* const q_ptr{ nullptr };
	CALProgressBarDelegate* delegate{ nullptr };

private:
	ALProgressType::ProgressType progressType{ ALProgressType::IndeterminateProgress };
	ALProgressType::ProgressTextPosition position{ ALProgressType::Right };
	bool showProgerssText{};
	QColor progressColor{};
	QColor backgroundColor{};
	qreal borderRadius{};

	void calculateRects(QRect& progressRect, QRect& textRect, const ALProgressType::ProgressTextPosition& position) const;
};

class CALProgressBarDelegate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALProgressBarDelegate)

	Q_PROPERTY(qreal offset WRITE setOffset READ getOffset)

public:
	explicit CALProgressBarDelegate(CALProgressBar* parent);
	~CALProgressBarDelegate() override;

	void setOffset(qreal offset);
	[[nodiscard]] qreal getOffset() const;

private:
	CALProgressBar* const m_progress{ nullptr };
	qreal m_offset{};
};
}

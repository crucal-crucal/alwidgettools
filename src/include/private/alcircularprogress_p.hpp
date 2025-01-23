#pragma once

#include <QColor>

#include "alwidgettoolsdef.hpp"

class QPainter;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALCircularProgressDelegate;
class CALCircularProgress;

class CALCircularProgressPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALCircularProgressPrivate)
	Q_DECLARE_PUBLIC(CALCircularProgress)

public:
	explicit CALCircularProgressPrivate(CALCircularProgress* q, QObject* parent = nullptr);
	~CALCircularProgressPrivate() override;

	void init();

	Q_SLOT void _updateAnimation();
	void drawIndeterminate(QPainter& painter, QPen& pen);
	void drawDeterminate(QPainter& painter, const QPen& pen);
	void drawDiscontinuous(QPainter& painter, const QPen& pen);
	void drawContinuous(QPainter& painter, const QPen& pen);
	void drawValue(QPainter& painter, QPen& pen);

protected:
	CALCircularProgress* const q_ptr{ nullptr };
	CALCircularProgressDelegate* delegate{ nullptr };

private:
	ALProgressType::ProgressType progressType{ ALProgressType::DeterminateProgress };
	QColor color{};
	QColor disabledColor{};
	QColor textColor{};
	QTimer* timer{ nullptr };
	qreal penWidth{};
	bool showProgressText{};
	int size{};
	int startIndex1{};
	int endIndex1{};
	int currentIndex1{ 0xe052 };
	int startIndex2{};
	int endIndex2{};
	int currentIndex2{ 0xe100 };
};

class CALCircularProgressDelegate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALCircularProgressDelegate)
	Q_PROPERTY(qreal dashOffset WRITE setDashOffset READ getDashOffset)
	Q_PROPERTY(qreal dashLength WRITE setDashLength READ getDashLength)
	Q_PROPERTY(int angle WRITE setAngle READ getAngle)

public:
	explicit CALCircularProgressDelegate(CALCircularProgress* parent);
	~CALCircularProgressDelegate() override;

	void setDashOffset(qreal offset);
	[[nodiscard]] qreal getDashOffset() const;

	void setDashLength(qreal length);
	[[nodiscard]] qreal getDashLength() const;

	void setAngle(int nAngle);
	[[nodiscard]] int getAngle() const;

protected:
	CALCircularProgress* const progress{ nullptr };

private:
	qreal dashOffset{};
	qreal dashLength{};
	int angle{};
};
}

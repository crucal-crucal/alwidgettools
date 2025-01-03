#pragma once

#include <QColor>

#include "uvwidgettoolsdef.hpp"

class CUVCircularProgressDelegate;
class CUVCircularProgress;

class CUVCircularProgressPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVCircularProgressPrivate)
	Q_DECLARE_PUBLIC(CUVCircularProgress)

public:
	explicit CUVCircularProgressPrivate(CUVCircularProgress* q, QObject* parent = nullptr);
	~CUVCircularProgressPrivate() override;

	void init();

	Q_SLOT void _updateAnimation();
	void drawIndeterminate(QPainter& painter, QPen& pen);
	void drawDeterminate(QPainter& painter, const QPen& pen);
	void drawDiscontinuous(QPainter& painter, const QPen& pen);
	void drawContinuous(QPainter& painter, const QPen& pen);
	void drawValue(QPainter& painter, QPen& pen);

protected:
	CUVCircularProgress* const q_ptr{ nullptr };
	CUVCircularProgressDelegate* delegate{ nullptr };

private:
	UVProgressType::ProgressType progressType{ UVProgressType::DeterminateProgress };
	QColor color{};
	QColor disabledColor{};
	QColor textColor{};
	QTimer* timer{ nullptr };
	qreal penWidth{};
	bool showProgressText{ true };
	int size{};

	int startIndex1{ 0xe052 };
	int endIndex1{ 0xe0cb };
	int currentIndex1{ 0xe052 };

	int startIndex2{ 0xe100 };
	int endIndex2{ 0xe176 };
	int currentIndex2{ 0xe100 };
};

class CUVCircularProgressDelegate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVCircularProgressDelegate)
	Q_PROPERTY(qreal dashOffset WRITE setDashOffset READ getDashOffset)
	Q_PROPERTY(qreal dashLength WRITE setDashLength READ getDashLength)
	Q_PROPERTY(int angle WRITE setAngle READ getAngle)

public:
	explicit CUVCircularProgressDelegate(CUVCircularProgress* parent);
	~CUVCircularProgressDelegate() override;

	void setDashOffset(qreal offset);
	[[nodiscard]] qreal getDashOffset() const;

	void setDashLength(qreal length);
	[[nodiscard]] qreal getDashLength() const;

	void setAngle(int nAngle);
	[[nodiscard]] int getAngle() const;

private:
	CUVCircularProgress* const m_progress{ nullptr };

	qreal dashOffset{};
	qreal dashLength{};
	int angle{};
};

#pragma once

#include <QAbstractScrollArea>
#include <QObject>
#include <QScrollBar>

#include "uvscrollbar.hpp"

class QTimer;
class QPropertyAnimation;

class CUVScrollBarPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CUVScrollBarPrivate)
	Q_DECLARE_PUBLIC(CUVScrollBar)
	Q_PROPERTY(int pTargetMaximum MEMBER targetMaximum NOTIFY sigTargetMaximumChanged)

public:
	explicit CUVScrollBarPrivate(CUVScrollBar* q, QObject* parent = nullptr);
	~CUVScrollBarPrivate() override;

	Q_SLOT void slotRangeChanged(int min, int max);

	void setTargetMaximum(int targetMaximum);
	[[nodiscard]] int getTargetMaximum() const;

Q_SIGNALS:
	void sigTargetMaximumChanged();

protected:
	CUVScrollBar* const q_ptr{ nullptr };

private:
	QScrollBar* originScrollBar{ nullptr };
	QAbstractScrollArea* originScrollArea{ nullptr };
	QTimer* expandTimer{ nullptr };
	QPropertyAnimation* slideSmoothAnimation{ nullptr };
	bool isExpand{ false };
	bool isAnimation{};
	CUVScrollBar::ContextMenuFlags contextMenuFlags{ CUVScrollBar::NoFlag };
	qreal speedLimit{};
	int targetMaximum{};
	int scrollValue{ -1 };
	int lastHorizontalDeltaAngle{ -120 };
	int lastVerticalDeltaAngle{ -120 };

	void scroll(const Qt::KeyboardModifiers& modifiers, int value);
	[[nodiscard]] int pixelPosToRangeValue(int pos) const;

	void initAllConfig();
	static void handleScrollBarValueChanged(QScrollBar* scrollBar, int value);
	void handleScrollBarRangeChanged(int min, int max);
	void handleScrollBarGeometry();
};

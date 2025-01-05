#pragma once

#include <QObject>

class CUVScrollBar;
class CUVScrollArea;

class CUVScrollAreaPrivate final : public QObject {
	Q_OBJECT
	Q_DECLARE_PUBLIC(CUVScrollArea)

public:
	explicit CUVScrollAreaPrivate(CUVScrollArea* q, QObject* parent = nullptr);
	~CUVScrollAreaPrivate() override;

	[[nodiscard]] CUVScrollBar* getScrollBar(const Qt::Orientation& orientation) const;

protected:
	CUVScrollArea* const q_ptr{ nullptr };

private:
	CUVScrollBar* horizontalScrollBar{ nullptr };
	CUVScrollBar* verticalScrollBar{ nullptr };
};

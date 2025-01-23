#pragma once

#include <QLayout>
#include <QObject>
#include <QMap>
#include <QStyle>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALFlowLayout;

class CALFlowLayoutPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALFlowLayoutPrivate)
	Q_DECLARE_PUBLIC(CALFlowLayout)

public:
	explicit CALFlowLayoutPrivate(QObject* parent = nullptr, CALFlowLayout* q = nullptr);
	~CALFlowLayoutPrivate() override;

protected:
	CALFlowLayout* const q_ptr{ nullptr };

private:
	mutable QMap<CALFlowLayout*, int> lastHeightMap{};
	mutable QMap<QLayoutItem*, QPoint> lastGeometryMap{};
	QList<QLayoutItem*> itemList{};
	bool isAnimation{};
	int hSpacing{};
	int vSpacing{};

	int doLayout(const QRect& rect, bool testOnly) const;
	int smartSpacing(QStyle::PixelMetric pm) const;
};
}

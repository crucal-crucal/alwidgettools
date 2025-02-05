#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPovitPrivate;

class CALWIDGETTOOLS_EXPORT CALPovit : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALPovit)
	Q_PROPERTY(int textPixelSize READ getTextPixelSize WRITE setTextPixelSize NOTIFY sigTextPixelSizeChanged)
	Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY sigCurrentIndexChanged)
	Q_PROPERTY(int povitSpacing READ getPovitSpacing WRITE setPovitSpacing NOTIFY sigPovitSpacingChanged)
	Q_PROPERTY(int markWidth READ getMarkWidth WRITE setMarkWidth NOTIFY sigMarkWidthChanged)

public:
	explicit CALPovit(QWidget* parent = nullptr);
	~CALPovit() override;

	int appendPovit(const QString& povieTitle);
	bool removePovit(const QString& povieTitle);
	bool removePovit(int index);

	void setTextPixelSize(int textPixelSize);
	[[nodiscard]] int getTextPixelSize() const;

	void setCurrentIndex(int currentindex);
	[[nodiscard]] int currentIndex() const;

	void setPovitSpacing(int spacing);
	[[nodiscard]] int getPovitSpacing() const;

	void setMarkWidth(int width);
	[[nodiscard]] int getMarkWidth() const;

Q_SIGNALS:
	void sigPovitClicked(int index);
	void sigPovitDoubleClicked(int index);
	void sigTextPixelSizeChanged();
	void sigCurrentIndexChanged();
	void sigPovitSpacingChanged();
	void sigMarkWidthChanged();

protected:
	const QScopedPointer<CALPovitPrivate> d_ptr{ nullptr };
};
}

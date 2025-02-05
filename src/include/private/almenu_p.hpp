#pragma once

#include <QPixmap>
#include <QPoint>
#include <QObject>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALMenu;
class CALMenuStyle;

class CALMenuPrivate final : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(CALMenuPrivate)
	Q_DECLARE_PUBLIC(CALMenu)

	Q_PROPERTY(int animationImagePosY READ getAnimationImagePosY WRITE setAnimationImagePosY NOTIFY onAnimationImagePosYChanged)

public:
	explicit CALMenuPrivate(CALMenu* q, QObject* parent = nullptr);
	~CALMenuPrivate() override;

	void setAnimationImagePosY(int pos);
	[[nodiscard]] int getAnimationImagePosY() const;

Q_SIGNALS:
	void onAnimationImagePosYChanged();

protected:
	CALMenu* const q_ptr{ nullptr };

private:
	int animationImagePosY{};
	QPixmap animationPix;
	bool isCloseAnimation{};
	QPoint mousePressPoint;
	CALMenuStyle* menuStyle{ nullptr };
};
}

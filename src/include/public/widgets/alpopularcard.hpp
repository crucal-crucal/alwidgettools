#pragma once

#include <QWidget>
#include <QPixmap>

#include "alwidgettoolsdef.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPopularCardPrivate;

class CALWIDGETTOOLS_EXPORT CALPopularCard : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALPopularCard)
	Q_PROPERTY(qreal borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(QWidget* cardFloatArea READ getCardFloatArea WRITE setCardFloatArea NOTIFY sigCardFloatAreaChanged)
	Q_PROPERTY(QPixmap cardPixmap READ getCardPixmap WRITE setCardPixmap NOTIFY sigCardPixmapChanged)
	Q_PROPERTY(QPixmap cardFloatPixmap READ getCardFloatPixmap WRITE setCardFloatPixmap NOTIFY sigCardFloatPixmapChanged)
	Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY sigTitleChanged)
	Q_PROPERTY(QString subTitle READ getSubTitle WRITE setSubTitle NOTIFY sigSubTitleChanged)
	Q_PROPERTY(QString interactiveTips READ getInteractiveTips WRITE setInteractiveTips NOTIFY sigInteractiveTipsChanged)
	Q_PROPERTY(QString detailedText READ getDetailedText WRITE setDetailedText NOTIFY sigDetailedTextChanged)
	Q_PROPERTY(QString cardButtonText READ getCardButtonText WRITE setCardButtonText NOTIFY sigCardButtonTextChanged)

public:
	explicit CALPopularCard(QWidget* parent = nullptr);
	~CALPopularCard() override;

	void setBorderRadius(qreal radius);
	[[nodiscard]] qreal getBorderRadius() const;

	void setCardFloatArea(QWidget* cardFloatArea);
	[[nodiscard]] QWidget* getCardFloatArea() const;

	void setCardPixmap(const QPixmap& pixmap);
	[[nodiscard]] QPixmap getCardPixmap() const;

	void setCardFloatPixmap(const QPixmap& pixmap);
	[[nodiscard]] QPixmap getCardFloatPixmap() const;

	void setTitle(const QString& title);
	[[nodiscard]] QString getTitle() const;

	void setSubTitle(const QString& subTitle);
	[[nodiscard]] QString getSubTitle() const;

	void setInteractiveTips(const QString& interactiveTips);
	[[nodiscard]] QString getInteractiveTips() const;

	void setDetailedText(const QString& detailedText);
	[[nodiscard]] QString getDetailedText() const;

	void setCardButtonText(const QString& cardButtonText);
	[[nodiscard]] QString getCardButtonText() const;

Q_SIGNALS:
	void sigPopularCardClicked();
	void sigPopularCardButtonClicked();
	void sigBorderRadiusChanged();
	void sigCardFloatAreaChanged();
	void sigCardPixmapChanged();
	void sigCardFloatPixmapChanged();
	void sigTitleChanged();
	void sigSubTitleChanged();
	void sigInteractiveTipsChanged();
	void sigDetailedTextChanged();
	void sigCardButtonTextChanged();

protected:
	const QScopedPointer<CALPopularCardPrivate> d_ptr{ nullptr };

	bool event(QEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
};
}

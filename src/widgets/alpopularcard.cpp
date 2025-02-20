#include "alpopularcard.hpp"

#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTimer>

#include "alpopularcard_p.hpp"
#include "alpopularcardfloater.hpp"
#include "alpushbutton.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALPopularCardPrivate
 * Internal class for CALPopularCard
 * @param q pointer to the public class
 * @param parent pointer to the parent class
 */
CALPopularCardPrivate::CALPopularCardPrivate(CALPopularCard* q, QObject* parent): QObject(parent), q_ptr(q) {
}

CALPopularCardPrivate::~CALPopularCardPrivate() = default;

void CALPopularCardPrivate::setHoverYOffset(const qreal offset) {
	hoverYOffset = offset;
	Q_EMIT sigHoverYOffsetChanged();
}

qreal CALPopularCardPrivate::getHoverYOffset() const {
	return hoverYOffset;
}

void CALPopularCardPrivate::setHoverOpacity(const qreal opacity) {
	hoverOpacity = opacity;
	Q_EMIT sigHoverOpacityChanged();
}

qreal CALPopularCardPrivate::getHoverOpacity() const {
	return hoverOpacity;
}

void CALPopularCardPrivate::showFloater() {
	Q_Q(CALPopularCard);

	q->update();
	isFloating = true;
	hoverYOffset = 0;
	floatTimer->stop();
	floater->showFloater();
}

/**
 * @brief \class CALPopularCard
 * @param parent pointer to the parent class
 */
CALPopularCard::CALPopularCard(QWidget* parent): QWidget(parent), d_ptr(new CALPopularCardPrivate(this, this)) {
	Q_D(CALPopularCard);

	d->borderRadius = 8.0;
	d->hoverYOffset = 0;
	d->hoverOpacity = 0;
	d->shadowBorderWidth = 6;
	d->textHSpacing = 20;
	d->textVSpacing = 5;
	d->isFloating = false;
	d->cardFloatArea = parentWidget();
	d->floater = new CALPopularCardFloater(this, d, d->cardFloatArea);
	d->floater->setVisible(false);
	d->floatTimer = new QTimer(this);
	connect(d->floatTimer, &QTimer::timeout, d, &CALPopularCardPrivate::showFloater);
	d->themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [d](const ALThemeType::ThemeMode& mode) { d->themeMode = mode; });

	setObjectName("CALPopularCard");
	setStyleSheet("#CALPopularCard { background-color: transparent; }");
	setFixedSize(320, 120);
	setMouseTracking(true);
}

CALPopularCard::~CALPopularCard() = default;

void CALPopularCard::setBorderRadius(const qreal radius) {
	d_func()->borderRadius = radius;
	Q_EMIT sigBorderRadiusChanged();
}

qreal CALPopularCard::getBorderRadius() const {
	return d_func()->borderRadius;
}

void CALPopularCard::setCardFloatArea(QWidget* cardFloatArea) {
	Q_D(CALPopularCard);

	if (!cardFloatArea && cardFloatArea == this) {
		return;
	}

	d->cardFloatArea = cardFloatArea;
	d->floater->setParent(cardFloatArea);
	Q_EMIT sigCardFloatAreaChanged();
}

QWidget* CALPopularCard::getCardFloatArea() const {
	return d_func()->cardFloatArea;
}

void CALPopularCard::setCardPixmap(const QPixmap& pixmap) {
	d_func()->cardPixmap = pixmap;
	Q_EMIT sigCardPixmapChanged();
}

QPixmap CALPopularCard::getCardPixmap() const {
	return d_func()->cardPixmap;
}

void CALPopularCard::setCardFloatPixmap(const QPixmap& pixmap) {
	d_func()->cardFloatPixmap = pixmap;
	Q_EMIT sigCardFloatPixmapChanged();
}

QPixmap CALPopularCard::getCardFloatPixmap() const {
	return d_func()->cardFloatPixmap;
}

void CALPopularCard::setTitle(const QString& title) {
	d_func()->title = title;
	Q_EMIT sigTitleChanged();
}

QString CALPopularCard::getTitle() const {
	return d_func()->title;
}

void CALPopularCard::setSubTitle(const QString& subTitle) {
	d_func()->subTitle = subTitle;
	Q_EMIT sigSubTitleChanged();
}

QString CALPopularCard::getSubTitle() const {
	return d_func()->subTitle;
}

void CALPopularCard::setInteractiveTips(const QString& interactiveTips) {
	d_func()->interactiveTips = interactiveTips;
	Q_EMIT sigInteractiveTipsChanged();
}

QString CALPopularCard::getInteractiveTips() const {
	return d_func()->interactiveTips;
}

void CALPopularCard::setDetailedText(const QString& detailedText) {
	d_func()->detailedText = detailedText;
	Q_EMIT sigDetailedTextChanged();
}

QString CALPopularCard::getDetailedText() const {
	return d_func()->detailedText;
}

void CALPopularCard::setCardButtonText(const QString& cardButtonText) {
	Q_D(CALPopularCard);

	if (cardButtonText.isEmpty()) {
		return;
	}

	d->cardButtonText = cardButtonText;
	d->floater->m_overButton->setText(d->cardButtonText);
	Q_EMIT sigCardButtonTextChanged();
}

QString CALPopularCard::getCardButtonText() const {
	return d_func()->cardButtonText;
}

bool CALPopularCard::event(QEvent* event) {
	Q_D(CALPopularCard);

	switch (event->type()) {
		case QEvent::Enter: {
			d->floatTimer->start(450);
			const auto hoverAnimation = new QPropertyAnimation(d, "hoverYOffset");
			connect(hoverAnimation, &QPropertyAnimation::valueChanged, this, [this]() { update(); });
			hoverAnimation->setDuration(130);
			hoverAnimation->setStartValue(d->hoverYOffset);
			hoverAnimation->setEndValue(6);
			hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			const auto opacityAnimation = new QPropertyAnimation(d, "hoverOpacity");
			opacityAnimation->setDuration(130);
			opacityAnimation->setStartValue(d->hoverOpacity);
			opacityAnimation->setEndValue(1);
			opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			break;
		}
		case QEvent::Leave: {
			d->floatTimer->stop();
			const auto hoverAnimation = new QPropertyAnimation(d, "hoverYOffset");
			connect(hoverAnimation, &QPropertyAnimation::valueChanged, this, [this]() { update(); });
			hoverAnimation->setDuration(130);
			hoverAnimation->setStartValue(d->hoverYOffset);
			hoverAnimation->setEndValue(0);
			hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			const auto opacityAnimation = new QPropertyAnimation(d, "hoverOpacity");
			opacityAnimation->setDuration(130);
			opacityAnimation->setStartValue(d->hoverOpacity);
			opacityAnimation->setEndValue(0);
			opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			break;
		}
		case QEvent::MouseMove: {
			if (d->isFloating) {
				const QPoint globalPos = QCursor::pos();
				const QRect thisRect = rect();
				// 如果鼠标不在 this 或 floater 区域内，并且 isFloating 为 true
				if (const QRect floaterRect = d->floater->rect(); !thisRect.contains(mapFromGlobal(globalPos)) && !floaterRect.contains(mapFromGlobal(globalPos))) {
					d->floatTimer->stop();
					const auto hoverAnimation = new QPropertyAnimation(d, "hoverYOffset");
					connect(hoverAnimation, &QPropertyAnimation::valueChanged, this, [this]() { update(); });
					hoverAnimation->setDuration(130);
					hoverAnimation->setStartValue(d->hoverYOffset);
					hoverAnimation->setEndValue(0);
					hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);
					const auto opacityAnimation = new QPropertyAnimation(d, "hoverOpacity");
					opacityAnimation->setDuration(130);
					opacityAnimation->setStartValue(d->hoverOpacity);
					opacityAnimation->setEndValue(0);
					opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
				}
			}
			break;
		}
		case QEvent::MouseButtonRelease: {
			Q_EMIT sigPopularCardClicked();
			break;
		}
		default: {
			break;;
		}
	}

	return QWidget::event(event);
}

void CALPopularCard::paintEvent(QPaintEvent* event) {
	Q_D(CALPopularCard);

	if (d->isFloating) {
		return;
	}

	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	// shadow
	if (underMouse()) {
		painter.setOpacity(d->hoverOpacity);
		QRect shadowRect = rect();
		shadowRect.adjust(0, 0, 0, -d->hoverYOffset); // NOLINT
		ALTheme->drawEffectShadow(&painter, shadowRect, d->shadowBorderWidth, d->borderRadius);
	}
	// background
	const QRectF foregroundRect(d->shadowBorderWidth, d->shadowBorderWidth - d->hoverYOffset + 1, width() - 2 * d->shadowBorderWidth, height() - 2 * d->shadowBorderWidth);
	painter.setOpacity(1);
	painter.setPen(ALThemeColor(d->themeMode, underMouse() ? ALThemeType::PopupBorderHover : ALThemeType::BasicBorder));
	painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::BasicBaseAlpha));
	painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
	// pixmap
	painter.save();
	const QRectF pixRect(foregroundRect.x() + foregroundRect.height() * 0.15, foregroundRect.y() + foregroundRect.height() * 0.15, foregroundRect.height() * 0.7, foregroundRect.height() * 0.7);
	QPainterPath pixPath;
	pixPath.addRoundedRect(pixRect, 4, 4);
	painter.setClipPath(pixPath);
	painter.drawPixmap(pixRect, d->cardPixmap, d->cardPixmap.rect());
	painter.restore();
	/// text
	// calculate the ent rect
	const qreal buttonTargetWidth = qMin(width() * 0.25, d->floater->m_overButton->fontMetrics().horizontalAdvance(d->floater->m_overButton->text()) + 40.0);
	// clapushbutton shadow offset
	d->buttonTargetRect = QRect(QPoint(width() + 2 * d->floater->m_floatGeometryOffset - d->shadowBorderWidth + 3 - foregroundRect.height() * 0.15 - buttonTargetWidth, foregroundRect.height() * 0.15 - 3), QSize(buttonTargetWidth, 36)); // NOLINT
	// title
	painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicText));
	QFont font = painter.font();
	font.setWeight(QFont::Bold);
	font.setPixelSize(15);
	painter.setFont(font);
	const int titleHeight = painter.fontMetrics().height();
	const QRectF titleRect(pixRect.right() + d->textHSpacing, pixRect.y(), d->floater->m_floatGeometryOffset * 2 + foregroundRect.width() - pixRect.width() - d->textHSpacing * 2 - foregroundRect.height() * 0.15 - buttonTargetWidth, titleHeight);
	const QString titleText = painter.fontMetrics().elidedText(d->title, Qt::ElideRight, titleRect.width()); // NOLINT
	painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine | Qt::TextWordWrap, titleText);
	// subtitle
	font.setWeight(QFont::DemiBold);
	font.setPixelSize(13);
	painter.setFont(font);
	const int subTitleHeight = painter.fontMetrics().height();
	const QRectF subTitleRect(pixRect.right() + d->textHSpacing, titleRect.bottom() + d->textVSpacing, d->floater->m_floatGeometryOffset * 2 + foregroundRect.width() - pixRect.width() - d->textHSpacing * 2 - foregroundRect.height() * 0.15 - buttonTargetWidth, subTitleHeight);
	const QString subTitleText = painter.fontMetrics().elidedText(d->subTitle, Qt::ElideRight, subTitleRect.width()); // NOLINT
	painter.drawText(subTitleRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine | Qt::TextWordWrap, subTitleText);
	// InteractiveTips
	const int tipWidth = painter.fontMetrics().horizontalAdvance(d->interactiveTips);
	const int tipHeight = painter.fontMetrics().height();
	if (!d->interactiveTips.isEmpty()) {
		font.setWeight(QFont::DemiBold);
		font.setPixelSize(12);
		painter.setFont(font);
		//覆盖背景绘制
		const QRectF tipRect(foregroundRect.right() - d->textHSpacing - tipWidth, foregroundRect.bottom() - d->textHSpacing - tipHeight, foregroundRect.width() / 2 - d->textHSpacing, tipHeight);
		painter.setPen(Qt::NoPen);
		painter.setBrush(ALThemeColor(d->themeMode, ALThemeType::BasicBaseDeep));
		QRectF baseRect = tipRect;
		baseRect.setRight(tipRect.x() + tipWidth);
		baseRect.adjust(-7, -3, 4, 3);
		d->interactiveTipsBaseRect = baseRect;
		painter.drawRoundedRect(baseRect, 6, 6);
		//文字绘制
		painter.setPen(ALThemeColor(d->themeMode, ALThemeType::BasicText));
		painter.drawText(tipRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextWordWrap, d->interactiveTips);
	} else {
		QRectF tipRect(foregroundRect.right() - d->textHSpacing - 50, foregroundRect.bottom() - d->textHSpacing - tipHeight, foregroundRect.width() / 2 - d->textHSpacing, tipHeight);
		tipRect.setRight(tipRect.x() + tipWidth);
		tipRect.adjust(-7, -3, 4, 3);
		d->interactiveTipsBaseRect = tipRect;
	}
	painter.restore();
}
}

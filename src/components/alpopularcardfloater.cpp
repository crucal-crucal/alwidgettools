#include "alpopularcardfloater.hpp"

#include <QEvent>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "alpopularcard.hpp"
#include "alpopularcard_p.hpp"
#include "alpushbutton.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALPopularCardFloater
 * @param card pointer to CALPopularCard
 * @param cardPrivate pointer to CALPopularCardPrivate
 * @param parent pointer to the parent class
 */
CALPopularCardFloater::CALPopularCardFloater(CALPopularCard* card, CALPopularCardPrivate* cardPrivate, QWidget* parent): QWidget(parent) {
	m_card = card;
	m_cardPrivate = cardPrivate;
	m_hoverOpacity = 0;
	m_hoverYOffset = 0;
	m_isHideAnimationFinished = true;
	m_floatGeometryOffset = 25;

	m_overButton = new CALPushButton(tr("get"), this);
	m_opacityEffect = new QGraphicsOpacityEffect(m_overButton);
	m_opacityEffect->setOpacity(1);
	m_overButton->setGraphicsEffect(m_opacityEffect);
	m_overButton->move(0, 0);
	m_overButton->setLightDefaultColor(ALThemeColor(ALThemeType::Light, ALThemeType::PrimaryNormal));
	m_overButton->setLightHoverColor(ALThemeColor(ALThemeType::Light, ALThemeType::PrimaryHover));
	m_overButton->setLightPressedColor(ALThemeColor(ALThemeType::Light, ALThemeType::PrimaryPress));
	m_overButton->setLightTextColor(Qt::white);
	m_overButton->setDarkDefaultColor(ALThemeColor(ALThemeType::Dark, ALThemeType::PrimaryNormal));
	m_overButton->setDarkHoverColor(ALThemeColor(ALThemeType::Dark, ALThemeType::PrimaryHover));
	m_overButton->setDarkPressedColor(ALThemeColor(ALThemeType::Dark, ALThemeType::PrimaryPress));
	m_overButton->setDarkTextColor(Qt::white);
	m_overButton->setMinimumSize(0, 0);
	m_overButton->setMaximumSize({ QWIDGETSIZE_MAX, QWIDGETSIZE_MAX });
	connect(m_overButton, &CALPushButton::clicked, m_card, &CALPopularCard::sigPopularCardButtonClicked);

	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALPopularCardFloater::~CALPopularCardFloater() = default;

void CALPopularCardFloater::showFloater() {
	m_isHideAnimationFinished = true;
	m_hoverYOffset = 6;
	m_hoverOpacity = 1;
	m_opacityEffect->setOpacity(1);

	// geometry animation
	const auto geometryAnimation = new QPropertyAnimation(this, "geometry");
	connect(geometryAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
	geometryAnimation->setDuration(300);
	geometryAnimation->setEasingCurve(QEasingCurve::OutQuad);
	const auto cardGeometry = QRect(m_card->mapTo(m_cardPrivate->cardFloatArea, QPoint(0, 0)), m_card->size());
	const QRect endGeometry = calculateTargetGeometry(cardGeometry);
	const auto startGeometry = QRect(endGeometry.x() + m_floatGeometryOffset, endGeometry.y() + m_floatGeometryOffset, m_card->width(), m_card->height());
	geometryAnimation->setStartValue(startGeometry);
	geometryAnimation->setEndValue(endGeometry);
	geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	// button animation
	const auto buttonAnimation = new QPropertyAnimation(m_overButton, "geometry");
	buttonAnimation->setDuration(300);
	buttonAnimation->setEasingCurve(QEasingCurve::OutQuad);
	buttonAnimation->setStartValue(m_cardPrivate->interactiveTipsBaseRect);
	buttonAnimation->setEndValue(m_cardPrivate->buttonTargetRect);
	buttonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	setVisible(true);
}

void CALPopularCardFloater::hideFloater() {
	if (!m_isHideAnimationFinished) {
		return;
	}

	m_isHideAnimationFinished = false;
	// geometry animation
	const auto geometryAnimation = new QPropertyAnimation(this, "geometry");
	connect(geometryAnimation, &QPropertyAnimation::finished, this, [=]() {
		m_cardPrivate->isFloating = false;
		setVisible(false);
		m_card->update();
	});
	connect(geometryAnimation, &QPropertyAnimation::valueChanged, this, [=]() { geometryAnimation->setEndValue(QRect(m_card->mapTo(m_cardPrivate->cardFloatArea, QPoint(0, 0)), m_card->size())); });
	geometryAnimation->setDuration(300);
	geometryAnimation->setEasingCurve(QEasingCurve::InOutSine);
	geometryAnimation->setStartValue(geometry());
	geometryAnimation->setEndValue(QRect(m_card->mapTo(m_cardPrivate->cardFloatArea, QPoint(0, 0)), m_card->size()));
	geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	// opacity animation
	const auto opacityAnimation = new QPropertyAnimation(this, "hoverOpacity");
	opacityAnimation->setDuration(300);
	opacityAnimation->setStartValue(m_hoverOpacity);
	opacityAnimation->setEndValue(0);
	opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	// hover animation
	const auto hoverAnimation = new QPropertyAnimation(this, "hoverYOffset");
	connect(hoverAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
	hoverAnimation->setDuration(300);
	hoverAnimation->setStartValue(m_hoverYOffset);
	hoverAnimation->setEndValue(0);
	hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	// button animation
	const auto buttonAnimation = new QPropertyAnimation(m_overButton, "geometry");
	buttonAnimation->setDuration(300);
	buttonAnimation->setEasingCurve(QEasingCurve::InOutSine);
	buttonAnimation->setStartValue(m_overButton->geometry());
	QRectF endRect = m_cardPrivate->interactiveTipsBaseRect;
	endRect.adjust(0, 6, 0, 6);
	buttonAnimation->setEndValue(endRect);
	buttonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	// button graphicsEffect opacity animation
	const auto buttonOpacityAnimation = new QPropertyAnimation(m_overButton->graphicsEffect(), "opacity");
	buttonOpacityAnimation->setDuration(300);
	buttonOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
	buttonOpacityAnimation->setStartValue(1);
	buttonOpacityAnimation->setEndValue(0);
	buttonOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CALPopularCardFloater::setHoverYOffset(const qreal offset) {
	m_hoverYOffset = offset;
	Q_EMIT sigHoverYOffsetChanged();
}

qreal CALPopularCardFloater::getHoverYOffset() const {
	return m_hoverYOffset;
}

void CALPopularCardFloater::setHoverOpacity(const qreal opacity) {
	m_hoverOpacity = opacity;
	Q_EMIT sigHoverOpacityChanged();
}

qreal CALPopularCardFloater::getHoverOpacity() const {
	return m_hoverOpacity;
}

bool CALPopularCardFloater::event(QEvent* event) {
	switch (event->type()) {
		case QEvent::Leave: {
			hideFloater();
			break;
		}
		case QEvent::MouseButtonRelease: {
			Q_EMIT m_card->sigPopularCardClicked();
			break;
		}
		default: {
			break;
		}
	}

	return QWidget::event(event);
}

void CALPopularCardFloater::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	// shadow
	painter.setOpacity(m_hoverOpacity);
	QRect shadowRect = rect();
	shadowRect.adjust(0, 0, 0, -m_hoverYOffset); // NOLINT
	ALTheme->drawEffectShadow(&painter, shadowRect, m_cardPrivate->shadowBorderWidth, m_cardPrivate->borderRadius);
	// background
	const QRectF foregroundRect(m_cardPrivate->shadowBorderWidth, m_cardPrivate->shadowBorderWidth - m_hoverYOffset + 1, width() - 2 * m_cardPrivate->shadowBorderWidth, height() - 2 * m_cardPrivate->shadowBorderWidth);
	const QRectF cardForegroundRect(m_cardPrivate->shadowBorderWidth, m_cardPrivate->shadowBorderWidth - m_cardPrivate->hoverYOffset + 1, m_card->width() - 2 * m_cardPrivate->shadowBorderWidth, m_card->height() - 2 * m_cardPrivate->shadowBorderWidth);
	painter.setOpacity(1);
	painter.setPen(ALThemeColor(m_themeMode, ALThemeType::PopupBorderHover));
	painter.setBrush(ALThemeColor(m_themeMode, ALThemeType::DialogBase));
	painter.drawRoundedRect(foregroundRect, m_cardPrivate->borderRadius, m_cardPrivate->borderRadius);
	painter.setClipRect(foregroundRect);
	// pixmap
	painter.save();
	const QRectF pixRect(foregroundRect.x() + cardForegroundRect.height() * 0.15, foregroundRect.y() + cardForegroundRect.height() * 0.15, cardForegroundRect.height() * 0.7, cardForegroundRect.height() * 0.7);
	QPainterPath pixPath;
	pixPath.addRoundedRect(pixRect, 4, 4);
	painter.setClipPath(pixPath);
	painter.drawPixmap(pixRect, m_cardPrivate->cardPixmap, m_cardPrivate->cardPixmap.rect());
	painter.restore();
	/// text
	// title
	painter.setPen(ALThemeColor(m_themeMode, ALThemeType::BasicText));
	QFont font = painter.font();
	font.setWeight(QFont::Bold);
	font.setPixelSize(15);
	painter.setFont(font);
	const int titleHeight = painter.fontMetrics().height();
	const QRectF titleRect(pixRect.right() + m_cardPrivate->textHSpacing, pixRect.y(), m_floatGeometryOffset * 2 + cardForegroundRect.width() - pixRect.width() - m_cardPrivate->textHSpacing * 2 - cardForegroundRect.height() * 0.15 - m_cardPrivate->buttonTargetRect.width(), titleHeight);
	const QString titleText = painter.fontMetrics().elidedText(m_cardPrivate->title, Qt::ElideRight, titleRect.width()); // NOLINT
	painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, titleText);
	// subtitle
	font.setWeight(QFont::DemiBold);
	font.setPixelSize(13);
	painter.setFont(font);
	const int subTitleHeight = painter.fontMetrics().height();
	const QRectF subTitleRect(pixRect.right() + m_cardPrivate->textHSpacing, titleRect.bottom() + m_cardPrivate->textVSpacing,
	                    m_floatGeometryOffset * 2 + cardForegroundRect.width() - pixRect.width() - m_cardPrivate->textHSpacing * 2 - cardForegroundRect.height() * 0.15 - m_cardPrivate->buttonTargetRect.width(), subTitleHeight);
	const QString subTitleText = painter.fontMetrics().elidedText(m_cardPrivate->subTitle, Qt::ElideRight, subTitleRect.width()); // NOLINT
	painter.drawText(subTitleRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, subTitleText);
	// detailedText
	painter.setPen(ALThemeColor(m_themeMode, ALThemeType::BasicDetailsText));
	const int detailedTextHeight = painter.fontMetrics().height() * 2 + 2;
	const QRectF detailedTextRect(pixRect.x(), pixRect.bottom() + cardForegroundRect.height() * 0.15, cardForegroundRect.width() + 2 * m_floatGeometryOffset - m_cardPrivate->textHSpacing - cardForegroundRect.height() * 0.15, detailedTextHeight);
	const QString detailedText = painter.fontMetrics().elidedText(m_cardPrivate->detailedText, Qt::ElideRight, detailedTextRect.width() * 1.9); // NOLINT
	painter.drawText(detailedTextRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, detailedText);
	// Parting line
	painter.setPen(ALThemeColor(m_themeMode, ALThemeType::BasicBaseLine));
	painter.drawLine(foregroundRect.x(), detailedTextRect.bottom() + 5, foregroundRect.right(), detailedTextRect.bottom() + 5); // NOLINT
	// cardFloatPixmap
	painter.drawPixmap(QRect(pixRect.x(), detailedTextRect.bottom() + 15, cardForegroundRect.bottom() + m_floatGeometryOffset + 90 - detailedTextRect.bottom() - 15 - m_cardPrivate->shadowBorderWidth - 10, // NOLINT
	                         cardForegroundRect.bottom() + m_floatGeometryOffset + 90 - detailedTextRect.bottom() - 15 - m_cardPrivate->shadowBorderWidth - 10), m_cardPrivate->cardFloatPixmap);            // NOLINT
	painter.restore();
}

QRect CALPopularCardFloater::calculateTargetGeometry(const QRect& cardGeometry) const {
	QRect targetGeometry = cardGeometry;
	targetGeometry.adjust(-m_floatGeometryOffset, -m_floatGeometryOffset, m_floatGeometryOffset, 90);
	const QRect windowRect = m_cardPrivate->cardFloatArea->rect();
	if (targetGeometry.bottom() > windowRect.bottom()) {
		const int offset = targetGeometry.bottom() - windowRect.bottom();
		targetGeometry.adjust(0, -offset, 0, -offset);
	}
	if (targetGeometry.top() < windowRect.top()) {
		const int offset = targetGeometry.top() - windowRect.top();
		targetGeometry.adjust(0, -offset, 0, -offset);
	}
	return targetGeometry;
}
}

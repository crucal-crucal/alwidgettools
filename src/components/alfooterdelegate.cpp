#include "alfooterdelegate.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QDebug>

#include "albaselistview.hpp"
#include "alfootermodel.hpp"
#include "alnavigationnode.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALFooterDelegate
 * @param parent pointer to the parent class
 */
CALFooterDelegate::CALFooterDelegate(QObject* parent): QStyledItemDelegate(parent) {
	themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [this](const ALThemeType::ThemeMode& mode) { themeMode = mode; });
	setProperty("lastSelectMarkTop", 10.0);
	setProperty("lastSelectMarkBottom", 10.0);
	setProperty("selectMarkTop", 10.0);
	setProperty("selectMarkBottom", 10.0);
	// Mark向上
	m_lastSelectMarkTopAnimation = new QPropertyAnimation(this, "lastSelectMarkTop");
	m_lastSelectMarkTopAnimation->setDuration(300);
	m_lastSelectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
	connect(m_lastSelectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) {
		m_lastSelectMarkTop = value.toReal();
		m_listView->viewport()->update();
	});

	m_selectMarkBottomAnimation = new QPropertyAnimation(this, "selectMarkBottom");
	m_selectMarkBottomAnimation->setDuration(300);
	m_selectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
	connect(m_selectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) {
		m_selectMarkBottom = value.toReal();
		m_listView->viewport()->update();
	});
	connect(m_lastSelectMarkTopAnimation, &QPropertyAnimation::finished, this, [this]() {
		m_isSelectMarkDisplay = true;
		m_lastSelectedNode = nullptr;
		m_selectMarkBottomAnimation->setStartValue(0);
		m_selectMarkBottomAnimation->setEndValue(10);
		m_selectMarkBottomAnimation->start();
	});

	// Mark向下
	m_lastSelectMarkBottomAnimation = new QPropertyAnimation(this, "lastSelectMarkBottom");
	m_lastSelectMarkBottomAnimation->setDuration(300);
	m_lastSelectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
	connect(m_lastSelectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) {
		m_lastSelectMarkBottom = value.toReal();
		m_listView->viewport()->update();
	});

	m_selectMarkTopAnimation = new QPropertyAnimation(this, "selectMarkTop");
	m_selectMarkTopAnimation->setDuration(300);
	m_selectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
	connect(m_selectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) {
		m_selectMarkTop = value.toReal();
		m_listView->viewport()->update();
	});
	connect(m_lastSelectMarkBottomAnimation, &QPropertyAnimation::finished, this, [this]() {
		m_isSelectMarkDisplay = true;
		m_lastSelectedNode = nullptr;
		m_selectMarkTopAnimation->setStartValue(0);
		m_selectMarkTopAnimation->setEndValue(10);
		m_selectMarkTopAnimation->start();
	});
}

CALFooterDelegate::~CALFooterDelegate() = default;

void CALFooterDelegate::setCALBaseListView(CALBaseListView* listView) {
	m_listView = listView;
}

CALBaseListView* CALFooterDelegate::getCALBaseListView() const {
	return m_listView;
}

void CALFooterDelegate::setPressIndex(const QModelIndex& index) {
	m_pressIndex = index;
}

QModelIndex CALFooterDelegate::getPressIndex() const {
	return m_pressIndex;
}

void CALFooterDelegate::navigationNodeStateChange(const QVariantMap& data) {
	if (data.contains("SelectMarkChanged")) {
		m_lastSelectedNode = data.value("LastSelectedNode").value<CALNavigationNode*>();
		const auto selectedNode = data.value("SelectedNode").value<CALNavigationNode*>();
		const bool direction = compareItemY(selectedNode, m_lastSelectedNode);
		m_lastSelectMarkTop = 10;
		m_lastSelectMarkBottom = 10;
		m_selectMarkTop = 10;
		m_selectMarkBottom = 10;
		const auto& lastSelectedMarkStartAnimation = direction ? m_lastSelectMarkTopAnimation : m_lastSelectMarkBottomAnimation;
		const auto& lastSelectedMarkStopAnimation = direction ? m_lastSelectMarkBottomAnimation : m_lastSelectMarkTopAnimation;
		const auto& selectMarkAnimation = direction ? m_selectMarkTopAnimation : m_selectMarkBottomAnimation;
		lastSelectedMarkStartAnimation->setStartValue(10);
		lastSelectedMarkStartAnimation->setEndValue(0);
		lastSelectedMarkStartAnimation->start();
		lastSelectedMarkStopAnimation->stop();
		selectMarkAnimation->stop();
		m_isSelectMarkDisplay = false;
	}
}

void CALFooterDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	QStyleOptionViewItem viewOption(option);
	initStyleOption(&viewOption, index);
	const auto model = dynamic_cast<CALFooterModel*>(const_cast<QAbstractItemModel*>(index.model()));
	const auto node = index.data(Qt::UserRole).value<CALNavigationNode*>();
	if (option.state.testFlag(QStyle::State_HasFocus)) {
		viewOption.state &= ~QStyle::State_HasFocus;
	}
	QStyledItemDelegate::paint(painter, viewOption, index);
	// background
	QRect itemRect = option.rect;
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	itemRect.setTop(itemRect.top() + 2);
	itemRect.setBottom(itemRect.bottom() - 2);
	QPainterPath path;
	path.addRoundedRect(itemRect, 8, 8);
	if (option.state.testFlag(QStyle::State_Selected)) {
		if (index == m_pressIndex) {
			painter->fillPath(path, ALThemeColor(themeMode, ALThemeType::BasicHoverAlpha));
		} else {
			painter->fillPath(path, ALThemeColor(themeMode, option.state.testFlag(QStyle::State_MouseOver) ? ALThemeType::BasicSelectedHoverAlpha : ALThemeType::BasicSelectedAlpha));
		}
	} else {
		if (index == m_pressIndex) {
			painter->fillPath(path, ALThemeColor(themeMode, ALThemeType::BasicSelectedHoverAlpha));
		} else if (option.state.testFlag(QStyle::State_MouseOver)) {
			painter->fillPath(path, ALThemeColor(themeMode, ALThemeType::BasicHoverAlpha));
		}
	}
	painter->restore();

	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	itemRect = option.rect;

	// top
	if (index.row() == 0) {
		painter->setPen(ALThemeColor(themeMode, ALThemeType::BasicBaseLine));
		painter->drawLine(itemRect.x(), itemRect.y() + 1, itemRect.x() + itemRect.width(), itemRect.y() + 1);
	}

	// iocn
	painter->setPen(index == m_pressIndex ? ALThemeColor(themeMode, ALThemeType::BasicTextPress) : ALThemeColor(themeMode, ALThemeType::BasicText));
	if (node->getIconType() != ALIcon::None) {
		painter->save();
		QFont iconFont(ALIcon::getEnumTypeFontName(node->getIconType()));
		iconFont.setPixelSize(17);
		painter->setFont(iconFont);
		painter->drawText(itemRect.x(), itemRect.y(), m_iconAreaWidth, itemRect.height(), Qt::AlignCenter, node->property(ALIcon::iconProperty).toString());
		painter->restore();
	}

	// keyPoints
	if (int keyPoints = node->getKeyPoints()) {
		painter->save();
		painter->setPen(Qt::NoPen);
		painter->setBrush(Qt::white);
		painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 10, 10);
		painter->setBrush(ALThemeColor(themeMode, ALThemeType::StatusDanger));
		const bool isUpper99 = keyPoints > 99;
		keyPoints = qMin(99, keyPoints);
		const QString keyPointstext = isUpper99 ? "99+" : QString::number(keyPoints);
		painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 12, 12);
		painter->setPen(QPen(Qt::white, 2));
		QFont font = painter->font();
		font.setBold(true);
		font.setPixelSize(keyPoints > 9 ? 11 : 12);
		painter->setFont(font);
		painter->drawText(isUpper99 ? 245 : keyPoints > 9 ? 248 : 251, itemRect.y() + itemRect.height() / 2 + 4, keyPointstext);
		painter->restore();
	}

	// text
	painter->setPen(ALThemeColor(themeMode, index == m_pressIndex ? ALThemeType::BasicTextPress : ALThemeType::BasicText));
	const int padding = node->getIconType() != ALIcon::None ? m_iconAreaWidth : m_leftPadding;
	const QRect textRect{ itemRect.x() + padding, itemRect.y(), itemRect.width() - m_textRightSpacing - m_indicatorIconAreaWidth - padding, itemRect.height() };
	const QString text = painter->fontMetrics().elidedText(node->getNodeTitle(), Qt::ElideRight, textRect.width());
	painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
	// indicator
	painter->setPen(Qt::NoPen);
	painter->setBrush(ALThemeColor(themeMode, ALThemeType::PrimaryNormal));
	if (m_isSelectMarkDisplay && (node == model->getSelectedNode())) {
		painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + m_selectMarkTop, 3, itemRect.height() - m_selectMarkTop - m_selectMarkBottom), 3, 3);
	}
	if (node == m_lastSelectedNode) {
		painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + m_lastSelectMarkTop, 3, itemRect.height() - m_lastSelectMarkTop - m_lastSelectMarkBottom), 3, 3);
	}
	painter->restore();
}


QSize CALFooterDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(40);
	return size;
}

bool CALFooterDelegate::compareItemY(const CALNavigationNode* node1, const CALNavigationNode* node2) {
	if (!node1) {
		return true;
	}

	if (!node2) {
		return false;
	}

	return node1->getModelIndex().row() < node2->getModelIndex().row();
}
}

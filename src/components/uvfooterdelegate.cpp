#include "uvfooterdelegate.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "uvbaselistview.hpp"
#include "uvfootermodel.hpp"
#include "uvnavigationnode.hpp"
#include "uvtheme.hpp"

CUVFooterDelegate::CUVFooterDelegate(QObject* parent): QStyledItemDelegate(parent) {
	themeMode = UVTheme->getThemeMode();
	connect(UVTheme, &CUVThemeManager::sigThemeModeChanged, this, [=](const UVThemeType::ThemeMode& mode) { themeMode = mode; });
	setProperty("lastSelectMarkTop", 10.0);
	setProperty("lastSelectMarkBottom", 10.0);
	setProperty("selectMarkTop", 10.0);
	setProperty("selectMarkBottom", 10.0);
	// Mark向上
	m_lastSelectMarkTopAnimation = new QPropertyAnimation(this, "lastSelectMarkTop");
	m_lastSelectMarkTopAnimation->setDuration(300);
	m_lastSelectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
	connect(m_lastSelectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
		m_lastSelectMarkTop = value.toReal();
		m_listView->viewport()->update();
	});

	m_selectMarkBottomAnimation = new QPropertyAnimation(this, "selectMarkBottom");
	m_selectMarkBottomAnimation->setDuration(300);
	m_selectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
	connect(m_selectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
		m_selectMarkBottom = value.toReal();
		m_listView->viewport()->update();
	});
	connect(m_lastSelectMarkTopAnimation, &QPropertyAnimation::finished, this, [=]() {
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
	connect(m_lastSelectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
		m_lastSelectMarkBottom = value.toReal();
		m_listView->viewport()->update();
	});

	m_selectMarkTopAnimation = new QPropertyAnimation(this, "selectMarkTop");
	m_selectMarkTopAnimation->setDuration(300);
	m_selectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
	connect(m_selectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
		m_selectMarkTop = value.toReal();
		m_listView->viewport()->update();
	});
	connect(m_lastSelectMarkBottomAnimation, &QPropertyAnimation::finished, this, [=]() {
		m_isSelectMarkDisplay = true;
		m_lastSelectedNode = nullptr;
		m_selectMarkTopAnimation->setStartValue(0);
		m_selectMarkTopAnimation->setEndValue(10);
		m_selectMarkTopAnimation->start();
	});
}

CUVFooterDelegate::~CUVFooterDelegate() = default;

void CUVFooterDelegate::setCUVBaseListView(CUVBaseListView* listView) {
	m_listView = listView;
}

CUVBaseListView* CUVFooterDelegate::getCUVBaseListView() const {
	return m_listView;
}

void CUVFooterDelegate::setPressIndex(const QModelIndex& index) {
	m_pressIndex = index;
}

QModelIndex CUVFooterDelegate::getPressIndex() const {
	return m_pressIndex;
}

void CUVFooterDelegate::navigationNodeStateChange(const QVariantMap& data) {
	if (data.contains("SelectMarkChanged")) {
		m_lastSelectedNode = data.value("LastSelectedNode").value<CUVNavigationNode*>();
		const auto selectedNode = data.value("SelectedNode").value<CUVNavigationNode*>();
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

void CUVFooterDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	QStyleOptionViewItem viewOption(option);
	initStyleOption(&viewOption, index);
	const auto model = dynamic_cast<CUVFooterModel*>(const_cast<QAbstractItemModel*>(index.model()));
	const auto node = index.data(Qt::UserRole).value<CUVNavigationNode*>();
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
			painter->fillPath(path, UVThemeColor(themeMode, UVThemeType::BasicHoverAlpha));
		} else {
			painter->fillPath(path, UVThemeColor(themeMode, option.state.testFlag(QStyle::State_MouseOver) ? UVThemeType::BasicSelectedHoverAlpha : UVThemeType::BasicSelectedAlpha));
		}
	} else {
		if (index == m_pressIndex) {
			painter->fillPath(path, UVThemeColor(themeMode, UVThemeType::BasicSelectedHoverAlpha));
		} else if (option.state.testFlag(QStyle::State_MouseOver)) {
			painter->fillPath(path, UVThemeColor(themeMode, UVThemeType::BasicHoverAlpha));
		}
	}
	painter->restore();

	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	itemRect = option.rect;

	// top
	if (index.row() == 0) {
		painter->setPen(UVThemeColor(themeMode, UVThemeType::BasicBaseLine));
		painter->drawLine(itemRect.x(), itemRect.y() + 1, itemRect.x() + itemRect.width(), itemRect.y() + 1);
	}

	// ioco
	painter->setPen(index == m_pressIndex ? UVThemeColor(themeMode, UVThemeType::BasicTextPress) : UVThemeColor(themeMode, UVThemeType::BasicText));
	if (node->getAwesomeIcon() != UVIcon::CUVAweSomeIcon::None) {
		painter->save();
		QFont iconFont("CUVAwesome");
		iconFont.setPixelSize(17);
		painter->setFont(iconFont);
		painter->drawText(itemRect.x() + 10, itemRect.y() + 3, 20, 20, Qt::AlignCenter, QChar(static_cast<unsigned short>(node->getAwesomeIcon())));
		painter->restore();
	}

	// keyPoints
	if (int keyPoints = node->getKeyPoints()) {
		painter->save();
		painter->setPen(Qt::NoPen);
		painter->setBrush(Qt::white);
		painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 10, 10);
		painter->setBrush(UVThemeColor(themeMode, UVThemeType::StatusDanger));
		painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 9, 9);
		painter->setPen(QPen(Qt::white, 2));
		QFont font = painter->font();
		font.setBold(true);
		keyPoints = std::min(99, keyPoints);
		font.setPixelSize(keyPoints > 9 ? 11 : 12);
		painter->setFont(font);
		painter->drawText(keyPoints > 9 ? 248 : 251, itemRect.y() + itemRect.height() / 2 + 4, QString::number(keyPoints));
		painter->restore();
	}

	// text
	painter->setPen(UVThemeColor(themeMode, index == m_pressIndex ? UVThemeType::BasicTextPress : UVThemeType::BasicText));
	const int padding = node->getAwesomeIcon() != UVIcon::CUVAweSomeIcon::None ? m_iconAreaWidth : m_leftPadding;
	const QRect textRect{itemRect.x() + padding, itemRect.y(), itemRect.width() - m_textRightSpacing - m_indicatorIconAreaWidth - padding, itemRect.height()};
	const QString text = painter->fontMetrics().elidedText(node->getNodeTitle(), Qt::ElideRight, textRect.width());
	painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
	// indicator
	painter->setPen(Qt::NoPen);
		painter->setBrush(UVThemeColor(themeMode, UVThemeType::PrimaryNormal));
	if (m_isSelectMarkDisplay && (node == model->getSelectedNode())) {
		painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + m_selectMarkTop, 3, itemRect.height() - m_selectMarkTop - m_selectMarkBottom), 3, 3);
	}
	if (node == m_lastSelectedNode) {
		painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + m_lastSelectMarkTop, 3, itemRect.height() - m_lastSelectMarkTop - m_lastSelectMarkBottom), 3, 3);
	}
	painter->restore();
}


QSize CUVFooterDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(40);
	return size;
}

bool CUVFooterDelegate::compareItemY(const CUVNavigationNode* node1, const CUVNavigationNode* node2) {
	if (!node1) {
		return true;
	}

	if (!node2) {
		return false;
	}

	return node1->getModelIndex().row() < node2->getModelIndex().row();
}

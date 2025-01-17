#include "alnavigationstyle.hpp"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "alnavigationmodel.hpp"
#include "alnavigationnode.hpp"
#include "alnavigationview.hpp"
#include "althememanager.hpp"

/**
 * @brief \namespace AL
 */
namespace AL {
/**
 * @brief \class CALNavigationStyle
 * @param style pointer to the parent style
 */
CALNavigationStyle::CALNavigationStyle(QStyle* style) {
	m_opacity = 1.0;
	m_itemHeight = 40;
	m_lastSelectMarkTop = 10.0;
	m_lastSelectMarkBottom = 10.0;
	m_selectMarkTop = 10.0;
	m_selectMarkBottom = 10.0;

	/// mark 向上
	m_lastSelectedMarkTopAnimation = new QPropertyAnimation(this, "lastSelectMarkTop");
	connect(m_lastSelectedMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=]() { m_navigationView->viewport()->update(); });
	m_lastSelectedMarkTopAnimation->setDuration(300);
	m_lastSelectedMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);

	m_selectMarkBottomAnimation = new QPropertyAnimation(this, "selectMarkBottom");
	connect(m_selectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=]() { m_navigationView->viewport()->update(); });
	m_selectMarkBottomAnimation->setDuration(300);
	m_selectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
	connect(m_lastSelectedMarkTopAnimation, &QPropertyAnimation::finished, this, [=]() {
		m_isSelectedMarkDisplay = true;
		m_lastSelectedNode = nullptr;
		m_selectMarkBottomAnimation->setStartValue(0);
		m_selectMarkBottomAnimation->setEndValue(10);
		m_selectMarkBottomAnimation->start();
	});

	/// mark 向下
	m_lastSelectedMarkBottomAnimation = new QPropertyAnimation(this, "lastSelectMarkBottom");
	connect(m_lastSelectedMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=]() { m_navigationView->viewport()->update(); });
	m_lastSelectedMarkBottomAnimation->setDuration(300);
	m_lastSelectedMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);

	m_selectMarkTopAnimation = new QPropertyAnimation(this, "selectMarkTop");
	connect(m_selectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=]() { m_navigationView->viewport()->update(); });
	m_selectMarkTopAnimation->setDuration(300);
	m_selectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
	connect(m_lastSelectedMarkBottomAnimation, &QPropertyAnimation::finished, this, [=]() {
		m_isSelectedMarkDisplay = true;
		m_lastSelectedNode = nullptr;
		m_selectMarkTopAnimation->setStartValue(0);
		m_selectMarkTopAnimation->setEndValue(10);
		m_selectMarkTopAnimation->start();
	});

	m_themeMode = ALTheme->getThemeMode();
	connect(ALTheme, &CALThemeManager::sigThemeModeChanged, this, [=](const ALThemeType::ThemeMode& mode) { m_themeMode = mode; });
}

CALNavigationStyle::~CALNavigationStyle() = default;

void CALNavigationStyle::setOpacity(const qreal opacity) {
	m_opacity = opacity;
	Q_EMIT sigOpacityChanged();
}

qreal CALNavigationStyle::getOpacity() const {
	return m_opacity;
}

void CALNavigationStyle::setRotate(const qreal rotate) {
	m_rotate = rotate;
	Q_EMIT sigRotateChanged();
}

qreal CALNavigationStyle::getRotate() const {
	return m_rotate;
}

void CALNavigationStyle::setItemHeight(const int itemHeight) {
	m_itemHeight = itemHeight;
	Q_EMIT sigItemHeightChanged();
}

int CALNavigationStyle::getItemHeight() const {
	return m_itemHeight;
}

void CALNavigationStyle::setLastSelectMarkTop(const qreal lastSelectMarkTop) {
	m_lastSelectMarkTop = lastSelectMarkTop;
	Q_EMIT sigLastSelectMarkTopChanged();
}

qreal CALNavigationStyle::getLastSelectMarkTop() const {
	return m_lastSelectMarkTop;
}

void CALNavigationStyle::setLastSelectMarkBottom(const qreal lastSelectMarkBottom) {
	m_lastSelectMarkBottom = lastSelectMarkBottom;
	Q_EMIT sigLastSelectMarkBottomChanged();
}

qreal CALNavigationStyle::getLastSelectMarkBottom() const {
	return m_lastSelectMarkBottom;
}

void CALNavigationStyle::setSelectMarkTop(const qreal selectMarkTop) {
	m_selectMarkTop = selectMarkTop;
	Q_EMIT sigSelectMarkTopChanged();
}

qreal CALNavigationStyle::getSelectMarkTop() const {
	return m_selectMarkTop;
}

void CALNavigationStyle::setSelectMarkBottom(const qreal selectMarkBottom) {
	m_selectMarkBottom = selectMarkBottom;
	Q_EMIT sigSelectMarkBottomChanged();
}

qreal CALNavigationStyle::getSelectMarkBottom() const {
	return m_selectMarkBottom;
}

void CALNavigationStyle::setNavigationView(CALNavigationView* navigationView) {
	m_navigationView = navigationView;
}

CALNavigationView* CALNavigationStyle::getNavigationView() const {
	return m_navigationView;
}

void CALNavigationStyle::setPressIndex(const QModelIndex& index) {
	m_pressIndex = index;
}

QModelIndex CALNavigationStyle::getPressIndex() const {
	return m_pressIndex;
}

void CALNavigationStyle::drawPrimitive(const PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (pe) {
		case QStyle::PE_PanelItemViewItem: {
			// Item 背景
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt)) {
				p->save();
				const QModelIndex index = vopt->index;
				if (const auto node = static_cast<CALNavigationNode*>(index.internalPointer()); this->m_opacityAnimationTargetNode && node->getParentNode() == this->m_opacityAnimationTargetNode) {
					p->setOpacity(m_opacity);
				}
				p->setRenderHint(QPainter::Antialiasing);
				QRect itemRect = vopt->rect;
				itemRect.setTop(itemRect.top() + 2);
				itemRect.setBottom(itemRect.bottom() - 2);
				QPainterPath path;
				path.addRoundedRect(itemRect, 8, 8);
				if (vopt->state.testFlag(QStyle::State_Selected)) {
					if (index == m_pressIndex) { // 选中时点击
						p->fillPath(path, ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
					} else {
						p->fillPath(path, ALThemeColor(m_themeMode, vopt->state.testFlag(QStyle::State_MouseOver) ? ALThemeType::BasicSelectedHoverAlpha : ALThemeType::BasicSelectedAlpha));
					}
				} else {
					if (index == m_pressIndex) { // 未选中时点击
						p->fillPath(path, ALThemeColor(m_themeMode, ALThemeType::BasicSelectedHoverAlpha));
					} else {
						if (vopt->state.testFlag(QStyle::State_MouseOver)) { // 覆盖时颜色
							p->fillPath(path, ALThemeColor(m_themeMode, ALThemeType::BasicHoverAlpha));
						}
					}
				}
				p->restore();
			}
			return;
		}
		case QStyle::PE_PanelItemViewRow:  // 行背景
		case QStyle::PE_IndicatorBranch: { // Branch 指示器
			return;
		}
		default: break;
	}

	QProxyStyle::drawPrimitive(pe, opt, p, w);
}

void CALNavigationStyle::drawControl(const ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const {
	switch (element) {
		case QStyle::CE_ShapedFrame: { // viewport 视口外的其他区域背景
			return;
		}
		case QStyle::CE_ItemViewItem: {
			if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt)) {
				// background
				this->drawPrimitive(QStyle::PE_PanelItemViewItem, opt, p, w);
				// content
				const QRect itemRect = opt->rect;
				p->save();
				p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
				const auto node = static_cast<CALNavigationNode*>(vopt->index.internalPointer());
				const auto model = dynamic_cast<CALNavigationModel*>(const_cast<QAbstractItemModel*>(vopt->index.model()));
				if (this->m_opacityAnimationTargetNode && node->getParentNode() == m_opacityAnimationTargetNode) {
					p->setOpacity(m_opacity);
				}

				// 选中特效
				if (m_isSelectedMarkDisplay && (node == model->getSelectedNode() || node == model->getSelectedExpandedNode())) {
					p->setPen(Qt::NoPen);
					p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
					p->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + m_selectMarkTop, 3, itemRect.height() - m_selectMarkTop - m_selectMarkBottom), 3, 3);
				}

				if (node == m_lastSelectedNode) {
					p->setPen(Qt::NoPen);
					p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
					p->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + m_lastSelectMarkTop, 3, itemRect.height() - m_lastSelectMarkTop - m_lastSelectMarkBottom), 3, 3);
				}

				// 图标绘制
				p->setPen(ALThemeColor(m_themeMode, vopt->index == m_pressIndex ? ALThemeType::BasicTextPress : ALThemeType::BasicText));
				if (node->getIconType() != ALIcon::None) {
					p->save();
					QFont iconFont(ALIcon::getEnumTypeFontName(node->getIconType()));
					iconFont.setPixelSize(17);
					p->setFont(iconFont);
					p->drawText(QRect(itemRect.x(), itemRect.y(), m_iconAreaWidth, itemRect.height()), Qt::AlignCenter, node->property("CALIcon").toString());
					p->restore();
				}

				const int viewWidth = w->width();
				// 文字绘制
				p->setPen(vopt->index == m_pressIndex ? ALThemeColor(m_themeMode, ALThemeType::BasicTextPress) : ALThemeColor(m_themeMode, ALThemeType::BasicText));
				const int leftPadding = node->getIconType() != ALIcon::None ? m_iconAreaWidth : m_leftPadding;
				const auto textRect = QRect(itemRect.x() + leftPadding, itemRect.y(), itemRect.width() - m_textRightSapcing - m_indicatorIconAreaWidth - leftPadding, itemRect.height());
				const QString text = p->fontMetrics().elidedText(node->getNodeTitle(), Qt::ElideRight, textRect.width());
				p->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);

				if (viewWidth > 260) {
					// 展开图标 & keyPoints
					if (node->getIsExpanderNode()) {
						if (node->getIsHasChild()) {
							p->save();
							const QRectF expandIconRect(itemRect.right() - m_indicatorIconAreaWidth, itemRect.y(), 17, itemRect.height());
							QFont iconFont(ALIcon::getEnumTypeFontName(ALIcon::Awesome));
							iconFont.setPixelSize(17);
							p->setFont(iconFont);
							p->translate(expandIconRect.x() + expandIconRect.width() / 2.0, expandIconRect.y() + expandIconRect.height() / 2.0);
							p->rotate(node == m_expandAnimationTargetNode ? m_rotate : node->getIsExpanded() ? -180 : 0);
							p->translate(-expandIconRect.x() - expandIconRect.width() / 2.0 + 1, -expandIconRect.y() - expandIconRect.height() / 2.0);
							p->drawText(expandIconRect, Qt::AlignVCenter, QChar(static_cast<unsigned short>(ALIcon::AweSomeIcon::AngleDown)));
							p->restore();
						}

						if (node->getIsChildHasKeyPoints()) {
							p->setPen(Qt::NoPen);
							p->setBrush(ALThemeColor(m_themeMode, ALThemeType::PrimaryNormal));
							p->drawEllipse(QPoint(itemRect.right() - 17, itemRect.y() + 12), 3, 3);
						}
					} else {
						if (int keyPoints = node->getKeyPoints()) {
							// keyPoints
							p->setPen(Qt::NoPen);
							p->setBrush(Qt::white);
							p->drawEllipse(QPoint(itemRect.right() - 26, itemRect.y() + itemRect.height() / 2), 10, 10);
							p->setBrush(ALThemeColor(m_themeMode, ALThemeType::StatusDanger));
							const bool isUpper99 = keyPoints > 99;
							keyPoints = qMin(99, keyPoints);
							const QString keyPointstext = isUpper99 ? "99+" : QString::number(keyPoints);
							const int textWidth = p->fontMetrics().horizontalAdvance(keyPointstext);
							p->drawEllipse(QPoint(itemRect.right() - 26, itemRect.y() + itemRect.height() / 2), 12, 12);
							p->setPen(QPen(Qt::white, 2));
							QFont font = p->font();
							font.setBold(true);
							keyPoints = qMin(99, keyPoints);
							font.setPixelSize(keyPoints > 9 ? 11 : 12);
							p->setFont(font);
							const int textX = itemRect.right() - (isUpper99 ? 25 : 26) - textWidth / 2;
							p->drawText(textX, itemRect.y() + itemRect.height() / 2 + 4, keyPointstext);
						}
					}
				}
				p->restore();
			}
			return;
		}
		default: break;
	}

	QProxyStyle::drawControl(element, opt, p, w);
}

QSize CALNavigationStyle::sizeFromContents(const ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const {
	switch (ct) {
		case QStyle::CT_ItemViewItem: {
			QSize itemSize = QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
			itemSize.setHeight(m_itemHeight);
			return itemSize;
		}
		default: break;
	}

	return QProxyStyle::sizeFromContents(ct, opt, contentsSize, w);
}

void CALNavigationStyle::navigationNodeStateChanged(const QVariantMap& data) {
	if (data.contains("Expand")) {
		const CALNavigationNode* lastExpandNode = m_expandAnimationTargetNode;
		m_opacityAnimationTargetNode = data.value("Expand").value<CALNavigationNode*>();
		m_expandAnimationTargetNode = m_opacityAnimationTargetNode;

		const auto nodeOpacityAnimation = new QPropertyAnimation(this, "opacity");
		connect(nodeOpacityAnimation, &QPropertyAnimation::finished, this, [=]() { m_opacityAnimationTargetNode = nullptr; });
		connect(nodeOpacityAnimation, &QPropertyAnimation::valueChanged, this, [=]() { m_navigationView->viewport()->update(); });
		nodeOpacityAnimation->setDuration(600);
		nodeOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
		nodeOpacityAnimation->setKeyValueAt(0.4, 0);
		nodeOpacityAnimation->setStartValue(0);
		nodeOpacityAnimation->setEndValue(1);
		nodeOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

		const auto rotateAnimation = new QPropertyAnimation(this, "rotate");
		connect(rotateAnimation, &QPropertyAnimation::finished, this, [=]() { m_expandAnimationTargetNode = nullptr; });
		connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=]() { m_navigationView->viewport()->update(); });
		rotateAnimation->setDuration(300);
		rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
		rotateAnimation->setStartValue(lastExpandNode == m_expandAnimationTargetNode ? m_rotate : 0);
		rotateAnimation->setEndValue(-180);
		rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	} else if (data.contains("Collapse")) {
		const CALNavigationNode* lastExpandNode = m_expandAnimationTargetNode;
		m_opacityAnimationTargetNode = data.value("Collapse").value<CALNavigationNode*>();
		m_expandAnimationTargetNode = m_opacityAnimationTargetNode;
		m_opacity = 0;

		const auto rotateAnimation = new QPropertyAnimation(this, "rotate");
		connect(rotateAnimation, &QPropertyAnimation::finished, this, [=]() {
			m_expandAnimationTargetNode = nullptr;
			m_opacity = -1;
		});
		connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=]() { m_navigationView->viewport()->update(); });
		rotateAnimation->setDuration(300);
		rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
		rotateAnimation->setStartValue(lastExpandNode == m_expandAnimationTargetNode ? m_rotate : -180);
		rotateAnimation->setEndValue(0);
		rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	} else if (data.contains("SelectMarkChanged")) {
		m_lastSelectedNode = data.value("LastSelectedNode").value<CALNavigationNode*>();
		const auto selectedNode = data.value("SelectedNode").value<CALNavigationNode*>();
		const bool direction = compareItemY(selectedNode, m_lastSelectedNode);
		m_lastSelectMarkTop = 10;
		m_lastSelectMarkBottom = 10;
		m_selectMarkTop = 10;
		m_selectMarkBottom = 10;
		const auto& lastSelectMarkStartAnimation = direction ? m_lastSelectedMarkTopAnimation : m_lastSelectedMarkBottomAnimation;
		const auto& lastSelectMarkStopAnimation = direction ? m_lastSelectedMarkBottomAnimation : m_lastSelectedMarkTopAnimation;
		const auto& selectMarkAnimation = direction ? m_selectMarkTopAnimation : m_selectMarkBottomAnimation;
		lastSelectMarkStartAnimation->setStartValue(10);
		lastSelectMarkStartAnimation->setEndValue(0);
		lastSelectMarkStartAnimation->start();
		lastSelectMarkStopAnimation->stop();
		selectMarkAnimation->stop();
		m_isSelectedMarkDisplay = false;
	}
}

bool CALNavigationStyle::compareItemY(CALNavigationNode* node1, CALNavigationNode* node2) {
	// 返回 true 即 node1 高于 node2
	if (!node1) {
		return false;
	}

	if (!node2) {
		return true;
	}

	// 同一个父节点
	if (node1->getParentNode() == node2->getParentNode()) {
		return node1->getModelIndex().row() < node2->getModelIndex().row();
	} else {
		const CALNavigationNode* node1OriginalNode = node1->getOriginalNode();
		// 不同父节点，相同起源点
		if (const CALNavigationNode* node2OriginalNode = node2->getOriginalNode(); node1OriginalNode == node2OriginalNode) {
			const int node1Depth = node1->getDepth();
			const int node2Depth = node2->getDepth();
			const CALNavigationNode* node1ParentNode = node1->getParentNode();
			const CALNavigationNode* node2ParentNode = node2->getParentNode();
			// 相同深度
			if (node1Depth == node2Depth) {
				return node1ParentNode->getModelIndex().row() < node2ParentNode->getModelIndex().row();
			} else if (node1Depth < node2Depth) {
				while (node2ParentNode->getDepth() != node1Depth) {
					node2ParentNode = node2ParentNode->getParentNode();
				}
				// 父子节点关系
				if (node1 == node2ParentNode) {
					return true;
				}
				return node1->getModelIndex().row() < node2ParentNode->getModelIndex().row();
			} else {
				while (node1ParentNode->getDepth() != node2Depth) {
					node1ParentNode = node1ParentNode->getParentNode();
				}
				// 父子节点关系
				if (node2 == node1ParentNode) {
					return false;
				}
				return node2->getModelIndex().row() > node1ParentNode->getModelIndex().row();
			}
		} else {
			return node1OriginalNode->getModelIndex().row() < node2OriginalNode->getModelIndex().row();
		}
	}
}
}

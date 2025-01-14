#pragma once

#include <QProxyStyle>

#include "alnavigationnode.hpp"
#include "alwidgettoolsdef.hpp"

class QPropertyAnimation;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALNavigationView;
class CALNavigationNode;

class CALNavigationStyle final : public QProxyStyle {
	Q_OBJECT
	Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity NOTIFY sigOpacityChanged)
	Q_PROPERTY(qreal rotate READ getRotate WRITE setRotate NOTIFY sigRotateChanged)
	Q_PROPERTY(int itemHeight READ getItemHeight WRITE setItemHeight NOTIFY sigItemHeightChanged)
	Q_PROPERTY(qreal lastSelectMarkTop READ getLastSelectMarkTop WRITE setLastSelectMarkTop NOTIFY sigLastSelectMarkTopChanged)
	Q_PROPERTY(qreal lastSelectMarkBottom READ getLastSelectMarkBottom WRITE setLastSelectMarkBottom NOTIFY sigLastSelectMarkBottomChanged)
	Q_PROPERTY(qreal selectMarkTop READ getSelectMarkTop WRITE setSelectMarkTop NOTIFY sigSelectMarkTopChanged)
	Q_PROPERTY(qreal selectMarkBottom READ getSelectMarkBottom WRITE setSelectMarkBottom NOTIFY sigSelectMarkBottomChanged)

public:
	explicit CALNavigationStyle(QStyle* style = nullptr);
	~CALNavigationStyle() override;

	void setOpacity(qreal opacity);
	[[nodiscard]] qreal getOpacity() const;

	void setRotate(qreal rotate);
	[[nodiscard]] qreal getRotate() const;

	void setItemHeight(int itemHeight);
	[[nodiscard]] int getItemHeight() const;

	void setLastSelectMarkTop(qreal lastSelectMarkTop);
	[[nodiscard]] qreal getLastSelectMarkTop() const;

	void setLastSelectMarkBottom(qreal lastSelectMarkBottom);
	[[nodiscard]] qreal getLastSelectMarkBottom() const;

	void setSelectMarkTop(qreal selectMarkTop);
	[[nodiscard]] qreal getSelectMarkTop() const;

	void setSelectMarkBottom(qreal selectMarkBottom);
	[[nodiscard]] qreal getSelectMarkBottom() const;

	void setNavigationView(CALNavigationView* navigationView);
	[[nodiscard]] CALNavigationView* getNavigationView() const;

	void setPressIndex(const QModelIndex& index);
	[[nodiscard]] QModelIndex getPressIndex() const;

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
	QSize sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* w) const override;

	void navigationNodeStateChanged(const QVariantMap& data);

Q_SIGNALS:
	Q_SIGNAL void sigOpacityChanged();
	Q_SIGNAL void sigRotateChanged();
	Q_SIGNAL void sigItemHeightChanged();
	Q_SIGNAL void sigLastSelectMarkTopChanged();
	Q_SIGNAL void sigLastSelectMarkBottomChanged();
	Q_SIGNAL void sigSelectMarkTopChanged();
	Q_SIGNAL void sigSelectMarkBottomChanged();

private:
	qreal m_opacity{};
	qreal m_rotate{};
	qreal m_lastSelectMarkTop{};
	qreal m_lastSelectMarkBottom{};
	qreal m_selectMarkTop{};
	qreal m_selectMarkBottom{};
	int m_itemHeight{};
	int m_leftPadding{ 11 };
	int m_iconAreaWidth{ 40 };
	int m_textRightSapcing{ 3 };
	int m_indicatorIconAreaWidth{ 34 };
	bool m_isSelectedMarkDisplay{ true };
	ALThemeType::ThemeMode m_themeMode{};
	QModelIndex m_pressIndex{};
	CALNavigationView* m_navigationView{ nullptr };
	CALNavigationNode* m_opacityAnimationTargetNode{ nullptr };
	CALNavigationNode* m_expandAnimationTargetNode{ nullptr };
	CALNavigationNode* m_lastSelectedNode{ nullptr };

	QPropertyAnimation* m_lastSelectedMarkTopAnimation{ nullptr };
	QPropertyAnimation* m_lastSelectedMarkBottomAnimation{ nullptr };
	QPropertyAnimation* m_selectMarkTopAnimation{ nullptr };
	QPropertyAnimation* m_selectMarkBottomAnimation{ nullptr };

	static bool compareItemY(CALNavigationNode* node1, CALNavigationNode* node2);
};
}

#pragma once

#include <QStyledItemDelegate>

#include "alwidgettoolsdef.hpp"

class QPropertyAnimation;

/**
 * @brief \namespace AL
 */
namespace AL {
class CALBaseListView;
class CALNavigationNode;

class CALFooterDelegate final : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit CALFooterDelegate(QObject* parent = nullptr);
	~CALFooterDelegate() override;

	void setCALBaseListView(CALBaseListView* listView);
	[[nodiscard]] CALBaseListView* getCALBaseListView() const;

	void setPressIndex(const QModelIndex& index);
	[[nodiscard]] QModelIndex getPressIndex() const;

	void navigationNodeStateChange(const QVariantMap& data);

protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	[[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	CALBaseListView* m_listView{ nullptr };
	CALNavigationNode* m_lastSelectedNode{ nullptr };
	QPropertyAnimation* m_lastSelectMarkTopAnimation{ nullptr };
	QPropertyAnimation* m_lastSelectMarkBottomAnimation{ nullptr };
	QPropertyAnimation* m_selectMarkTopAnimation{ nullptr };
	QPropertyAnimation* m_selectMarkBottomAnimation{ nullptr };
	QModelIndex m_pressIndex{};

	ALThemeType::ThemeMode themeMode{};
	int m_leftPadding{ 11 };
	int m_iconAreaWidth{ 40 };
	int m_textRightSpacing{ 3 };
	int m_indicatorIconAreaWidth{ 24 };
	qreal m_lastSelectMarkTop{ 10 };
	qreal m_lastSelectMarkBottom{ 10 };
	qreal m_selectMarkTop{ 10 };
	qreal m_selectMarkBottom{ 10 };
	bool m_isSelectMarkDisplay{ true };

	static bool compareItemY(const CALNavigationNode* node1, const CALNavigationNode* node2);
};
}

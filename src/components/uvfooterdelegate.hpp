#pragma once

#include <QStyledItemDelegate>

#include "uvwidgettoolsdef.hpp"

class QPropertyAnimation;

class CUVBaseListView;
class CUVNavigationNode;

class CUVFooterDelegate final : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit CUVFooterDelegate(QObject* parent = nullptr);
	~CUVFooterDelegate() override;

	void setCUVBaseListView(CUVBaseListView* listView);
	[[nodiscard]] CUVBaseListView* getCUVBaseListView() const;

	void setPressIndex(const QModelIndex& index);
	[[nodiscard]] QModelIndex getPressIndex() const;

	void navigationNodeStateChange(const QVariantMap& data);

protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	[[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	CUVBaseListView* m_listView{ nullptr };
	CUVNavigationNode* m_lastSelectedNode{ nullptr };
	QPropertyAnimation* m_lastSelectMarkTopAnimation{ nullptr };
	QPropertyAnimation* m_lastSelectMarkBottomAnimation{ nullptr };
	QPropertyAnimation* m_selectMarkTopAnimation{ nullptr };
	QPropertyAnimation* m_selectMarkBottomAnimation{ nullptr };
	QModelIndex m_pressIndex{};

	UVThemeType::ThemeMode themeMode{};
	int m_leftPadding{ 11 };
	int m_iconAreaWidth{ 40 };
	int m_textRightSpacing{ 3 };
	int m_indicatorIconAreaWidth{ 24 };
	qreal m_lastSelectMarkTop{ 10 };
	qreal m_lastSelectMarkBottom{ 10 };
	qreal m_selectMarkTop{ 10 };
	qreal m_selectMarkBottom{ 10 };
	bool m_isSelectMarkDisplay{ true };

	static bool compareItemY(const CUVNavigationNode* node1, const CUVNavigationNode* node2);
};

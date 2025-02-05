#pragma once

#include <QListView>

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPovitStyle;
}

/**
 * @brief \namespace AL
 */
namespace AL {
class CALPovitView final : public QListView {
	Q_OBJECT
	Q_PROPERTY(int markX READ getMarkX WRITE setMarkX NOTIFY sigMarkXChanged)
	Q_PROPERTY(int markAnimationWidth READ getMarkAnimationWidth WRITE setMarkAnimationWidth NOTIFY sigMarkAnimationWidthChanged)

public:
	explicit CALPovitView(QWidget* parent = nullptr);
	~CALPovitView() override;

	void doCurrentIndexChangedAnimation(const QModelIndex& index);

	void setMarkX(int x);
	[[nodiscard]] int getMarkX() const;

	void setMarkAnimationWidth(int width);
	[[nodiscard]] int getMarkAnimationWidth() const;

	void setMarkWidth(int width);
	[[nodiscard]] int getMarkWidth() const;

	void setPovitStyle(CALPovitStyle* style);
	[[nodiscard]] CALPovitStyle* getPovitStyle() const;

Q_SIGNALS:
	 void sigMarkXChanged();
	 void sigMarkAnimationWidthChanged();

protected:
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

private:
	int m_markX{};
	int m_markAnimationWidth{};
	int m_markWidth{};

	CALPovitStyle* m_style{ nullptr };
};
}

#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

namespace AL {
class CALSuggestBoxPrivate;

class CALWIDGETTOOLS_EXPORT CALSuggestBox : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CALSuggestBox)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(Qt::CaseSensitivity caseSensitivity READ getCaseSensitivity WRITE setCaseSensitivity NOTIFY sigCaseSensitivityChanged)

public:
	explicit CALSuggestBox(QWidget* parent = nullptr);
	~CALSuggestBox() override;

	void setBorderRadius(int borderRadius);
	[[nodiscard]] int getBorderRadius() const;

	void setCaseSensitivity(const Qt::CaseSensitivity& caseSensitivity);
	[[nodiscard]] Qt::CaseSensitivity getCaseSensitivity() const;

	void setPlaceholderText(const QString& placaHolderText);

	void addSuggestion(const QString& suggestText, const QVariantMap& suggestData = {});
	void addSuggestion(const ALIcon::AweSomeIcon& awesomeIcon, const QString& suggestText, const QVariantMap& suggestData = {});
	void removeSuggestion(const QString& suggestText);
	void removeSuggestion(int index);

Q_SIGNALS:
	Q_SIGNAL void sigBorderRadiusChanged();
	Q_SIGNAL void sigCaseSensitivityChanged();
	Q_SIGNAL void sigSuggestionClicked(const QString& suggestText, const QVariantMap& suggestData);

protected:
	const QScopedPointer<CALSuggestBoxPrivate> d_ptr{ nullptr };
};
} // namespace AL
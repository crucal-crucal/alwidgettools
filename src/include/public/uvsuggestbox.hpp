#pragma once

#include <QWidget>

#include "alwidgettoolsdef.hpp"

class CUVSuggestBoxPrivate;

class CALWIDGETTOOLS_EXPORT CUVSuggestBox : public QWidget {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CUVSuggestBox)
	Q_PROPERTY(int borderRadius READ getBorderRadius WRITE setBorderRadius NOTIFY sigBorderRadiusChanged)
	Q_PROPERTY(Qt::CaseSensitivity caseSensitivity READ getCaseSensitivity WRITE setCaseSensitivity NOTIFY sigCaseSensitivityChanged)

public:
	explicit CUVSuggestBox(QWidget* parent = nullptr);
	~CUVSuggestBox() override;

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
	const QScopedPointer<CUVSuggestBoxPrivate> d_ptr{ nullptr };
};

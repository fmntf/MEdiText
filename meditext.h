#ifndef MEDITEXT_H
#define MEDITEXT_H

#include <QSettings>
#include "ui_meditext.h"
#include "document_manager.h"

class MEdiText : public QWidget, private Ui::MEdiTextDLG, public DocumentManager
{
	Q_OBJECT

	public:
		MEdiText();
		MEdiText(char* filename);

	private:
		void init();
		void loadFromFilename(QString filename);
		QString currentDocument;
		QSettings* settings;
		bool changesCanBeSaved();

	public slots:
		virtual void createNewDocument();
		virtual void openExistingDocument();
		virtual bool saveDocument();
		
	private slots:
		void openPreferences();
		void setBackground(QString path, int alpha);
		void setFont(QString family, int size);
		void closeIfSaved();
};

#endif

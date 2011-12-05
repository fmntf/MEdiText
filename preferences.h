#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QSettings>
#include "ui_preferences.h"

class Preferences : public QDialog, private Ui::MEdiTextPREFS
{
	Q_OBJECT

	public:
		Preferences(QSettings* preferences);

	private:
		QSettings* settings;

	public slots:
		void update();
		void restore();
	
	private slots:
		void updateTransparency(int alpha);
		void pickImage();
		void updateFont(QString font);
		void updateFont(int size);

	signals:
		void backgroundChanged(QString path, int alpha);
		void fontChanged(QString family, int size);
};

#endif

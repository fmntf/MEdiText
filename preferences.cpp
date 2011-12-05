#include <QtGui>
#include "meditext.h"
#include "preferences.h"

/**
 * Creates a Preferences window reading saved settings.
 * 
 * @param settings
 */
Preferences::Preferences(QSettings* settings)
{
	setupUi(this);
	this->settings = settings;

	connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( restore() ) );
	connect( saveBtn, SIGNAL( clicked() ), this, SLOT( update() ) );
	connect( browseBtn, SIGNAL( clicked() ), this, SLOT( pickImage() ) );
	connect( transparency, SIGNAL( valueChanged(int) ), this, SLOT( updateTransparency(int) ) );
	connect( fontList, SIGNAL( currentIndexChanged(QString) ), this, SLOT( updateFont(QString) ) );
	connect( fontSize, SIGNAL( valueChanged(int) ), this, SLOT( updateFont(int) ) );


	// restore image settings
	QString background = this->settings->value("background_path", "mckenzie.jpg").toString();
	int alpha = this->settings->value("background_alpha", 128).toInt();

	this->backgroundPath->setText(background);

	this->transparency->setMinimum(0);
	this->transparency->setMaximum(255);
	this->transparency->setValue(alpha);

	// restore font settings
	QString family = this->settings->value("font_family", "DejaVu Sans").toString();
	int size = this->settings->value("font_size", 11).toInt();

	QFontDatabase fdb;
	this->fontList->addItems(fdb.families());
	int fontToSelect = this->fontList->findText(family, Qt::MatchExactly);
	this->fontList->setCurrentIndex(fontToSelect);

	this->fontSize->setMinimum(3);
	this->fontSize->setMaximum(25);
	this->fontSize->setValue(size);
}

/**
 * Persists the (new) settings.
 */
void Preferences::update()
{
	this->settings->setValue("background_alpha", this->transparency->value());
	this->settings->setValue("background_path", this->backgroundPath->text());

	this->settings->setValue("font_family", this->fontList->currentText());
	this->settings->setValue("font_size", this->fontSize->value());

	this->close();
}

/**
 * Discards every change made to the settings.
 */
void Preferences::restore()
{
	QString path = this->settings->value("background_path", "mckenzie.jpg").toString();
	int alpha = this->settings->value("background_alpha", 128).toInt();
	emit backgroundChanged(path, alpha);

	QString family = this->settings->value("font_family", "DejaVu Sans").toString();
	int size = this->settings->value("font_size", 11).toInt();
	emit fontChanged(family, size);

	this->close();
}

/**
 * Opens a window to select a new background image.
 * Emits backgroundChanged.
 */
void Preferences::pickImage()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open file",
				QString(), "Images (*.jpg);;All files (*)");
	if (!fileName.isEmpty()) {
		this->backgroundPath->setText(fileName);
		int alpha = this->transparency->value();
		emit backgroundChanged(fileName, alpha);		
	}
}

/**
 * Callback for the transparency slider
 * Emits backgroundChanged.
 * 
 * @param alpha
 */
void Preferences::updateTransparency(int alpha)
{
	QString path = this->backgroundPath->text();
	emit backgroundChanged(path, alpha);
}

/**
 * Callback for the font combo.
 * Emits fontChanged.
 * 
 * @param font
 */
void Preferences::updateFont(QString font)
{
	int size = this->fontSize->value();
	emit fontChanged(font, size);
}

/**
 * Callback font the font size combo.
 * Emits fontChanged.
 * 
 * @param size
 */
void Preferences::updateFont(int size)
{
	QString font = this->fontList->currentText();
	emit fontChanged(font, size);
}

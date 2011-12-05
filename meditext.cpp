#include <QtGui>
#include <QFile>
#include "background.cpp"
#include "preferences.cpp"
#include "meditext.h"


// construction

/**
 * Starts MEdiText with a blank document.
 */
MEdiText::MEdiText()
{
	init();
	this->currentDocument = QString();
}

/**
 * Starts MEdiText preloading a file.
 * 
 * @param filename Path to the text file to preload (by argv)
 */
MEdiText::MEdiText(char* filename)
{
	init();
	this->loadFromFilename(QString(filename));
}

/**
 * Setups the UI: textarea transparency, preferences restore, signals&slots.
 */
void MEdiText::init()
{
	// ui
	setupUi(this);
	showFullScreen();
	this->textEdit->viewport()->setAutoFillBackground(false);

	// signal listening
	connect( newBtn, SIGNAL( clicked() ), this, SLOT( createNewDocument() ) );
	connect( openBtn, SIGNAL( clicked() ), this, SLOT( openExistingDocument() ) );
	connect( saveBtn, SIGNAL( clicked() ), this, SLOT( saveDocument() ) );
	connect( quitBtn, SIGNAL( clicked() ), this, SLOT( closeIfSaved() ) );
	connect( prefsBtn, SIGNAL( clicked() ), this, SLOT( openPreferences() ) );

	// preference restore
	this->settings = new QSettings("MEdiText", "MEdiText");
	QString path = this->settings->value("background_path", "mckenzie.jpg").toString();
	int alpha = this->settings->value("background_alpha", 128).toInt();
	this->setBackground(path, alpha);

	QString family = this->settings->value("font_family", "DejaVu Sans").toString();
	int size = this->settings->value("font_size", 11).toInt();
	this->setFont(family, size);
}


// text and ui handling

/**
 * Empties the textarea to allow the user to write a new document.
 * If there are some unsaved changes, a prompt is showed to allow
 * the user to save the file.
 */
void MEdiText::createNewDocument()
{
	if (this->changesCanBeSaved()) {
		this->textEdit->clear();
		this->currentDocument = QString();
	}
}

/**
 * Open a prompt to allow the user to select a document to load.
 * If there are some unsaved changes, a prompt is showed to allow
 * the user to save the file.
 */
void MEdiText::openExistingDocument()
{
	if (this->changesCanBeSaved()) {
		QString fileName = QFileDialog::getOpenFileName(this, "Open file",
					QString(), "Text files (*.txt);;All files (*)");
		if (!fileName.isEmpty()) {
			loadFromFilename(fileName);
		}
	}
}

/**
 * Saves the document to disk.
 * If there is a document previously saved, overwrites it.
 * It the document is totally new, opens a dialog to let the user to choose
 * a file name.
 * 
 * @return false in case of failure
 */
bool MEdiText::saveDocument()
{
	QString fileName;

	if (this->currentDocument.isEmpty()) {

		fileName = QFileDialog::getSaveFileName(this, "Save as",
				QString(), "Text files (*.txt);;All files (*)");

		if (fileName.isEmpty()) {
			return false;
		}       

	} else {
		fileName = this->currentDocument;
	}

	QTextDocumentWriter writer(fileName);
	bool hasSaved = writer.write(textEdit->document());
    	if (hasSaved) {
		this->currentDocument = fileName;
		textEdit->document()->setModified(false);
		return true;
	} else {
		QMessageBox::warning(this, "Cannot save",
			"It is not possibile to save your document.",
			QMessageBox::Ok);

		return false;
	}		
}

/**
 * Checks if there are unsaved changes.
 * If yes, gives to the user the ability to save them.
 * 
 * @return true if
 *  - there are no changes
 *  - the changes can be saved correctly
 *  - the user want to discard them
 */
bool MEdiText::changesCanBeSaved()
{
	if (textEdit->document()->isModified()) {

		int whatToDo = QMessageBox::question(this, "Unsaved changes",
				"Would you like to save your changes before to continue?",
				QMessageBox::Yes, QMessageBox::No|QMessageBox::Escape);

		if (whatToDo == QMessageBox::Yes) {
			bool saved = this->saveDocument();
			if (!saved) {
				return false;
			}
		}
	}

	return true;
}

/**
 * Reads filename from the I/O and puts its content in the textarea.
 * Updates also the reference to the opened file to allow the user to
 * replace directly filename when saving.
 * 
 * @param filename Path to the text
 */
void MEdiText::loadFromFilename(QString filename)
{
	QFile file(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	
	if (file.exists()) {
		QTextStream out(&file);
		QString output = out.readAll();
		file.close();
				
		textEdit->setPlainText(output);
		this->currentDocument = filename;
	} else {
        this->currentDocument = QString();
    }
}

/**
 * Closes the window, after checking that there are no unsaved changes.
 */
void MEdiText::closeIfSaved()
{
	if (this->changesCanBeSaved()) {
		this->close();
	}
}


// preferences

/**
 * Open the preferences window.
 * Listens backgroundChanged and fontChanged events to repaint this window.
 */
void MEdiText::openPreferences()
{
	Preferences *dialog = new Preferences(this->settings);
	dialog->show();

	connect( dialog, SIGNAL( backgroundChanged(QString, int) ), this, SLOT( setBackground(QString, int) ) );
	connect( dialog, SIGNAL( fontChanged(QString, int) ), this, SLOT( setFont(QString, int) ) );
}

/**
 * Changes the window background.
 * 
 * @param path image file
 * @param alpha values between 0-255
 */
void MEdiText::setBackground(QString path, int alpha)
{
	Background back;
	QPalette palette;
	palette.setBrush(this->backgroundRole(), back.getBrush(path, alpha));
	this->setPalette(palette);
}

/**
 * Changes the text font.
 * 
 * @param family name
 * @param size in points
 */
void MEdiText::setFont(QString family, int size)
{
	this->textEdit->setFont(QFont(family, size));
}

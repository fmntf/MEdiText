#include <QApplication>
#include "meditext.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	MEdiText *dialog;
	if (argc > 1) {
		dialog = new MEdiText(argv[1]);
	} else {
		dialog = new MEdiText();
	}

    dialog->show();
    return app.exec();
}

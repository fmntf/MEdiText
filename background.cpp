#include <QPainter>
#include <QBrush>
#include "background.h"

/**
 * Factory method to create a background given an image and the
 * transparency level.
 * 
 * @param filename image file
 * @param level values from 0 (original image) to 255 (pure white image)
 * @return 
 */
QBrush Background::getBrush(QString filename, int level)
{
	QImage image = QImage(filename);
	QPainter p; 
	p.begin(&image);
	p.setCompositionMode(QPainter::CompositionMode_SourceOver);
	p.fillRect(image.rect(), QColor(255, 255, 255, level));
	p.end();

	return QBrush(image);
}

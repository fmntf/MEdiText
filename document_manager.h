#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

class DocumentManager
{
	public:
		virtual void createNewDocument() = 0;
		virtual void openExistingDocument() = 0;
		virtual bool saveDocument() = 0;
};

#endif


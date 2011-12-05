HEADERS     = meditext.h background.h preferences.h document_manager.h
SOURCES	    = meditext.cpp background.cpp preferences.cpp main.cpp
FORMS       = meditext.ui preferences.ui
RESOURCES   = resources.qrc #integra le icone nel binario

# install
target.path = meditext
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
sources.path = .
INSTALLS += target sources


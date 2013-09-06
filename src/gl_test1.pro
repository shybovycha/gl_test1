TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += /home/shybovycha/SFML-2.1/include
INCLUDEPATH += /home/shybovycha/glew-1.10.0/include
INCLUDEPATH += /home/shybovycha/assimp-3.0.2170/include

LIBS += -L/home/shybovycha/SFML-2.1/lib
LIBS += -L/home/shybovycha/glew-1.10.0/lib
LIBS += -lGL -lGLU -lGLEW -lsfml-system -lsfml-window -lsfml-graphics -lassimp

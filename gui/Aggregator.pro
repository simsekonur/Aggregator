QT       += core gui sql widgets concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    Forest/AggDataNode.cpp \
    Forest/AggExp.cpp \
    Forest/AggForest.cpp \
    Forest/AggFormulaNode.cpp \
    Forest/AggNode.cpp \
    Forest/AggOperators/Average.cpp \
    Forest/AggOperators/CountNonZero.cpp \
    Forest/AggOperators/LogicalAnd.cpp \
    Forest/AggOperators/LogicalOr.cpp \
    Forest/AggOperators/Max.cpp \
    Forest/AggOperators/Median.cpp \
    Forest/AggOperators/Min.cpp \
    Forest/AggOperators/Mod.cpp \
    Forest/AggOperators/Multiply.cpp \
    Forest/AggOperators/Sum.cpp \
    Forest/AggOperators/WeightedSum.cpp \
    Forest/AggParser.cpp \
    Forest/AggTree.cpp \
    Forest/DBHandler.cpp \
    Forest/UINode.cpp \
    Forest/agg.lex.cpp \
    Forest/agg.tab.cpp \
    ConnDialog.cpp \
    Forest/tinyxml/tinyxml2.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Forest/AggDataNode.hpp \
    Forest/AggExceptions.hpp \
    Forest/AggExp.hpp \
    Forest/AggForest.hpp \
    Forest/AggFormulaNode.hpp \
    Forest/AggNode.hpp \
    Forest/AggOperators/Average.h \
    Forest/AggOperators/CountNonZero.h \
    Forest/AggOperators/LogicalAnd.h \
    Forest/AggOperators/LogicalOr.h \
    Forest/AggOperators/Max.h \
    Forest/AggOperators/Median.h \
    Forest/AggOperators/Min.h \
    Forest/AggOperators/Mode.hpp \
    Forest/AggOperators/Multiply.hpp \
    Forest/AggOperators/Sum.h \
    Forest/AggOperators/WeightedSum.hpp \
    Forest/AggParser.hpp \
    Forest/AggTree.hpp \
    Forest/DBHandler.hpp \
    Forest/UINode.hpp \
    Forest/agg.lex.hpp \
    Forest/agg.tab.hpp \
    Forest/agglextab.hpp \
    ConnDialog.h \
    Forest/tinyxml/tinyxml2.h \
    mainwindow.h
    
FORMS += \
ConnDialog.ui \
    mainwindow.ui \

RESOURCES += res/icons.qrc \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L'C:/Program Files/MySQL/Connector C++ 8.0/lib64/vs14/' -lmysqlcppconn

INCLUDEPATH += 'C:/Program Files/MySQL/Connector C++ 8.0/include'
DEPENDPATH += 'C:/Program Files/MySQL/Connector C++ 8.0/include'

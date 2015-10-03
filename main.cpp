//#include <QtGui/QApplication>
#include <QtWidgets/QApplication>
#include "swipe.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SWipe w;
#if defined(Q_WS_S60)
    w.showMaximized();
#else
    w.show();
#endif

    return a.exec();
}

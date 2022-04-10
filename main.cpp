#include <QApplication>
#include <QMainWindow>
#include <QDesktopServices>
#include <QIcon>
#include "qtermwidget.h"

void activateUrl(const QUrl &url, bool fromContextMenu) {
	if (QApplication::keyboardModifiers() & Qt::ControlModifier || fromContextMenu) {
		QDesktopServices::openUrl(url);
	}
}
int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QMainWindow *mainWindow = new QMainWindow();

    QTermWidget *console = new QTermWidget();

    QFont font = QApplication::font();
    font.setFamily(QStringLiteral("Monospace"));
    font.setPointSize(12);

    console->setTerminalFont(font);
	console->setColorScheme("DarkPastels");

    console->setScrollBarPosition(QTermWidget::ScrollBarRight);
	
	mainWindow->setWindowIcon(QIcon::fromTheme("utilities-terminal"));
    mainWindow->setCentralWidget(console);
    mainWindow->resize(600, 400);
	
	QObject::connect(console, &QTermWidget::urlActivated, mainWindow, activateUrl);  

	QObject::connect(console, &QTermWidget::termKeyPressed, mainWindow,
		[=](const QKeyEvent *key) -> void {
			if (key->matches(QKeySequence::Copy)) {
				console->copyClipboard();
			} else if (key->matches(QKeySequence::Paste)) {
				console->pasteClipboard();
			}
		}
	);
	
    QObject::connect(console, &QTermWidget::finished, mainWindow, &QMainWindow::close);
    mainWindow->show();
    return app.exec();
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>

namespace Ui {
class MainWindow;
}

class KeyPressEater : public QObject
{
	Q_OBJECT

public:
	KeyPressEater() : QObject() {}

protected:
	bool eventFilter(QObject *obj, QEvent *event);

signals:
	void virtKeyEvent( int code, bool pressed );
};


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void keyPressEvent(QKeyEvent * event);
	void keyReleaseEvent(QKeyEvent * event);

private slots:
	void on_edtSerial_editingFinished();
	void key_event( QKeyEvent *event, bool pressed );
	void key_send( int code, bool pressed );

	void on_butLock_toggled(bool checked);

	void on_edtSerial_cursorPositionChanged(int arg1, int arg2);

	void on_butReset_clicked();

private:
	void key_assign( int code, QAbstractButton *widget );
	bool symbol_event( int key, bool pressed );

	Ui::MainWindow *ui;
	KeyPressEater *keyFilter;
	int serial_fd;

	int key_mods;
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QDebug>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "pc88_keycodes.h"

struct layout
{
	int code;
	int key, key_shift;
	QAbstractButton *widget;
};

struct layout key_layout[] =
{
	{ KEY88_RETURN, Qt::Key_unknown, Qt::Key_unknown, NULL },
	{ KEY88_SPACE, Qt::Key_Space, Qt::Key_unknown, NULL },
	{ KEY88_AT, Qt::Key_unknown, Qt::Key_unknown, NULL },
	{ KEY88_A, Qt::Key_A, Qt::Key_unknown, NULL },
	{ KEY88_B, Qt::Key_B, Qt::Key_unknown, NULL },
	{ KEY88_C, Qt::Key_C, Qt::Key_unknown, NULL },
	{ KEY88_D, Qt::Key_D, Qt::Key_unknown, NULL },
	{ KEY88_E, Qt::Key_E, Qt::Key_unknown, NULL },
	{ KEY88_F, Qt::Key_F, Qt::Key_unknown, NULL },
	{ KEY88_G, Qt::Key_G, Qt::Key_unknown, NULL },
	{ KEY88_H, Qt::Key_H, Qt::Key_unknown, NULL },
	{ KEY88_I, Qt::Key_I, Qt::Key_unknown, NULL },
	{ KEY88_J, Qt::Key_J, Qt::Key_unknown, NULL },
	{ KEY88_K, Qt::Key_K, Qt::Key_unknown, NULL },
	{ KEY88_L, Qt::Key_L, Qt::Key_unknown, NULL },
	{ KEY88_M, Qt::Key_M, Qt::Key_unknown, NULL },
	{ KEY88_N, Qt::Key_N, Qt::Key_unknown, NULL },
	{ KEY88_O, Qt::Key_O, Qt::Key_unknown, NULL },
	{ KEY88_P, Qt::Key_P, Qt::Key_unknown, NULL },
	{ KEY88_Q, Qt::Key_Q, Qt::Key_unknown, NULL },
	{ KEY88_R, Qt::Key_R, Qt::Key_unknown, NULL },
	{ KEY88_S, Qt::Key_S, Qt::Key_unknown, NULL },
	{ KEY88_T, Qt::Key_T, Qt::Key_unknown, NULL },
	{ KEY88_U, Qt::Key_U, Qt::Key_unknown, NULL },
	{ KEY88_V, Qt::Key_V, Qt::Key_unknown, NULL },
	{ KEY88_W, Qt::Key_W, Qt::Key_unknown, NULL },
	{ KEY88_X, Qt::Key_X, Qt::Key_unknown, NULL },
	{ KEY88_Y, Qt::Key_Y, Qt::Key_unknown, NULL },
	{ KEY88_Z, Qt::Key_Z, Qt::Key_unknown, NULL },
	{ KEY88_BRL, Qt::Key_BracketLeft, Qt::Key_BraceLeft, NULL },
	{ KEY88_BACKSLASH, Qt::Key_Backslash, Qt::Key_Bar, NULL },
	{ KEY88_BRR, Qt::Key_BracketRight, Qt::Key_BraceRight, NULL },
	{ KEY88_TILDE, Qt::Key_QuoteLeft, Qt::Key_AsciiTilde, NULL },
	{ KEY88_MINUS, Qt::Key_Minus, Qt::Key_Underscore, NULL },
	{ KEY88_1, Qt::Key_1, Qt::Key_Exclam, NULL },
	{ KEY88_2, Qt::Key_2, Qt::Key_At, NULL },
	{ KEY88_3, Qt::Key_3, Qt::Key_NumberSign, NULL },
	{ KEY88_4, Qt::Key_4, Qt::Key_Dollar, NULL },
	{ KEY88_5, Qt::Key_5, Qt::Key_Percent, NULL },
	{ KEY88_6, Qt::Key_6, Qt::Key_AsciiCircum, NULL },
	{ KEY88_7, Qt::Key_7, Qt::Key_Ampersand, NULL },
	{ KEY88_8, Qt::Key_8, Qt::Key_Asterisk, NULL },
	{ KEY88_9, Qt::Key_9, Qt::Key_ParenLeft, NULL },
	{ KEY88_0, Qt::Key_0, Qt::Key_ParenRight, NULL },

	{ KEY88_COLON, Qt::Key_Apostrophe, Qt::Key_QuoteDbl, NULL },
	{ KEY88_SEMICOLON, Qt::Key_Semicolon, Qt::Key_Colon, NULL },
	{ KEY88_COMMA, Qt::Key_Comma, Qt::Key_Less, NULL },
	{ KEY88_PERIOD, Qt::Key_Period, Qt::Key_Greater, NULL },
	{ KEY88_SLASH, Qt::Key_Slash, Qt::Key_Question, NULL },
	{ KEY88_UNDERSCORE, Qt::Key_Underscore, Qt::Key_unknown, NULL },

	{ KEY88_SHIFT, Qt::Key_Shift, Qt::Key_unknown, NULL },
	{ KEY88_CTRL, Qt::Key_Control, Qt::Key_unknown, NULL },
	{ KEY88_GRPH, Qt::Key_Alt, Qt::Key_unknown, NULL },
	{ KEY88_KANA, Qt::Key_Meta, Qt::Key_unknown, NULL },
	{ KEY88_CAPS, Qt::Key_CapsLock, Qt::Key_unknown, NULL },

	{ KEY88_UP, Qt::Key_Up, Qt::Key_unknown, NULL },
	{ KEY88_DOWN, Qt::Key_Down, Qt::Key_unknown, NULL },
	{ KEY88_LEFT, Qt::Key_Left, Qt::Key_unknown, NULL },
	{ KEY88_RIGHT, Qt::Key_Right, Qt::Key_unknown, NULL },

	{ KEY88_INSDEL, 0, Qt::Key_unknown, NULL },
	{ KEY88_TAB, Qt::Key_Tab, Qt::Key_unknown, NULL },
	{ KEY88_ROLLUP, Qt::Key_PageUp, Qt::Key_unknown, NULL },
	{ KEY88_ROLLDOWN, Qt::Key_PageDown, Qt::Key_unknown, NULL },

	{ KEY88_F1, Qt::Key_F1, Qt::Key_unknown, NULL },
	{ KEY88_F2, Qt::Key_F2, Qt::Key_unknown, NULL },
	{ KEY88_F3, Qt::Key_F3, Qt::Key_unknown, NULL },
	{ KEY88_F4, Qt::Key_F4, Qt::Key_unknown, NULL },
	{ KEY88_F5, Qt::Key_F5, Qt::Key_unknown, NULL },
	{ KEY88_F6, Qt::Key_F6, Qt::Key_unknown, NULL },
	{ KEY88_F7, Qt::Key_F7, Qt::Key_unknown, NULL },
	{ KEY88_F8, Qt::Key_F8, Qt::Key_unknown, NULL },
	{ KEY88_F9, Qt::Key_F9, Qt::Key_unknown, NULL },
	{ KEY88_F10, Qt::Key_F10, Qt::Key_unknown, NULL },

	{ KEY88_ESC, Qt::Key_Escape, Qt::Key_unknown, NULL },
	{ KEY88_HELP, Qt::Key_End, Qt::Key_unknown, NULL },
	{ KEY88_STOP, Qt::Key_F11, Qt::Key_unknown, NULL },
	{ KEY88_COPY, Qt::Key_F12, Qt::Key_unknown, NULL },

	{ KEY88_HOMECLR, Qt::Key_Home, Qt::Key_unknown, NULL },
	{ KEY88_KP_0, Qt::KeypadModifier | Qt::Key_0, Qt::Key_unknown, NULL },
	{ KEY88_KP_1, Qt::KeypadModifier | Qt::Key_1, Qt::Key_unknown, NULL },
	{ KEY88_KP_2, Qt::KeypadModifier | Qt::Key_2, Qt::Key_unknown, NULL },
	{ KEY88_KP_3, Qt::KeypadModifier | Qt::Key_3, Qt::Key_unknown, NULL },
	{ KEY88_KP_4, Qt::KeypadModifier | Qt::Key_4, Qt::Key_unknown, NULL },
	{ KEY88_KP_5, Qt::KeypadModifier | Qt::Key_5, Qt::Key_unknown, NULL },
	{ KEY88_KP_6, Qt::KeypadModifier | Qt::Key_6, Qt::Key_unknown, NULL },
	{ KEY88_KP_7, Qt::KeypadModifier | Qt::Key_7, Qt::Key_unknown, NULL },
	{ KEY88_KP_8, Qt::KeypadModifier | Qt::Key_8, Qt::Key_unknown, NULL },
	{ KEY88_KP_9, Qt::KeypadModifier | Qt::Key_9, Qt::Key_unknown, NULL },
	{ KEY88_KP_MUL, Qt::KeypadModifier | Qt::Key_Asterisk, Qt::Key_unknown, NULL },
	{ KEY88_KP_PLUS, Qt::KeypadModifier | Qt::Key_Plus, Qt::Key_unknown, NULL },
	{ KEY88_KP_EQUAL, Qt::Key_Equal, Qt::Key_Plus, NULL },
	{ KEY88_KP_COMMA, Qt::Key_unknown, Qt::Key_unknown, NULL },
	{ KEY88_KP_PERIOD, Qt::KeypadModifier | Qt::Key_Period, Qt::Key_unknown, NULL },
	{ KEY88_KP_MINUS, Qt::KeypadModifier | Qt::Key_Minus, Qt::Key_unknown, NULL },
	{ KEY88_KP_DIV, Qt::KeypadModifier | Qt::Key_Slash, Qt::Key_unknown, NULL },

	{ KEY88_BACKSPACE, Qt::Key_Backspace, Qt::Key_unknown, NULL },
	{ KEY88_INS, Qt::Key_Insert, Qt::Key_unknown, NULL },
	{ KEY88_DEL, Qt::Key_Delete, Qt::Key_unknown, NULL },
	{ KEY88_HENKAN, Qt::Key_unknown, Qt::Key_unknown, NULL },
	{ KEY88_KETTEI, Qt::Key_unknown, Qt::Key_unknown, NULL },
	{ KEY88_PC, Qt::Key_Menu, Qt::Key_unknown, NULL },
	{ KEY88_ZENKAKU, Qt::Key_ScrollLock, Qt::Key_unknown, NULL },
	{ KEY88_RETURNL, Qt::Key_Return, Qt::Key_unknown, NULL },
	{ KEY88_RETURNR, Qt::KeypadModifier | Qt::Key_Enter, Qt::Key_unknown, NULL },
	{ KEY88_SHIFTL, Qt::Key_unknown, Qt::Key_unknown, NULL },
	{ KEY88_SHIFTR, Qt::Key_unknown, Qt::Key_unknown, NULL },

	{ KEY88_NONE, Qt::Key_unknown, Qt::Key_unknown, NULL }
};

void MainWindow::key_assign( int code, QAbstractButton *widget )
{
	struct layout *l;

	for ( l = key_layout; l->code != KEY88_NONE; l ++ )
	{
		if ( l->code == code )
		{
			l->widget = widget;
			widget->installEventFilter(keyFilter);
			return;
		}
	}
}

struct layout *key_get_code( int code )
{
	struct layout *l;

	for ( l = key_layout; l->code != KEY88_NONE; l ++ )
	{
		if ( l->code == code )
			return l;
	}

	return NULL;
}

struct layout *key_get( int key )
{
	struct layout *l;
	bool shifted = false;

	if ( key & Qt::ShiftModifier )
	{
		key &= ~Qt::ShiftModifier;
		shifted = true;
	}

	for ( l = key_layout; l->code != KEY88_NONE; l ++ )
	{
		if ( shifted && (l->key_shift == key) )
			return l;
		else if ( l->key == key )
			return l;
	}

	return NULL;
}

struct layout *key_get( QAbstractButton *widget )
{
	struct layout *l;

	for ( l = key_layout; l->code != KEY88_NONE; l ++ )
	{
		if ( l->widget == widget )
			return l;
	}

	return NULL;
}

bool KeyPressEater::eventFilter(QObject *obj, QEvent *event)
{
	if ( (event->type() == QEvent::MouseButtonPress) || (event->type() == QEvent::MouseButtonRelease) )
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		QAbstractButton *key = static_cast<QAbstractButton *>(obj);
		struct layout *l = key_get( key );
//fprintf(stderr,"vkey %p\n",l);
		if ( (event->type() == QEvent::MouseButtonPress) && (mouseEvent->button() == Qt::RightButton) )
		{
			if ( l )
				emit virtKeyEvent( l->code, !key->isDown() );
			key->setDown( !key->isDown() );
			//key->setChecked( key->isChecked() );

			return true;
		}

		if ( (mouseEvent->button() == Qt::LeftButton) && l )
		{
			if ( l->widget && l->widget->isCheckable() )
			{
				if ( event->type() == QEvent::MouseButtonPress )
					emit virtKeyEvent( l->code, !l->widget->isChecked() );
			}
			else
				emit virtKeyEvent( l->code, event->type() == QEvent::MouseButtonPress );
		}
	}

	return QObject::eventFilter(obj, event);
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow),
	serial_fd( -1 ),
	key_mods( 0 )
{
	ui->setupUi(this);
	ui->lblLogo->setScaledContents( true );
	ui->lblLogo->setPixmap( QPixmap(":/img/logo.png") );

	keyFilter = new KeyPressEater;
	connect( keyFilter, SIGNAL(virtKeyEvent(int,bool)), this, SLOT(key_send(int,bool)) );

	on_edtSerial_editingFinished();

	//key_assign( KEY88_RETURN, ui->keyReturn );
	key_assign( KEY88_SPACE, ui->keySpace );
	key_assign( KEY88_AT, ui->keyAt );
	key_assign( KEY88_A, ui->keyA );
	key_assign( KEY88_B, ui->keyB );
	key_assign( KEY88_C, ui->keyC );
	key_assign( KEY88_D, ui->keyD );
	key_assign( KEY88_E, ui->keyE );
	key_assign( KEY88_F, ui->keyF );
	key_assign( KEY88_G, ui->keyG );
	key_assign( KEY88_H, ui->keyH );
	key_assign( KEY88_I, ui->keyI );
	key_assign( KEY88_J, ui->keyJ );
	key_assign( KEY88_K, ui->keyK );
	key_assign( KEY88_L, ui->keyL );
	key_assign( KEY88_M, ui->keyM );
	key_assign( KEY88_N, ui->keyN );
	key_assign( KEY88_O, ui->keyO );
	key_assign( KEY88_P, ui->keyP );
	key_assign( KEY88_Q, ui->keyQ );
	key_assign( KEY88_R, ui->keyR );
	key_assign( KEY88_S, ui->keyS );
	key_assign( KEY88_T, ui->keyT );
	key_assign( KEY88_U, ui->keyU );
	key_assign( KEY88_V, ui->keyV );
	key_assign( KEY88_W, ui->keyW );
	key_assign( KEY88_X, ui->keyX );
	key_assign( KEY88_Y, ui->keyY );
	key_assign( KEY88_Z, ui->keyZ );
	key_assign( KEY88_BRL, ui->keySqBrL );
	key_assign( KEY88_BACKSLASH, ui->keyYen );
	key_assign( KEY88_BRR, ui->keySqBrR );
	key_assign( KEY88_TILDE, ui->keyTilde );
	key_assign( KEY88_MINUS, ui->keyMinus );
	key_assign( KEY88_0, ui->key0 );
	key_assign( KEY88_1, ui->key1 );
	key_assign( KEY88_2, ui->key2 );
	key_assign( KEY88_3, ui->key3 );
	key_assign( KEY88_4, ui->key4 );
	key_assign( KEY88_5, ui->key5 );
	key_assign( KEY88_6, ui->key6 );
	key_assign( KEY88_7, ui->key7 );
	key_assign( KEY88_8, ui->key8 );
	key_assign( KEY88_9, ui->key9 );

	key_assign( KEY88_COLON, ui->keyColon );
	key_assign( KEY88_SEMICOLON, ui->keySemicolon );
	key_assign( KEY88_COMMA, ui->keyComma );
	key_assign( KEY88_PERIOD, ui->keyPeriod );
	key_assign( KEY88_SLASH, ui->keySlash );
	key_assign( KEY88_UNDERSCORE, ui->keyUnderscore );
	key_assign( KEY88_HOMECLR, ui->keyHomeClr );
	key_assign( KEY88_UP, ui->keyUp );
	key_assign( KEY88_DOWN, ui->keyDown );
	key_assign( KEY88_LEFT, ui->keyLeft );
	key_assign( KEY88_RIGHT, ui->keyRight );
	//key_assign( KEY88_INSDEL, ui->keyInsDel );
	key_assign( KEY88_GRPH, ui->keyGrph );
	key_assign( KEY88_KANA, ui->keyKana );
	//key_assign( KEY88_SHIFT, ui->keyShift );
	key_assign( KEY88_CTRL, ui->keyCtrl );
	key_assign( KEY88_STOP, ui->keyStop );
	key_assign( KEY88_F1, ui->keyF1 );
	key_assign( KEY88_F2, ui->keyF2 );
	key_assign( KEY88_F3, ui->keyF3 );
	key_assign( KEY88_F4, ui->keyF4 );
	key_assign( KEY88_F5, ui->keyF5 );
	key_assign( KEY88_F6, ui->keyF6 );
	key_assign( KEY88_F7, ui->keyF7 );
	key_assign( KEY88_F8, ui->keyF8 );
	key_assign( KEY88_F9, ui->keyF9 );
	key_assign( KEY88_F10, ui->keyF10 );
	key_assign( KEY88_ESC, ui->keyEsc );
	key_assign( KEY88_TAB, ui->keyTab );
	key_assign( KEY88_HELP, ui->keyHelp );
	key_assign( KEY88_COPY, ui->keyCopy );
	key_assign( KEY88_CAPS, ui->keyCaps );
	key_assign( KEY88_ROLLUP, ui->keyRollUp );
	key_assign( KEY88_ROLLDOWN, ui->keyRollDown );

	key_assign( KEY88_KP_0, ui->keyKP0 );
	key_assign( KEY88_KP_1, ui->keyKP1 );
	key_assign( KEY88_KP_2, ui->keyKP2 );
	key_assign( KEY88_KP_3, ui->keyKP3 );
	key_assign( KEY88_KP_4, ui->keyKP4 );
	key_assign( KEY88_KP_5, ui->keyKP5 );
	key_assign( KEY88_KP_6, ui->keyKP6 );
	key_assign( KEY88_KP_7, ui->keyKP7 );
	key_assign( KEY88_KP_8, ui->keyKP8 );
	key_assign( KEY88_KP_9, ui->keyKP9 );
	key_assign( KEY88_KP_MUL, ui->keyKPMul );
	key_assign( KEY88_KP_PLUS, ui->keyKPPlus );
	key_assign( KEY88_KP_EQUAL, ui->keyKPEqual );
	key_assign( KEY88_KP_COMMA, ui->keyKPComma );
	key_assign( KEY88_KP_PERIOD, ui->keyKPPeriod );
	key_assign( KEY88_KP_MINUS, ui->keyKPMinus );
	key_assign( KEY88_KP_DIV, ui->keyKPDiv );

	key_assign( KEY88_BACKSPACE, ui->keyBackspace );
	key_assign( KEY88_INS, ui->keyIns );
	key_assign( KEY88_DEL, ui->keyDel );
	key_assign( KEY88_HENKAN, ui->keyHenkan );
	key_assign( KEY88_KETTEI, ui->keyKettei );
	key_assign( KEY88_PC, ui->keyPC );
	key_assign( KEY88_ZENKAKU, ui->keyZenkaku );
	key_assign( KEY88_RETURNL, ui->keyReturn );
	key_assign( KEY88_RETURNR, ui->keyKPEnter );
	key_assign( KEY88_SHIFTL, ui->keyLShift );
	key_assign( KEY88_SHIFTR, ui->keyRShift );

	setFocus( Qt::ActiveWindowFocusReason );
	//grabKeyboard();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_edtSerial_editingFinished()
{
	QPalette pal;
	pal.setColor( QPalette::Text, QColor(Qt::white) );

	serial_fd = open( ui->edtSerial->text().toLocal8Bit().data(), O_WRONLY );fprintf(stderr,"serial fd: %d\n",serial_fd);
	if ( serial_fd < 0 )
	{
		pal.setColor( QPalette::Base, QColor(Qt::darkRed) );
		ui->edtSerial->setPalette( pal );
		return;
	}

	pal.setColor( QPalette::Base, QColor(Qt::darkGreen) );
	ui->edtSerial->setPalette( pal );

	struct termios termios_p;

	if ( tcgetattr( serial_fd, &termios_p ) )
		return;

	cfsetispeed( &termios_p, B9600 );
	cfsetospeed( &termios_p, B9600 );
	termios_p.c_cflag &= ~(CSIZE|CSTOPB|PARENB);
	termios_p.c_cflag |= CS8;
	termios_p.c_cc[VMIN]  = 1;
	termios_p.c_cc[VTIME] = 0;
	termios_p.c_lflag &= ~(ECHO|ICANON|ISIG|ECHOE|ECHOK|ECHONL);
	termios_p.c_oflag &= ~(OPOST);
	tcsetattr( serial_fd, TCSADRAIN, &termios_p );
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
	key_event( event, true );
}

void MainWindow::keyReleaseEvent(QKeyEvent * event)
{
	key_event( event, false );
}

#define KEY88MOD_SHIFT		0x01
#define KEY88MOD_CAPS		0x02
#define KEY88MOD_KANA		0x04
#define KEY88MOD_GRPH		0x08
#define KEY88MOD_ZENKAKU	0x10

struct sym_layout
{
	int key;
	int mod, code;
};

struct sym_layout keysym_layout[] =
{
	{ Qt::Key_QuoteDbl, KEY88MOD_SHIFT, KEY88_2 },
	{ Qt::Key_Ampersand, KEY88MOD_SHIFT, KEY88_6 },
	{ Qt::Key_Apostrophe, KEY88MOD_SHIFT, KEY88_7 },
	{ Qt::Key_ParenLeft, KEY88MOD_SHIFT, KEY88_8 },
	{ Qt::Key_ParenRight, KEY88MOD_SHIFT, KEY88_9 },
	{ Qt::Key_Minus, 0, KEY88_MINUS },
	{ Qt::Key_Equal, KEY88MOD_SHIFT, KEY88_MINUS },
	{ Qt::Key_At, 0, KEY88_AT },
	{ Qt::Key_Semicolon, 0, KEY88_SEMICOLON },
	{ Qt::Key_Plus, KEY88MOD_SHIFT, KEY88_SEMICOLON },
	{ Qt::Key_Colon, 0, KEY88_COLON },
	{ Qt::Key_Asterisk, KEY88MOD_SHIFT, KEY88_COLON },
	{ Qt::Key_Underscore, 0, KEY88_UNDERSCORE },
	{ Qt::Key_AsciiCircum, 0, KEY88_TILDE },
	//{ Qt::Key_AsciiTilde, 0, KEY88_AT },

	{ Qt::Key_unknown, 0, KEY88_NONE }
};

bool MainWindow::symbol_event( int key, bool pressed )
{
	struct sym_layout *l;
	bool shifted = false;

	if ( key & Qt::ShiftModifier )
	{
		key &= ~Qt::ShiftModifier;
		shifted = true;
	}

	for ( l = keysym_layout; l->code != KEY88_NONE; l ++ )
	{
		if ( l->key == key )
		{
			struct layout *vl;

			if ( pressed )
			{
				if ( shifted != (l->mod & KEY88MOD_SHIFT) )
					key_send( KEY88_SHIFT, l->mod & KEY88MOD_SHIFT );

				vl = key_get_code( KEY88_SHIFTL );
				if ( vl && vl->widget )
					vl->widget->setDown( l->mod & KEY88MOD_SHIFT );
			}

			key_send( l->code, pressed );
			vl = key_get_code( l->code );
			if ( vl && vl->widget )
				vl->widget->setDown( pressed );

			if ( !pressed )
			{
				if ( shifted != (l->mod & KEY88MOD_SHIFT) )
					key_send( KEY88_SHIFT, shifted );

				vl = key_get_code( KEY88_SHIFTL );
				if ( vl && vl->widget )
					vl->widget->setDown( shifted );
			}

			return true;
		}
	}

	return false;
}

void MainWindow::key_event( QKeyEvent *event, bool pressed )
{
	struct layout *l;
	int key = event->key();
	int mod = 0;
fprintf(stderr,"%s %x\n",__FUNCTION__,key);
	if ( event->modifiers() & Qt::KeypadModifier )
		key |= Qt::KeypadModifier;
	if ( event->modifiers() & Qt::ShiftModifier )
		key |= Qt::ShiftModifier;

	if ( ui->ckbSymbol->isChecked() )
	{
		if ( symbol_event( key, pressed ) )
			return;
	}

	l = key_get( key );
	if ( !l )
		return;

	switch ( l->code )
	{
		case KEY88_CAPS:
			mod |= KEY88MOD_CAPS;
			break;
		case KEY88_KANA:
			mod |= KEY88MOD_KANA;
			break;
		/*case KEY88_GRPH:
			mod |= KEY88MOD_GRPH;
			break;*/
		case KEY88_ZENKAKU:
			mod |= KEY88MOD_ZENKAKU;
			break;
	}

	if ( mod )
	{
		if ( !pressed )
			return;

		key_mods ^= mod;
		pressed = key_mods & mod;
	}

	if ( l->widget )
		l->widget->setDown( pressed );
	else if ( event->key() == Qt::Key_Shift )
	{
		ui->keyLShift->setDown( pressed );
		ui->keyRShift->setDown( pressed );
	}

	key_send( l->code, pressed );
}

#define MAKE_CODE(c,p)	\
	((c) & 7) | (((c) >> 1) & 0x78) | ((p) ? 0 : 0x80)

void MainWindow::key_send( int code, bool pressed )
{
	QByteArray data;
	int convetional = code;
fprintf(stderr,"88key %x %s",code,pressed?"press":"release");
	switch ( code )
	{
		case KEY88_F6: convetional = KEY88_F1; break;
		case KEY88_F7: convetional = KEY88_F2; break;
		case KEY88_F8: convetional = KEY88_F3; break;
		case KEY88_F9: convetional = KEY88_F4; break;
		case KEY88_F10: convetional = KEY88_F5; break;
		case KEY88_BACKSPACE: convetional = KEY88_INSDEL; break;
		case KEY88_INS: convetional = KEY88_INSDEL; break;
		case KEY88_DEL: convetional = KEY88_INSDEL; break;
		case KEY88_HENKAN: convetional = KEY88_SPACE; break;
		case KEY88_KETTEI: convetional = KEY88_SPACE; break;
		case KEY88_RETURNL: convetional = KEY88_RETURN; break;
		case KEY88_RETURNR: convetional = KEY88_RETURN; break;
		case KEY88_SHIFTL: convetional = KEY88_SHIFT; break;
		case KEY88_SHIFTR: convetional = KEY88_SHIFT; break;
	}

	data.append( MAKE_CODE(code, pressed) );
	if ( code & KEY88_EXT )
	{
		switch ( code )
		{
			case KEY88_INS:
				data.append( MAKE_CODE(KEY88_SHIFT, pressed) );
				data.append( MAKE_CODE(convetional, pressed) );
				break;
			case KEY88_F6:
			case KEY88_F7:
			case KEY88_F8:
			case KEY88_F9:
			case KEY88_F10:
				if ( pressed )
				{
					data.append( MAKE_CODE(KEY88_SHIFT, true) );
					data.append( MAKE_CODE(convetional, true) );
				}
				else
				{
					data.append( MAKE_CODE(convetional, false) );
					data.append( MAKE_CODE(KEY88_SHIFT, false) );
				}
				break;
			default:
				data.append( MAKE_CODE(convetional, pressed) );
		}
	}

	ui->keyPacket->setText( data.toHex().toUpper() );

	if ( serial_fd < 0 )
	{fprintf(stderr,"\n");
		return;
	}
fprintf(stderr," -");
	for ( int i = 0; i < data.size(); ++i )
	{fprintf(stderr," %.2x",(uchar)data.at(i));
		write( serial_fd, data.constData() + i, 1 );
		usleep( 10 * 1000 );
	}fprintf(stderr,"\n");
}

void MainWindow::on_butLock_toggled(bool checked)
{
	if ( checked )
		grabKeyboard();
	else
		releaseKeyboard();
}

void MainWindow::on_edtSerial_cursorPositionChanged(int arg1, int arg2)
{
	ui->edtSerial->setPalette( QPalette() );
}

void MainWindow::on_butReset_clicked()
{
	int row, i;

	if ( serial_fd < 0 )
		return;

	for ( row = 0; row < 0xF; ++row )
	{
		for ( i = 0; i < 8; ++i )
		{
			uchar tmp = ((row == 0xE) && (i == 7) ? 0 : 0x80) | (row << 3) | i;
			write( serial_fd, &tmp, 1 );
			usleep( 10 * 1000 );
		}
	}
}

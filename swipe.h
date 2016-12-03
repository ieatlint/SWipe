#ifndef MAGSTRIPE_H
#define MAGSTRIPE_H

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QScrollArea>

#include <QAudioInput>

#include "magdecode.h"
#include "carddetect.h"

class SWipe : public QMainWindow {
	Q_OBJECT
	
	public:
		SWipe(QWidget *parent = 0);
		~SWipe();
	
	private:
		void mkWindow();

		QLabel charStreamLabel;
		QLabel bitStreamLabel;
		QLabel encodingLabel;
		QLabel validSwipeLabel;
		QLabel typeLabel;
		QLabel accountNumberLabel;
		QLabel accountHolderLabel;
		QLabel accountIssuerLabel;
		QLabel accountValidLabel;
		QLabel expirationDateLabel;
		QLabel aamvaIssuerLabel;
		QLabel aamvaIssuerNameLabel;
		QLabel aamvaBirthdayLabel;
		QLabel aamvaAgeLabel;

		CardDetect cdetect;


		QLabel statusLabel;

		QPushButton mainBtn;

		bool captureAudio;
		MagDecode *magDec;
		QAudioInput *audioInput;
		QAudioFormat audioFormat;

		void captureStart();
		void captureStop();

	private slots:
		void mainBtn_clicked();
		void showCard( MagCard card );
};

#endif // MAGSTRIPE_H

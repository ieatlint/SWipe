#include "swipe.h"

#include <QDebug>

SWipe::SWipe(QWidget *parent) : QMainWindow(parent) {
	captureAudio = false;

	audioFormat.setFrequency( 48000 );
	audioFormat.setChannels( 1 );
	audioFormat.setSampleSize( 16 );
	audioFormat.setCodec( "audio/pcm" );
	audioFormat.setByteOrder( QAudioFormat::LittleEndian );
	audioFormat.setSampleType( QAudioFormat::SignedInt );

	QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
	if( !info.isFormatSupported( audioFormat ) ) {
		qWarning() << "default format not supported try to use nearest";
		audioFormat = info.nearestFormat( audioFormat );
	}

	magDec = NULL;

	mkWindow();
}

SWipe::~SWipe() {
}


void SWipe::mkWindow() {
        QScrollArea *scroll = new QScrollArea;
	QWidget *widget = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout( widget );

        scroll->setWidget( widget );
        scroll->setWidgetResizable( true );

	QFormLayout *form = new QFormLayout;
	form->addRow( "Raw Chars:", &charStreamLabel );
	form->addRow( "Bits:", &bitStreamLabel );
	bitStreamLabel.setWordWrap( true );
	form->addRow( "Encoding:", &encodingLabel );
	form->addRow( "Valid Swipe:", &validSwipeLabel );
	form->addRow( "Card Type:", &typeLabel );
	form->addRow( "Account Number:", &accountNumberLabel );
	form->addRow( "Account Holder:", &accountHolderLabel );
	form->addRow( "Account Issuer:", &accountIssuerLabel );
	form->addRow( "Luhn Algorithm Pass:", &accountValidLabel );
	form->addRow( "Expiration Date:", &expirationDateLabel );
	form->addRow( "AAMVA Issuer:", &aamvaIssuerLabel );
	form->addRow( "AAMVA Issuer Name:", &aamvaIssuerNameLabel );
	form->addRow( "AAMVA Birthday:", &aamvaBirthdayLabel );
	form->addRow( "AAMVA Birthday:", &aamvaAgeLabel );

	layout->addLayout( form );

	layout->addWidget( &statusLabel );

	mainBtn.setText( "Start" );
	layout->addWidget( &mainBtn );
	connect( &mainBtn, SIGNAL( clicked() ), this, SLOT( mainBtn_clicked() ) );


        resize( 640, 480 );


        setCentralWidget( scroll );
}


void SWipe::mainBtn_clicked() {
	if( captureAudio ) {
		captureStop();
		statusLabel.setText( "Stopped.  Press Start to capture" );
	} else {
		captureStart();
		statusLabel.setText( "Capturing audio.  Press Stop to stop" );
	}

	captureAudio = !captureAudio;
}

void SWipe::captureStart() {
	magDec = new MagDecode( this );
	connect( magDec, SIGNAL( cardRead( MagCard ) ), this, SLOT( showCard( MagCard ) ) );
	connect( magDec, SIGNAL( errorMsg( QString ) ), &statusLabel, SLOT( setText( QString ) ) );

	audioInput = new QAudioInput( audioFormat, this );
	magDec->start();
	audioInput->start( magDec );
	mainBtn.setText( "Stop" );
}

void SWipe::captureStop() {
	if( !magDec )
		return;

	audioInput->stop();
	magDec->stop();
	delete audioInput;
	delete magDec;
	mainBtn.setText( "Start" );
	magDec = NULL;
	
}

void SWipe::showCard( MagCard card ) {
	if( card.bitStream.isEmpty() )
		return;
	cdetect.setCard( &card );

	charStreamLabel.setText( card.charStream );
	for( int i = 50; i < card.bitStream.length(); i += 51 )
		card.bitStream.insert( i, "\n" );
	bitStreamLabel.setText( card.bitStream );

	if( card.encoding == ABA ) {
		encodingLabel.setText( "ABA" );
	} else {
		encodingLabel.setText( "IATA" );
	}

	if( card.swipeValid ) {
		validSwipeLabel.setText( "True" );
		statusLabel.setText( "Card Read!" );
	} else {
		validSwipeLabel.setText( "False" );
		statusLabel.setText( "Card Swipe Failed; Please Retry!" );
	}

	if( card.type == MagCard::CARD_AAMVA )
		typeLabel.setText( "AAMVA" );
	else if( card.type & MagCard::CARD_CC )
		typeLabel.setText( "Credit Card" );
	else
		typeLabel.setText( "Unknown" );

	accountNumberLabel.setText( card.accountNumber );
        if( card.accountHolder.isEmpty() )
                accountHolderLabel.setText( "n/a" );
        else
            accountHolderLabel.setText( card.accountHolder );

	if( card.type & MagCard::CARD_CC ) {
		accountIssuerLabel.setText( card.accountIssuer );
		if( card.accountValid )
			accountValidLabel.setText( "True" );
		else
			accountValidLabel.setText( "False" );
	} else {
		accountIssuerLabel.setText( "n/a" );
		accountValidLabel.setText( "n/a" );
	}

	expirationDateLabel.setText( card.expirationDate.toString( "MM/dd/yyyy" ) );

	if( card.type == MagCard::CARD_AAMVA ) {
		aamvaIssuerLabel.setText( card.aamvaIssuer );
		aamvaIssuerNameLabel.setText( card.aamvaIssuerName + " (" + card.aamvaIssuerAbr + ')' );
		aamvaBirthdayLabel.setText( card.aamvaBirthday.toString( "MM/dd/yyyy" ) );
		aamvaAgeLabel.setText( QString( "%1" ).arg( card.aamvaAge )  );
	} else {
		aamvaIssuerLabel.setText( "n/a" );
		aamvaIssuerNameLabel.setText( "n/a" );
		aamvaBirthdayLabel.setText( "n/a" );
                aamvaAgeLabel.setText( "n/a" );
	}

        card.clear();

}

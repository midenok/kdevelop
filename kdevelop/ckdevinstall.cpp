/***************************************************************************
                          ckdevinstall.cpp  -  description                              
                             -------------------                                         
    begin                : Thu Mar 4 1999                                           
    copyright            : (C) 1999 by Ralf Nolden
    email                : Ralf.Nolden@post.rwth-aachen.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#include <iostream.h>

#include <qdir.h>
#include <qtimer.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qprogressdialog.h>

#include <kapp.h>
#include <kmsgbox.h>
#include <kfiledialog.h>
#include <kprocess.h>

#include "ckdevinstall.h"
#include "ctoolclass.h"
#include "cupdatekdedocdlg.h"
#include "ccreatedocdatabasedlg.h"

CKDevInstall::CKDevInstall(QWidget *parent, const char *name ) : QDialog(parent,name,true) {

	config = kapp->getConfig();

  setCaption(i18n("KDevelop Installation"));

  main_frame = new QFrame( this, "Frame_2" );
	main_frame->setGeometry( 10, 10, 500, 290 );
	{
		QColorGroup normal( QColor( QRgb(0) ), QColor( QRgb(128) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(0) ), QColor( QRgb(16777215) ) );
		QColorGroup disabled( QColor( QRgb(8421504) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(8421504) ), QColor( QRgb(12632256) ) );
		QColorGroup active( QColor( QRgb(0) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(0) ), QColor( QRgb(16777215) ) );
		QPalette palette( normal, disabled, active );
		main_frame->setPalette( palette );
	}
	main_frame->setFrameStyle( 33 );
	main_frame->setLineWidth( 2 );


	welcome_label = new QLabel( this, "Label_2" );
	welcome_label->setGeometry( 40, 30, 440, 40 );
	{
		QColorGroup normal( QColor( QRgb(0) ), QColor( QRgb(8388608) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(16777215) ), QColor( QRgb(16777215) ) );
		QColorGroup disabled( QColor( QRgb(8421504) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(8421504) ), QColor( QRgb(12632256) ) );
		QColorGroup active( QColor( QRgb(0) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(0) ), QColor( QRgb(16777215) ) );
		QPalette palette( normal, disabled, active );
		welcome_label->setPalette( palette );
	}
	{
		QFont font( "Lucida", 18, 50, 0 );
		font.setStyleHint( (QFont::StyleHint)0 );
		font.setCharSet( (QFont::CharSet)1 );
		welcome_label->setFont( font );
	}
	welcome_label->setFrameStyle( 33 );
	welcome_label->setText(i18n("Welcome to the KDevelop Setup !"));
	welcome_label->setAlignment( 292 );
	welcome_label->setMargin( -1 );

	qt_label = new QLabel( this, "Label_3" );
	qt_label->setGeometry( 40, 100, 220, 30 );
	{
		QColorGroup normal( QColor( QRgb(0) ), QColor( QRgb(128) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(16777215) ), QColor( QRgb(16777215) ) );
		QColorGroup disabled( QColor( QRgb(8421504) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(8421504) ), QColor( QRgb(12632256) ) );
		QColorGroup active( QColor( QRgb(0) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(0) ), QColor( QRgb(16777215) ) );
		QPalette palette( normal, disabled, active );
		qt_label->setPalette( palette );
	}
	{
		QFont font( "LucidaTypewriter", 12, 50, 0 );
		font.setStyleHint( (QFont::StyleHint)0 );
		font.setCharSet( (QFont::CharSet)0 );
		qt_label->setFont( font );
	}
	qt_label->setText(i18n("Qt-Documentation Path:"));
	qt_label->setAlignment( 289 );
	qt_label->setMargin( -1 );

	config->setGroup("Doc_Location");
	QString qt_doc=config->readEntry("doc_qt","");
	if(qt_doc.isEmpty())
	  qt_test=true;
	qt_edit = new QLineEdit( this, "LineEdit_1" );
	qt_edit->setGeometry( 270, 100, 160, 30 );
	qt_edit->setText( qt_doc );
	qt_edit->setMaxLength( 32767 );
	qt_edit->setEchoMode( QLineEdit::Normal );
	qt_edit->setFrame( FALSE );

	qt_button = new QPushButton( this, "PushButton_4" );
	qt_button->setGeometry( 440, 100, 40, 30 );
	qt_button->setText( "..." );
	qt_button->setAutoRepeat( FALSE );
	qt_button->setAutoResize( FALSE );
	connect( qt_button, SIGNAL(clicked()), SLOT(slotQTpressed()) );


/*	kde_label = new QLabel( this, "Label_4" );
	kde_label->setGeometry( 40, 140, 220, 30 );
	{
		QColorGroup normal( QColor( QRgb(0) ), QColor( QRgb(128) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(16777215) ), QColor( QRgb(16777215) ) );
		QColorGroup disabled( QColor( QRgb(8421504) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(8421504) ), QColor( QRgb(12632256) ) );
		QColorGroup active( QColor( QRgb(0) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(0) ), QColor( QRgb(16777215) ) );
		QPalette palette( normal, disabled, active );
		kde_label->setPalette( palette );
	}
	{
		QFont font( "LucidaTypewriter", 12, 50, 0 );
		font.setStyleHint( (QFont::StyleHint)0 );
		font.setCharSet( (QFont::CharSet)0 );
		kde_label->setFont( font );
	}
	kde_label->setText(i18n("KDE-Documentation Path:"));
	kde_label->setAlignment( 289 );
	kde_label->setMargin( -1 );

	config->setGroup("Doc_Location");
	QString kde_doc=config->readEntry("doc_kde","");
	if(kde_doc.isEmpty())
	  kde_test=true;
	kde_edit = new QLineEdit( this, "LineEdit_2" );
	kde_edit->setGeometry( 270, 140, 160, 30 );
	kde_edit->setText( kde_doc );
	kde_edit->setMaxLength( 32767 );
	kde_edit->setEchoMode( QLineEdit::Normal );
	kde_edit->setFrame( TRUE );

	kde_button = new QPushButton( this, "PushButton_5" );
	kde_button->setGeometry( 440, 140, 40, 30 );
	kde_button->setText("...");
	kde_button->setAutoRepeat( FALSE );
	kde_button->setAutoResize( FALSE );
	connect( kde_button, SIGNAL(clicked()), SLOT(slotKDEpressed()) );
*/
	hint_label = new QLabel( this, "Label_5" );
	hint_label->setGeometry( 40, 150, 440, 120 );
	{
		QColorGroup normal( QColor( QRgb(0) ), QColor( QRgb(16777215) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(0) ), QColor( QRgb(16777215) ) );
		QColorGroup disabled( QColor( QRgb(8421504) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(8421504) ), QColor( QRgb(12632256) ) );
		QColorGroup active( QColor( QRgb(0) ), QColor( QRgb(12632256) ), QColor( QRgb(16777215) ), QColor( QRgb(8421504) ), QColor( QRgb(10789024) ), QColor( QRgb(0) ), QColor( QRgb(16777215) ) );
		QPalette palette( normal, disabled, active );
		hint_label->setPalette( palette );
	}
	{
		QFont font( "LucidaTypewriter", 12, 50, 0 );
		font.setStyleHint( (QFont::StyleHint)0 );
		font.setCharSet( (QFont::CharSet)0 );
		hint_label->setFont( font );
	}
	hint_label->setFrameStyle( 33 );
	hint_label->setText(i18n("This installation program lets you set the correct path "
	                          "to your documentations and check for needed helper programs."
	                          "  Choose 'Proceed` to invoke the automatic detection and "
	                          "documentation generator. If you press the right mouse button over"
	                          " the buttons or 'Help' you will get more specific information.") );
	hint_label->setAlignment( 1316 );
	hint_label->setMargin( -1 );

	help_button = new QPushButton( this, "PushButton_6" );
	help_button->setGeometry( 50, 310, 110, 30 );
	help_button->setText(i18n("Help") );
	help_button->setAutoRepeat( FALSE );
	help_button->setAutoResize( FALSE );
	connect( help_button, SIGNAL(clicked()), SLOT(slotHelp()) );
	
	auto_button = new QPushButton( this, "PushButton_1" );
	auto_button->setGeometry( 200, 310, 120, 30 );
	auto_button->setText(i18n("Proceed >>"));
	auto_button->setAutoRepeat( false );
	auto_button->setAutoResize( FALSE );
	connect( auto_button, SIGNAL(clicked()), SLOT(slotAuto()) );

	cancel_button = new QPushButton( this, "PushButton_3" );
	cancel_button->setGeometry( 360, 310, 110, 30 );
	connect( cancel_button, SIGNAL(pressed()), SLOT(slotCancel()) );
	cancel_button->setText( "Cancel" );
//	cancel_button->setAutoRepeat( FALSE );
//	cancel_button->setAutoResize( FALSE );
  setGeometry(QApplication::desktop()->width()/2-260, QApplication::desktop()->height()/2-180, 520,360);

//	resize( 520, 360 );

}
CKDevInstall::~CKDevInstall(){
}

void CKDevInstall::slotQTpressed()
{
  QString dir;
  dir = KFileDialog::getDirectory(config->readEntry("doc_qt"));
  if (!dir.isEmpty()){
    qt_edit->setText(dir);
    config->setGroup("Doc_Location");

    QString qt_testfile=dir+"classes.html"; // test if the path really is the qt-doc path
    if(QFileInfo(qt_testfile).exists()){
      config->writeEntry("doc_qt",dir);
      qt_test=false;
    }
    else{
      KMsgBox::message(this,i18n("The selected path is not correct!"),i18n("The chosen path does not lead to the\n"
                                                              "Qt-library documentation. Please choose the\n"
                                                              "correct path."),KMsgBox::EXCLAMATION);
    }
  }
}

void CKDevInstall::slotKDEpressed()
{
  QString dir;
  dir = KFileDialog::getDirectory(config->readEntry("doc_kde"));
  if (!dir.isEmpty()){
    kde_edit->setText(dir);
    config->setGroup("Doc_Location");

    QString kde_testfile=dir+"kdecore/index.html"; // test if the path really is the kde-doc path
    if(QFileInfo(kde_testfile).exists()){
      config->writeEntry("doc_kde",dir);
      kde_test=false;
    }
    else{
      KMsgBox::message(this,i18n("The selected path is not correct!"),i18n("The chosen path does not lead to the\n"
                                                              "KDE-library documentation. Please choose 'Proceed'\n"
                                                              "in any case. If you don't have a documentation,\n"
                                                              "it will be generated automatically in one of the\n"
                                                              "next steps"),KMsgBox::EXCLAMATION);
    }
  }

}
void CKDevInstall::slotHelp()
{

  kapp->invokeHTMLHelp("kdevelop/index-3.html", "ss3.7.3" );
}


void CKDevInstall::slotAuto() // proceed >>
{
  int numProgs=100; // number of installed Programs
  bool make=false;
  bool gmake=false;
  bool autoconf=false;
  bool autoheader=false;
  bool automake=false;
  bool perl=false;
  bool sgml2html=false;
  bool ksgml2html=false;
  bool kdoc=false;
  bool glimpse=false;
  bool glimpseindex=false;
  bool kdbg=false;
  bool kiconedit=false;
  bool ktranslator=false;

  QProgressDialog progress( i18n("Checking needed programs..."), i18n("Cancel"), numProgs, this );
  progress.setMinimumDuration ( 0 );


  for (int i=0; i<numProgs; i++) {
  progress.setProgress( i );
  if ( progress.wasCancelled() )
    break;
    if(CToolClass::searchInstProgram("make")){
      make=true;
    }
  }
  progress.setProgress( numProgs );

  if(CToolClass::searchInstProgram("gmake")){
    gmake=true;
  }
  if(CToolClass::searchInstProgram("autoconf")){
    autoconf=true;
  }
  if(CToolClass::searchInstProgram("autoheader")){
    autoheader=true;
  }
  if(CToolClass::searchInstProgram("automake")){
    automake=true;
  }
  if(CToolClass::searchInstProgram("perl")){
    perl=true;
  }
  if(CToolClass::searchInstProgram("sgml2html")){
    sgml2html=true;
  }
  if(CToolClass::searchInstProgram("ksgml2html")){
    ksgml2html=true;
  }
  if(CToolClass::searchInstProgram("kdoc")){
    kdoc=true;
  }
  if(CToolClass::searchInstProgram("glimpse")){
    glimpse=true;
  }
  if(CToolClass::searchInstProgram("glimpseindex")){
    glimpseindex=true;
  }
  if(CToolClass::searchInstProgram("kdbg")){
    kdbg=true;
  }
  if(CToolClass::searchInstProgram("kiconedit")){
    kiconedit=true;
  }
  if(CToolClass::searchInstProgram("ktranslator")){
    ktranslator=true;
  }

  QString found=i18n(" was found.\n");
  QString not_found=i18n(" was not found.\n");

  QString make_str;
  if(make){
    make_str="make"+found;
    config->setGroup("General Options");
    config->writeEntry("Make","make");
  }
  else
    make_str="make"+not_found;
  QString gmake_str;
  if(gmake){
    gmake_str="gmake"+found+" -- setting make-command to gmake";
    config->setGroup("General Options");
    config->writeEntry("Make","gmake");
  }
  else
    gmake_str="gmake"+not_found;
  QString autoconf_str;
  if(autoconf)
    autoconf_str="autoconf"+found;
  else
    autoconf_str="autoconf"+not_found;
  QString autoheader_str;
  if(autoheader)
    autoheader_str="autoheader"+found;
  else
    autoheader_str="autoheader"+not_found;
  QString automake_str;
  if(automake)
    automake_str="automake"+found;
  else
    automake_str="automake"+not_found;
  QString sgml2html_str;
  if(sgml2html)
    sgml2html_str="sgml2html"+found;
  else
    sgml2html_str="sgml2html"+not_found+" -- generating application handbooks will not be possible";
  QString kdoc_str;
  if(kdoc)
    kdoc_str="kdoc"+found;
  else
    kdoc_str="kdoc"+not_found+" -- generating API-documentation will not be possible";
  QString kdbg_str;
  if(kdbg)
    kdbg_str="kdbg"+found;
  else
    kdbg_str="kdbg"+not_found+" -- debugging within KDevelop will not be possible";
  QString kiconedit_str;
  if(kiconedit)
    kiconedit_str="KIconedit"+found;
  else
    kiconedit_str="KIconedit"+not_found+" -- editing icons will not be possible";
  QString ktranslator_str;
  if(ktranslator)
    ktranslator_str="KTranslator"+found;
  else
    ktranslator_str="KTranslator"+not_found;
  QString glimpse_str;
  if(glimpse)
    glimpse_str="Glimpse"+found;
  else
    glimpse_str="Glimpse"+not_found+" -- search functionality will not be provided";
  QString glimpseindex_str;
  if(glimpseindex)
    glimpseindex_str="Glimpseindex"+found;
  else
    glimpseindex_str="Glimpseindex"+not_found+" -- search functionality will not be provided";
  QString perl_str;
  if(perl)
    perl_str="Perl"+found;
  else
    perl_str="Perl"+not_found+" -- generation of new frame applications will not be possible";

  KMsgBox::message(this, i18n("Program test results"),i18n("The following results have been determined for your system:\n\n")
                  +make_str+gmake_str+autoconf_str+autoheader_str+automake_str+perl_str+sgml2html_str+kdoc_str+glimpse_str+glimpseindex_str
                  +kdbg_str+kiconedit_str+ktranslator_str, KMsgBox::INFORMATION);

  // now check for the qt libs documentation
  QString qt_testfile="classes.html";
  QString qt="";

  QDir* qt_dir=new QDir();
  if(qt_dir->cd("/usr/local/qt/html/"))
    if(qt_dir->exists("classes.html"))
      qt="/usr/lib/qt/html/";

  if(qt_dir->cd("/usr/local/lib/qt/html/"))
    if(qt_dir->exists("classes.html"))
      qt="/usr/local/lib/qt/html/";

  if(qt_dir->cd("/usr/lib/qt/html/"))
    if(qt_dir->exists("classes.html"))
      qt="/usr/lib/qt/html/";

  if(qt_dir->cd("/usr/lib/qt/doc/html/"))
    if(qt_dir->exists("classes.html"))
      qt="/usr/lib/qt/doc/html/";

  if(qt_dir->cd("/usr/X11/lib/qt/html/"))
    if(qt_dir->exists("classes.html"))
      qt="/usr/X11/lib/qt/html/";

  if(!qt.isEmpty())
    qt_test=false;

  if(!qt_test){
    config->setGroup("Doc_Location");
    config->writeEntry("doc_qt",qt);
    KMsgBox::message(this, i18n("Qt Documentation found"),i18n("The Qt-Documentation has been found in:\n\n"+
                          qt+"\n\nThe correct path has been set."),KMsgBox::INFORMATION);
  }
  else{  // return to the setup to set it manually ?
    int result=KMsgBox::yesNo(this,i18n("Information"),i18n("The Qt-library documentation could not\n"
                                                  "be detected. Please insert the correct path\n"
                                                  "to your Qt-documentation manually. Do you want\n"
                                                  "to set it now ?"),KMsgBox::QUESTION);
    if(result==1)
      return;
  }

  KShellProcess* shell_process=new KShellProcess();
  QDir* kde_dir=new QDir();

  if(!kdoc && !perl)
    KMsgBox::message(this,i18n("Information"),i18n("The Program KDoc was not found on your system,\n"
                                                "a library documentation update can not be performed.\n"
                                                "KDoc is part of the kdesdk package that can be obtained\n"
                                                "from www.kde.org."),KMsgBox::INFORMATION);
  else{
    KMsgBox::message(this,i18n("Information"), i18n("Now KDevelop will create a new KDE-library\n"
                                                    "documentation. For that, you need the kdelibs\n"
                                                    "package as the source package. If you don't have\n"
                                                    "the kdelibs as sources, we advise to obtain them\n"
                                                    "from www.kde.org. Mind that the sources should match\n"
                                                    "your installed kdelibs version.\n\nATTENTION: IF YOU PRESS `OK`\n"
                                                    "ON THE NEXT DIALOG, THE DOCUMENTATION WILL BE GENERATED\n"
                                                    "BUT NO INDICATOR SHOWS THAT IT IS READY. PLEASE CHECK THE\n"
                                                    "DOCUMENTATION FILES. THOSE SHOULD BE GENERATED WITHIN\n"
                                                    "2 MINUTES !!!"), KMsgBox::INFORMATION);

    kde_dir->setCurrent(QDir::homeDirPath ());
    kde_dir->mkdir(".kde",false);
    kde_dir->mkdir(".kde/share",false);
    kde_dir->mkdir(".kde/share/apps",false);
    kde_dir->mkdir(".kde/share/apps/kdevelop",false);
    kde_dir->mkdir(".kde/share/apps/kdevelop/KDE-Documentation",false);
    config->setGroup("Doc_Location");
    config->writeEntry("doc_kde",QDir::homeDirPath ()+"/.kde/share/apps/kdevelop/KDE-Documentation/");
    CUpdateKDEDocDlg dlg(this,"test",shell_process, config);
    dlg.exec();

  }
  if(!glimpse && !glimpseindex){
    KMsgBox::message(this,i18n("Information"), i18n("The program glimpse is not installed,\n"
                                                    "therefore KDevelop can not index your\n"
                                                    "documentation to provide a proper help\n"
                                                    "functionality. We advise to install glimpse\n"
                                                    "and create the searchdatabase with the KDevelop\n"
                                                    "Setup in the Options menu later.\n\n"
                                                    "As this is the last step of the Installation\n"
                                                    "process, KDevelop will be started now.\n\n"
                                                    "We hope you enjoy KDevelop and that it is a useful\n"
                                                    "help to create new software.\n\nThe KDevelop Team"),KMsgBox::INFORMATION);

    config->setGroup("General Options");
    config->writeEntry("Install",false);
    accept();
  }
  else{
    KMsgBox::message(this,i18n("Information"),i18n("Now KDevelop will perform the last step\n"
                                                  "of the installation process with indexing\n"
                                                  "your documentation. This will provide an extended\n"
                                                  "help functionality and will give you the information\n"
                                                  "you need.\n\nATTENTION: PLEASE WAIT UP TO ONE MINUTE\n"
                                                  "AFTER PRESSING 'OK' ON THE NEXT DIALOG TO ENSURE THE\n"
                                                  "INDEX HAS BEEN CREATED !!!"), KMsgBox::INFORMATION);

    CCreateDocDatabaseDlg dlg(this,"DLG",shell_process,config);
    dlg.exec();
  }

  KMsgBox::message(this,i18n("Installation successful !"), i18n("The installation process finished successfully.\n"
                                                          "The KDevelop Team wishes that you will enjoy our\n"
                                                          "program and we would be honoured for any feedback.\n\n"
                                                          "The KDevelop Team"), KMsgBox::INFORMATION);

  config->setGroup("General Options");
  config->writeEntry("Install",false);

  accept();
}
void CKDevInstall::slotCancel()
{
  config->setGroup("General Options");
  config->writeEntry("Install",false);
  close();
}






































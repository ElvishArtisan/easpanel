// directfilewidget.cpp
//
// Direct File Importer Subsystem
//
//   (C) Copyright 2026 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <syslog.h>

#include <QDateTime>
#include <QMessageBox>

#include "directfilewidget.h"

DirectFileWidget::DirectFileWidget(QUdpSocket *rml_sock,Config *c,QWidget *parent)
  : QFrame(parent)
{
  setFrameStyle(QFrame::Panel|QFrame::Sunken);

  //
  // Fonts
  //
  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  d_config=c;

  d_mode_button_mapper=new QSignalMapper(this);
  connect(d_mode_button_mapper,SIGNAL(mapped(int)),this,SLOT(autoData(int)));

  for(int i=0;i<d_config->directFileQuantity();i++) {
    //
    // File Detectors
    //
    d_detectors.push_back(new FileDetector(i,rml_sock,d_config,this));
    connect(d_detectors.back(),SIGNAL(eventStarted(int,FileInfo *)),
	    this,SLOT(eventStartedData(int,FileInfo *)));
    connect(d_detectors.back(),SIGNAL(eventStopped(int)),
	    this,SLOT(eventStoppedData(int)));
    connect(d_detectors.back(),SIGNAL(scanningStarted(int)),
	    this,SLOT(scanningStartedData(int)));
    connect(d_detectors.back(),SIGNAL(scanningStopped(int)),
	    this,SLOT(scanningStoppedData(int)));
    if(d_detectors.back()->setPath(d_config->directFilePath(i))) {
      syslog(LOG_DEBUG,"DirectFile%d set path for %s",
	     1+i,d_config->directFilePath(i).toUtf8().constData());
    }
    else {
      QMessageBox::critical(this,"Eas Panel",
			    tr("The directory")+" \""+
			    d_detectors.back()->path()+"\" "+
			    tr("does not exist for the")+
			    QString::asprintf(" DirectFile%d ",1+i)+
			    tr("processor."));
      syslog(LOG_ERR,"DirectFile%d failed to set path %s",
	     1+i,d_config->directFilePath(i).toUtf8().constData());
      exit(1);
    }
    if(!d_config->directFileBackupDirectory(i).isEmpty()) {
      if(d_detectors.back()->
	 setBackupDirectory(d_config->directFileBackupDirectory(i))) {
	syslog(LOG_DEBUG,"DirectFile%d set backup directory for %s",
	       1+i,d_config->directFileBackupDirectory(i).toUtf8().constData());
      }
      else {
	QMessageBox::critical(this,"Eas Panel",
			      tr("The backup directory")+" \""+
			      d_detectors.back()->backupDirectory()+"\" "+
			      tr("does not exist for the")+
			      QString::asprintf(" DirectFile%d ",1+i)+
			      tr("processor."));
	syslog(LOG_ERR,"DirectFile%d failed to set backup directory %s",
	       1+i,d_config->directFileBackupDirectory(i).toUtf8().constData());
	exit(1);
      }
    }

    //
    // UI Elements
    //
    d_autos.push_back(false);
    d_mode_buttons.push_back(new ModeButton(tr("Paused"),this));
    d_mode_buttons.back()->setFont(bold_font);
    d_mode_buttons.back()->setStyleSheet("background-color: #FF0000");
    d_mode_button_mapper->setMapping(d_mode_buttons.back(),i);
    connect(d_mode_buttons.back(),SIGNAL(clicked()),
    	    d_mode_button_mapper,SLOT(map()));
    connect(d_mode_buttons.back(),SIGNAL(quitRequested()),
	    this,SLOT(quitRequestedData()));

    d_description_labels.
      push_back(new QLabel(d_config->directFileDescription(i),this));
    d_description_labels.back()->setFont(bold_font);
    //  d_description_labels.back()->setStyleSheet("background-color: #FF0000");

    d_datetime_labels.push_back(new QLabel(this));
    //  d_datetime_labels.back()->setStyleSheet("background-color: #FFFF00");

    SetAutomaticMode(i);
  }
}


QSize DirectFileWidget::sizeHint() const
{
  return QSize(600-130,26*d_detectors.size());
}


void DirectFileWidget::playedCart(unsigned cartnum)
{
  for(int i=0;i<d_detectors.size();i++) {
    d_detectors.at(i)->playedCart(cartnum);
  }
}


void DirectFileWidget::cleanPaths() const
{
  for(int i=0;i<d_detectors.size();i++) {
    d_detectors.at(i)->cleanPath();
  }
}


void DirectFileWidget::setAutomaticMode(int id)
{
  for(int i=0;i<d_detectors.size();i++) {
    SetAutomaticMode(i);
  }
}


void DirectFileWidget::setPausedMode(int id)
{
  for(int i=0;i<d_detectors.size();i++) {
    SetPausedMode(i);
  }
}


void DirectFileWidget::autoData(int id)
{
  if(d_autos.at(id)) {
    SetPausedMode(id);
  }
  else {
    SetAutomaticMode(id);
  }
}


void DirectFileWidget::eventStartedData(int id,FileInfo *info)
{
  emit raiseRequested();
  d_description_labels.at(id)->setStyleSheet("background-color: #00FF00");
  d_datetime_labels.at(id)->setStyleSheet("background-color: #00FF00");
  d_datetime_labels.at(id)->setText(info->fileInfo().lastModified().
				    toString("ddd, MMM d yyyy @ hh:mm:ss"));
}


void DirectFileWidget::eventStoppedData(int id)
{
  d_description_labels.at(id)->setStyleSheet("");
  d_datetime_labels.at(id)->setStyleSheet("");
  d_datetime_labels.at(id)->clear();
}


void DirectFileWidget::scanningStartedData(int id)
{
  d_description_labels.at(id)->setEnabled(true);
}


void DirectFileWidget::scanningStoppedData(int id)
{
  d_description_labels.at(id)->setDisabled(true);
}


void DirectFileWidget::quitRequestedData()
{
  emit quitRequested();
}


void DirectFileWidget::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  //  int h=size().height();

  for(int i=0;i<d_description_labels.size();i++) {
    d_mode_buttons.at(i)->setGeometry(10,3+i*26,60,20);
    d_description_labels.at(i)->setGeometry(80,3+29*i,w-270,20);
    d_datetime_labels.at(i)->setGeometry(w-190,3+29*i,190,20);
  }
}


void DirectFileWidget::SetPausedMode(int id)
{
  d_mode_buttons.at(id)->setText(tr("Paused"));
  d_mode_buttons.at(id)->
    setStyleSheet("background-color: #FF0000;color=#FFFFFF");
  d_detectors.at(id)->stopScanning();
  d_autos[id]=false;
}


void DirectFileWidget::SetAutomaticMode(int id)
{
  d_mode_buttons.at(id)->setText(tr("Auto"));
  d_mode_buttons.at(id)->setStyleSheet("background-color: #00FF00");
  d_detectors.at(id)->startScanning();  
  d_autos[id]=true;

}

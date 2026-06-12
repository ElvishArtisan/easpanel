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
  d_on_button_mapper=new QSignalMapper(this);
  connect(d_on_button_mapper,SIGNAL(mapped(int)),
	  this,SLOT(onButtonData(int)));
  d_off_button_mapper=new QSignalMapper(this);
  connect(d_off_button_mapper,SIGNAL(mapped(int)),
	  this,SLOT(offButtonData(int)));

  for(int i=0;i<d_config->directFileQuantity();i++) {
    //
    // File Detectors
    //
    d_detectors.push_back(new FileDetector(i,rml_sock,d_config,this));
    connect(d_detectors.back(),SIGNAL(eventStarted(int)),
	    this,SLOT(eventStartedData(int)));
    connect(d_detectors.back(),SIGNAL(eventStopped(int)),
	    this,SLOT(eventStoppedData(int)));
    connect(d_detectors.back(),SIGNAL(scanningStarted(int)),
	    this,SLOT(scanningStartedData(int)));
    connect(d_detectors.back(),SIGNAL(scanningStopped(int)),
	    this,SLOT(scanningStoppedData(int)));
    /*
    connect(d_detectors.back(),SIGNAL(fileAdded(int,const QString &)),
	    this,SLOT(detectorFileAddedData(int,const QString &)));
    connect(d_detectors.back(),SIGNAL(fileRemoved(int,const QString &)),
	    this,SLOT(detectorFileRemovedData(int,const QString &)));
    */
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

    //
    // UI Elements
    //
    d_on_buttons.push_back(new QPushButton(tr("On"),this));
    d_on_buttons.back()->setFont(bold_font);
    d_on_button_mapper->setMapping(d_on_buttons.back(),i);
    connect(d_on_buttons.back(),SIGNAL(clicked()),
    	    d_on_button_mapper,SLOT(map()));
    d_off_buttons.push_back(new QPushButton(tr("Off"),this));
    d_off_buttons.back()->setFont(bold_font);
    d_off_button_mapper->setMapping(d_off_buttons.back(),i);
    connect(d_off_buttons.back(),SIGNAL(clicked()),
    	    d_off_button_mapper,SLOT(map()));

    d_description_labels.
      push_back(new QLabel(d_config->directFileDescription(i),this));
    d_description_labels.back()->setFont(bold_font);
    //    d_description_labels.back()->setStyleSheet("background-color: #00FF00");
  }
}


QSize DirectFileWidget::sizeHint() const
{
  return QSize(1020/3,26*d_detectors.size());
}


void DirectFileWidget::playedCart(unsigned cartnum)
{
  for(int i=0;i<d_detectors.size();i++) {
    d_detectors.at(i)->playedCart(cartnum);
  }
}


void DirectFileWidget::onButtonData(int id)
{
  d_detectors.at(id)->startScanning();
}


void DirectFileWidget::offButtonData(int id)
{
  d_detectors.at(id)->stopScanning();
}


void DirectFileWidget::eventStartedData(int id)
{
  d_description_labels.at(id)->setStyleSheet("background-color: #00FF00");
}


void DirectFileWidget::eventStoppedData(int id)
{
  d_description_labels.at(id)->setStyleSheet("");
}


void DirectFileWidget::scanningStartedData(int id)
{
  d_description_labels.at(id)->setEnabled(true);
}


void DirectFileWidget::scanningStoppedData(int id)
{
  d_description_labels.at(id)->setDisabled(true);
}


void DirectFileWidget::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  //  int h=size().height();

  for(int i=0;i<d_description_labels.size();i++) {
    d_description_labels.at(i)->setGeometry(10,3+26*i,w-135,20);
    d_on_buttons.at(i)->setGeometry(w-120,3+i*26,50,20);
    d_off_buttons.at(i)->setGeometry(w-60,3+i*26,50,20);
  }
}

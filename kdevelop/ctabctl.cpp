/***************************************************************************
                     ctabctl.cpp - a ktabctl class with a setCurrentTab   
                             -------------------                                         

    version              :                                   
    begin                : 22 Jul 1998                                        
    copyright            : (C) 1998 by Sandy Meier                         
    email                : smeier@rz.uni-potsdam.de                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/
#include "ctabctl.h"

CTabCtl::CTabCtl(QWidget* parent,char* name) : KTabCtl(parent,name){
  setFocusPolicy(QWidget::NoFocus);
//tabs->setFocusPolicy(QWidget::NoFocus);
  setShape(QTabBar::RoundedAbove);
  
}
void CTabCtl::setCurrentTab(int id){

  tabs->setCurrentTab(id);
//  tabSelected(id);
}

int CTabCtl::getCurrentTab(){
  int currentTab=tabs->currentTab();
  return currentTab;
}









/***************************************************************************
 *   Copyright (C) 2007 by Jesper Thomschutz <jesperht@yahoo.com>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "fifteen.h"
#include "piece.h"

#include <ctime>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItemAnimation>
#include <QTimeLine>

#define SIZE 48


Fifteen::Fifteen(QGraphicsItem *parent) 
	: QGraphicsRectItem(parent)
{
  m_pixmaps.resize(16);
  m_pieces.resize(16);
  m_splitPixmap = false;
  m_pixmap = QPixmap(":/images/greensquare.svg");
  m_numerals = true;

  shuffle();
}

void Fifteen::clearPieces()
{
  for (int i = 0; i < 16; ++i)
    delete m_pieces[i];
}

void Fifteen::shuffle()
{
  bool solvable = false;
  qsrand(time(0));
  while (!solvable)
  {
    clearPieces();
    m_pieces.fill(NULL);
    for (int i=0; i<16; ++i)
    {
      int rand = qrand() % 16;
     
      while (m_pieces.at(rand) != NULL)
        rand = qrand() % 16;
      
        m_pieces[rand] = new Piece(SIZE, i, this);
        m_pieces[rand]->hide();
        QObject::connect(m_pieces[rand], SIGNAL(pressed(QGraphicsItem*)), this, SLOT(piecePressed(QGraphicsItem*))); 
      
        if (i == 0)
          m_blank = m_pieces[rand];
    }
    solvable = isSolvable();
  }
  updatePixmaps();
  updateNumerals();
  drawPieces();
}

bool Fifteen::isSolvable()
{
  int fields[16];
  int odd_even_solvable;
  for (int i=0;  i<16; ++i)
  {
    fields[i] = m_pieces[i]->getId();
    if (fields[i] == 0)
    {
      fields[i] = 16;
      switch (i)
      {
        case 0: case  2: case  5: case  7: odd_even_solvable = 1; break;
        case 8: case 10: case 13: case 15: odd_even_solvable = 1; break;
        case 1: case  3: case  4: case  6: odd_even_solvable = -1; break;
        case 9: case 11: case 12: case 14: odd_even_solvable = -1; break;
      }
    }
  }

  signed int odd_even_permutations = 1;
  for (int i=0; i<16; ++i)
  {
    int field = fields[i];
    while (field != i+1)
    {
      int temp_field = fields[field-1];
      fields[field-1] = field;
      field = temp_field;
      odd_even_permutations = odd_even_permutations * -1;
    }
  }

  return odd_even_solvable == odd_even_permutations;
}

void Fifteen::updateNumerals()
{
  for (int i = 0; i < 16; ++i)
    m_pieces[i]->showNumeral(m_numerals);

  update(boundingRect());
}

void Fifteen::setNumerals(bool show)
{
  m_numerals = show;
  updateNumerals(); 
}

void Fifteen::setSplitPixmap(QString path)
{
  m_pixmap = QPixmap(path);
  m_splitPixmap = true;
  updatePixmaps();
}

void Fifteen::setIdentical()
{
  m_splitPixmap = false;
  m_pixmap = QPixmap(":/images/greensquare.svg");
  updatePixmaps();
}

void Fifteen::updatePixmaps()
{
  QPixmap pixmap;

  if (!m_splitPixmap)
  {
    pixmap = m_pixmap.scaled(SIZE, SIZE);
    m_pixmaps.fill(pixmap);
  }
  else
  {
    pixmap = m_pixmap.scaled(SIZE*4, SIZE*4);
    int x = 0;
    int y = 0;
    
    for (int i=1; i < 16; ++i)
    {
      if (i%4 == 0 && i != 0)
      {
        x = 0;
        y = y + SIZE;
      }
      m_pixmaps[i] = pixmap.copy(x, y, SIZE, SIZE);
      x += SIZE;
    }
  }

  for (int i=0;i<16;++i)
    m_pieces[i]->setPixmap(m_pixmaps[m_pieces[i]->getId()]);
}

void Fifteen::piecePressed(QGraphicsItem *item)
{
  if (isAdjacent(item, m_blank))
  {
    QPointF pos = item->pos();

    QTimeLine *timer = new QTimeLine(170);
    
    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
    animation->setItem(item);
    animation->setTimeLine(timer);

    animation->setPosAt(0.0, item->pos());
    animation->setPosAt(1.00, m_blank->pos());

    timer->start();

    m_blank->setPos(pos);
   }
}

bool Fifteen::isAdjacent(QGraphicsItem *a, QGraphicsItem *b)
{  
  qreal ax = a->pos().x();
  qreal ay = a->pos().y();
  
  qreal bx = b->pos().x();
  qreal by = b->pos().y();
  
  /*
  qDebug() << "ax:" << ax << "ay:" << ay; 
  qDebug() << "bx:" << bx << "by:" << by;
  */

  //Left
  if (ax + SIZE == bx && ay == by)
    return true;
  //Right
  if (ax - SIZE == bx && ay == by)
    return true;
  //Above
  if (ay + SIZE == by && ax == bx)
    return true;
  //Below
  if (ay - SIZE == by && ax == bx)
    return true;

  return false;
}

void Fifteen::drawPieces()
{
  int x = 0;
  int y = 0;

  for (int i=0; i < 16; ++i)
  {
    if (i%4 == 0 && i != 0)
    {
      x = 0;
      y = y + SIZE;
    }
    
    m_pieces.at(i)->setPos(x,y);
    m_pieces.at(i)->show();
    x += SIZE;
  }
}

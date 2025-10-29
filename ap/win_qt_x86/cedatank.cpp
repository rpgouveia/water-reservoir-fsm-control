#include "cedaTank.h"
#include <QPainter>
#include <QDebug>
#include <QPaintEvent>
#include <QPainterPath>


cedaTank::cedaTank(QWidget *parent) : QToolButton(parent)
{
    Deger1Suffix = "kg";
    Deger2Suffix = "cm";
  NameFont.setFamily("Arial");
  NameFont.setBold(true);
  NameFont.setPointSize(12);
}

void cedaTank::paintEvent(QPaintEvent *)
{

  QPainter myPainter(this);
  myPainter.setRenderHint(QPainter::Antialiasing);

  QPen BorderPen(outColor);
  QBrush inlineBrush(inColor);
  //Dış Çerçeve
  BorderPen.setWidth(PenWidth);
  myPainter.setPen(BorderPen);
  myPainter.setBrush(inlineBrush);
  Calculate_outlineRect();
  myPainter.drawRoundedRect(outlineRect, 25.0, 25.0);
  QPainterPath painterPath;
  painterPath.addRoundedRect(outlineRect,25,25);
  myPainter.setClipPath(painterPath);

  //İç Dolum
  BorderPen.setColor(outColor);
  BorderPen.setWidth(0);
  myPainter.setPen(BorderPen);

  inlineBrush.setColor(EmptyColor);
  myPainter.setBrush(EmptyColor);
  Calculate_inlineRect();
  myPainter.drawRect(inlineRect);

  //myPainter.fillRect(makeRect,EmptyColor);
  //iç Yazı
  BorderPen.setColor(Qt::black);
  myPainter.setPen(BorderPen);
  myPainter.setFont(this->font());
  if (ShowDeger1)
      myPainter.drawText(Deger1Rect,Qt::AlignCenter,QString(QString::number(w_value) + " %1").arg(Deger1Suffix));
  if (ShowDeger2)
      myPainter.drawText(Deger2Rect,Qt::AlignCenter,QString(QString::number(Deger2) + " %1").arg(Deger2Suffix));
  Deger2Rect =  QRectF(outlineRect.x(),
                       outlineRect.y() + PenWidth,
                       outlineRect.width() ,
                       this->fontMetrics().height() + 5);
  BorderPen.setColor(Qt::red);
  myPainter.setPen(BorderPen);
  myPainter.setFont(NameFont);
  myPainter.drawText(Deger2Rect,Qt::AlignCenter,text());
  myPainter.setClipping(false);
  myPainter.end();
}

void cedaTank::setValue(int value)
{
    if (w_value != value){
        if (value <= MaxValue){
            w_value = value;
            emit valuechanged(w_value);
        } else {
            w_value = MaxValue;
            emit valuechanged(w_value);
        }

    }
    repaint();
}

int cedaTank::value(){
    return w_value;
}

void cedaTank::setText(QString s){
    QToolButton::setText(s);
}

void cedaTank::Calculate_inlineRect()
{

    qreal carpan,Yukseklik;
//    qreal theYOffset,Genislik;
    carpan = qreal(w_value - MinValue) / qreal(MaxValue - MinValue);
//    theYOffset = outlineRect.y() + (outlineRect.height() - (carpan * outlineRect.height())) + qreal(PenWidth/2);
//    Genislik = outlineRect.width() - qreal(PenWidth);
    Yukseklik = (outlineRect.height() - (carpan * outlineRect.height()));

    inlineRect =  QRectF(outlineRect.x() , outlineRect.y() ,outlineRect.width(),Yukseklik );

//    qDebug() << "outlineRect.x() :" << outlineRect.x() << "outlineRect.y() :" << outlineRect.y() << "outlineRect.width() :" << outlineRect.width() << "outlineRect.height() :" << outlineRect.height();
//    qDebug() << "inlineRect.x() :" << inlineRect.x() << "inlineRect.y() :" << inlineRect.y() << "inlineRect.width() :" << inlineRect.width() << "inlineRect.height() :" << inlineRect.height();

}

void cedaTank::Calculate_outlineRect()
{
    outlineRect = QRectF(Padding,Padding,this->width() - (Padding * 2 ) , this->height()- (Padding * 2 ));
    Deger1Rect = QRectF(outlineRect.x(),
                        outlineRect.y() + (outlineRect.height() - (outlineRect.height() /4)),
                        outlineRect.width() ,
                        this->fontMetrics().height() + 5);
    Deger2Rect = QRectF(outlineRect.x(),
                        outlineRect.y() + (outlineRect.height() /4),
                        outlineRect.width() ,
                        this->fontMetrics().height() + 5);
}


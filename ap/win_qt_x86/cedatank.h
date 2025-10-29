#ifndef CEDATANK_H
#define CEDATANK_H

#include <QToolButton>

class cedaTank : public QToolButton
{
    Q_OBJECT
public:
    explicit cedaTank(QWidget *parent = 0);

    int MaxValue = 100;
    int MinValue = 0;
    int Padding = 1;
    QString Deger1Suffix;
    QString Deger2Suffix;
    bool ShowDeger1 = false;
    bool ShowDeger2 = false;
    int Deger2 = 0;
    void setText(QString s);

protected:
    void paintEvent(QPaintEvent *);
signals:
    void valuechanged(int value);

public slots:
    void setValue(int value);
    int value();
private:
   int w_value = 24;
   QRectF outlineRect;
   QRectF inlineRect;

   QRectF Deger1Rect;
   QRectF Deger2Rect;

   int PenWidth = 5;
   QColor outColor = Qt::darkGray;
   QColor inColor = Qt::blue;
   QColor EmptyColor = Qt::gray;
   QFont NameFont;
   void Calculate_inlineRect();
   void Calculate_outlineRect();
};


#endif // CEDATANK_H

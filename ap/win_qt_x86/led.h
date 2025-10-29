#ifndef LED_H
#define LED_H

#include <QToolButton>
#include <QPainter>

class Led : public QToolButton{
    Q_OBJECT
    Q_PROPERTY(bool power READ power WRITE setPower NOTIFY powerChanged)
    Led(const Led&)=delete;
    Led& operator=(const Led&)=delete;
public:
    explicit Led(QWidget* parent=nullptr)
        :QToolButton(parent)
        , m_power(false)
    {}
    bool power() const
    {
        return m_power;
    }
    void setText(QString){
    }
    void setColor(QColor c){
        color = c;
    }
public slots:
    void setPower(bool power)
    {
        if(power!=m_power){
            m_power = power;
            emit powerChanged();
            update();
        }
    }

signals:
    void powerChanged();
protected:
    QColor color = Qt::red;
    virtual void paintEvent(QPaintEvent *event) override{
        Q_UNUSED(event)
        QPainter ledPainter(this);
        ledPainter.setPen(Qt::black);
        if(m_power)
            ledPainter.setBrush(color);
        else
            ledPainter.setBrush(Qt::NoBrush);
//        ledPainter.drawEllipse(rect());
            ledPainter.drawEllipse(QRect(1,1,width()-2,height()-2));
    }
private:
    bool m_power;
};
#endif // LED_H

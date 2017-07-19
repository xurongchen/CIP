#include "selleroper.h"
#include "ui_selleroper.h"
#include "dboperation.h"
#include <QString>
#include <string>
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include <QMessageBox>


SellerOper::SellerOper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SellerOper)
{
    ui->setupUi(this);
    ui->tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px solid #e8f3f9;background:  transparent; }"); //����tab����͸��
    ui->tabWidget->findChildren<QTabBar*>().at(0)->hide(); //tab����������
    flag = 0;
    flag2 = 0;
    PolicyId = -1;
    this->setFixedSize(720,445); //���ù̶���С
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint); //���ر���������С���ɼ�

    //Բ��
    QBitmap objBitmap(size());
    QPainter painter(&objBitmap);
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(this->rect(),11,11);
    setMask(objBitmap);

    //���ӹرհ�ť�͹رն���
    connect(ui->PBClose, SIGNAL(clicked()), this, SLOT(closeWidget()));

    ui->DEStart->setMinimumDate(QDate::currentDate().addDays(0));
    ui->DEStart->setMaximumDate(QDate::currentDate().addDays(365));  // +365��
    ui->DEStart->setCalendarPopup(true);  // ��������
    //ui->DEStart->setCalendarWidget();

    ui->tabWidget->setCurrentIndex(0);

    QRegExp regExp_ID_Card("^((1[1-5])|(2[1-3])|(3[1-7])|(4[1-6])|(5[0-4])|(6[1-5])|71|(8[12])|91)\\d{4}((19\\d{2}(0[13-9]|1[012])(0[1-9]|[12]\\d|30))|(19\\d{2}(0[13578]|1[02])31)|(19\\d{2}02(0[1-9]|1\\d|2[0-8]))|(19([13579][26]|[2468][048]|0[48])0229))\\d{3}(\\d|X|x)?$");
    ui->LECard->setValidator(new QRegExpValidator(regExp_ID_Card,this));

    QRegExp regExp_Car_Number(QString::fromLocal8Bit("^[�����弽ԥ���ɺ�����³������Ӷ���ʽ����¼���������ش�����ʹ��A-Z]{1}[A-Z]{1}[A-Z0-9]{4}[A-Z0-9��ѧ���۰�]{1}$"));
    ui->LENum->setValidator(new QRegExpValidator(regExp_Car_Number,this));

    ui->SBPrice->setRange(5000, 50000000);  // ��Χ
    ui->SBPrice->setSingleStep(100000); // ����
    ui->SBPrice->setValue(100000);  // ��ǰֵ
    ui->SBPrice->setPrefix(QString::fromLocal8Bit("�� "));  // ǰ׺
    ui->SBPrice->setSuffix(QString::fromLocal8Bit(" Ԫ"));  // ��׺

    ui->DSBDiscount->setRange(0, 1); // ��Χ
    ui->DSBDiscount->setDecimals(3);  // ����
    ui->DSBDiscount->setSingleStep(0.01); // ����
    ui->DSBDiscount->setSuffix(QString::fromLocal8Bit(" %"));
    ui->DSBDiscount->setValue(0);  // ��ǰֵ
    ui->DSBDiscount->setSpecialValueText(QString::fromLocal8Bit("���ۿ�"));  // �����ı�ֵ



}

SellerOper::~SellerOper()
{
    delete ui;
    if(PolicyId!=-1)
    {
        Policy p(PolicyId);
        p.del();
    }
}

void SellerOper::mousePressEvent(QMouseEvent *e) //���������
{
    judge.setX(0), judge.setY(0);
    judge = e->pos();
    if(judge.y() > 45 || (judge.x() == 0 && judge.y() == 0))
    {
        flag = 0;
        return ;
    }
    flag = 1;
    last = e->globalPos();
    //qDebug() << judge.rx() << ".." << judge.ry();
}
void SellerOper::mouseMoveEvent(QMouseEvent *e) //�����������ƶ�
{
    if(flag == 0) return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    //qDebug() << last.rx() << ".." << last.ry();
    move(x()+dx, y()+dy);
}
void SellerOper::mouseReleaseEvent(QMouseEvent *e) //����ͷ�
{
    judge.setX(0), judge.setY(0);
    flag = 0;
}

bool SellerOper::closeWidget()
{
   //���涯�����ı�͸���ȵķ�ʽ��ʧ1 - 0����
   QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
   animation->setDuration(550);
   animation->setStartValue(1);
   animation->setEndValue(0);
   animation->start();
   connect(animation, SIGNAL(finished()), this, SLOT(close()));

   return true;
}

void SellerOper::closeEvent(QCloseEvent *)
{
    //�˳�ϵͳ
    if(flag2 != 1)
    {
        qDebug() << "exit";
        QApplication::quit();
    }
}

void SellerOper::on_PBMin_clicked()
{
    this->showMinimized();
}

void SellerOper::on_PBReturn_clicked()
{
    flag2 = 1;
    this->close();
}

void SellerOper::on_PBWork_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void SellerOper::on_PBMessage_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void SellerOper::on_PBSend1_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void SellerOper::on_PBSend1_2_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void SellerOper::on_PBSend1_3_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void SellerOper::on_PBMy1_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void SellerOper::on_PBMy1_2_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void SellerOper::on_PBMy1_3_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void SellerOper::on_PBGet1_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void SellerOper::on_PBGet1_2_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void SellerOper::on_PBGet1_3_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void SellerOper::on_PBA1_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void SellerOper::on_PBB1_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void SellerOper::on_PBC1_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void SellerOper::on_PBA2_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void SellerOper::on_PBB2_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void SellerOper::on_PBC2_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void SellerOper::on_PBA3_clicked()
{
    Policy p(ui->LEPolicynum->text(),
             ui->LEName->text(),
             ui->LECard->text(),
             ui->LENum->text(),
             ui->LEStyle->text(),
             int(ui->SBPrice->value()),
             ui->DSBDiscount->value(),
             ui->DEStart->date());
    if(PolicyId!=-1)
    {
        Policy pdel(PolicyId);
        pdel.del();
    }
    int message = p.add(PolicyId);
    if(message==POLICY_ADD_ERROR_IMPORTANT_INFO_EMPTY)
    {
        QMessageBox::critical(0, "ERROR",
                    QString("SOMETHING IS NULL!"), QMessageBox::Cancel);
        ui->tabWidget->setCurrentIndex(0);
    }
    if(message==POLICY_ADD_ERROR_SAME_POLICYID)
    {
        QMessageBox::critical(0, "ERROR",
                    QString("POLICYID HAS USED!"), QMessageBox::Cancel);
        ui->tabWidget->setCurrentIndex(4);
    }
    if(message==POLICY_ADD_SUCCESS)
    {
        ui->tabWidget->setCurrentIndex(5);
    }
}

void SellerOper::on_PBB3_clicked()
{
    Policy p(ui->LEPolicynum->text(),
             ui->LEName->text(),
             ui->LECard->text(),
             ui->LENum->text(),
             ui->LEStyle->text(),
             int(ui->SBPrice->value()),
             ui->DSBDiscount->value(),
             ui->DEStart->date());
    if(PolicyId!=-1)
    {
        Policy pdel(PolicyId);
        pdel.del();
    }
    int message = p.add(PolicyId);
    if(message==POLICY_ADD_ERROR_IMPORTANT_INFO_EMPTY)
    {
        QMessageBox::critical(0, "ERROR",
                    QString("SOMETHING IS NULL!"), QMessageBox::Cancel);
        ui->tabWidget->setCurrentIndex(0);
    }
    if(message==POLICY_ADD_ERROR_SAME_POLICYID)
    {
        QMessageBox::critical(0, "ERROR",
                    QString("POLICYID HAS USED!"), QMessageBox::Cancel);
        ui->tabWidget->setCurrentIndex(4);
    }
    if(message==POLICY_ADD_SUCCESS)
    {
        ui->tabWidget->setCurrentIndex(5);
    }
}

void SellerOper::on_PBC3_clicked()
{
    Policy p(ui->LEPolicynum->text(),
             ui->LEName->text(),
             ui->LECard->text(),
             ui->LENum->text(),
             ui->LEStyle->text(),
             int(ui->SBPrice->value()),
             ui->DSBDiscount->value(),
             ui->DEStart->date());
    if(PolicyId!=-1)
    {
        Policy pdel(PolicyId);
        pdel.del();
    }
    int message = p.add(PolicyId);
    if(message==POLICY_ADD_ERROR_IMPORTANT_INFO_EMPTY)
    {
        QMessageBox::critical(0, "ERROR",
                    QString("SOMETHING IS NULL!"), QMessageBox::Cancel);
        ui->tabWidget->setCurrentIndex(0);
    }
    if(message==POLICY_ADD_ERROR_SAME_POLICYID)
    {
        QMessageBox::critical(0, "ERROR",
                    QString("POLICYID HAS USED!"), QMessageBox::Cancel);
        ui->tabWidget->setCurrentIndex(4);
    }
    if(message==POLICY_ADD_SUCCESS)
    {
        ui->tabWidget->setCurrentIndex(5);
    }
}

void SellerOper::on_PBWork2_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void SellerOper::on_PBMessage2_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

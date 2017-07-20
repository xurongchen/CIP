#include "selleroper.h"
#include "ui_selleroper.h"
#include "dboperation.h"
#include "currentuser.h"
#include "information.h"
#include "policyinsurance.h"
#include <QString>
#include <string>
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QDate>

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
    //ui->DSBDiscount->setSuffix(QString::fromLocal8Bit(" %"));
    ui->DSBDiscount->setValue(0);  // ��ǰֵ
    ui->DSBDiscount->setSpecialValueText(QString::fromLocal8Bit("���ۿ�"));  // �����ı�ֵ


    infosend=NULL;
    inforeci=NULL;
    //infosend
    send_renew(infosend);
    ui->TVSend->setModel(infosend);

    //�����п��ɱ䶯��������ͨ������϶������п�
    ui->TVSend->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->TVSend->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    //ui->TVSend->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    ui->TVSend->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    //ui->TVSend->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    //���ñ��ĸ��еĿ��ֵ
    ui->TVSend->setColumnWidth(0,70);
    ui->TVSend->setColumnWidth(1,80);
    ui->TVSend->setColumnWidth(2,190);
    ui->TVSend->setColumnWidth(3,0);

    //Ĭ����ʾ��ͷ���������ò����۵Ļ������ǿ��Խ�����
    ui->TVSend->verticalHeader()->hide();
    //����ѡ��ʱΪ����ѡ��
    ui->TVSend->setSelectionBehavior(QAbstractItemView::SelectRows);

    //���ñ��ĵ�ԪΪֻ�����ԣ������ܱ༭
    ui->TVSend->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //���������QTableView��ʹ���Ҽ��˵��������ø�����
    ui->TVSend->setContextMenuPolicy(Qt::CustomContextMenu);


    //inforeci
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);

    //�����п��ɱ䶯��������ͨ������϶������п�
    ui->TVReci->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->TVReci->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    //ui->TVReci->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    ui->TVReci->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    //ui->TVReci->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    //���ñ��ĸ��еĿ��ֵ
    ui->TVReci->setColumnWidth(0,70);
    ui->TVReci->setColumnWidth(1,80);
    ui->TVReci->setColumnWidth(2,190);
    ui->TVReci->setColumnWidth(3,0);

    //Ĭ����ʾ��ͷ���������ò����۵Ļ������ǿ��Խ�����
    ui->TVReci->verticalHeader()->hide();
    //����ѡ��ʱΪ����ѡ��
    ui->TVReci->setSelectionBehavior(QAbstractItemView::SelectRows);

    //���ñ��ĵ�ԪΪֻ�����ԣ������ܱ༭
    ui->TVReci->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //���������QTableView��ʹ���Ҽ��˵��������ø�����
    ui->TVReci->setContextMenuPolicy(Qt::CustomContextMenu);


    insuranceget=NULL;
    //insuranceget
    insuranceget_renew(insuranceget);
    ui->TVInsuranceget->setModel(insuranceget);

    //�����п��ɱ䶯��������ͨ������϶������п�
    ui->TVInsuranceget->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->TVInsuranceget->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    //ui->TVInsuranceget->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    ui->TVInsuranceget->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    //ui->TVInsuranceget->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    //���ñ��ĸ��еĿ��ֵ
    ui->TVInsuranceget->setColumnWidth(0,100);
    ui->TVInsuranceget->setColumnWidth(1,50);
    ui->TVInsuranceget->setColumnWidth(2,50);
    ui->TVInsuranceget->setColumnWidth(3,0);

    //Ĭ����ʾ��ͷ���������ò����۵Ļ������ǿ��Խ�����
    ui->TVInsuranceget->verticalHeader()->hide();
    //����ѡ��ʱΪ����ѡ��
    ui->TVInsuranceget->setSelectionBehavior(QAbstractItemView::SelectRows);

    //���ñ��ĵ�ԪΪֻ�����ԣ������ܱ༭
    ui->TVInsuranceget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //���������QTableView��ʹ���Ҽ��˵��������ø�����
    ui->TVInsuranceget->setContextMenuPolicy(Qt::CustomContextMenu);\


    insuranceall=NULL;
    //insuranceall
    insuranceall_renew(insuranceall);
    ui->TVInsuranceall->setModel(insuranceall);

    //�����п��ɱ䶯��������ͨ������϶������п�
    ui->TVInsuranceall->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->TVInsuranceall->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    //ui->TVInsuranceall->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    ui->TVInsuranceall->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    //ui->TVInsuranceall->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    //���ñ��ĸ��еĿ��ֵ
    ui->TVInsuranceall->setColumnWidth(0,100);
    ui->TVInsuranceall->setColumnWidth(1,50);
    ui->TVInsuranceall->setColumnWidth(2,50);
    ui->TVInsuranceall->setColumnWidth(3,0);

    //Ĭ����ʾ��ͷ���������ò����۵Ļ������ǿ��Խ�����
    ui->TVInsuranceall->verticalHeader()->hide();
    //����ѡ��ʱΪ����ѡ��
    ui->TVInsuranceall->setSelectionBehavior(QAbstractItemView::SelectRows);

    //���ñ��ĵ�ԪΪֻ�����ԣ������ܱ༭
    ui->TVInsuranceall->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //���������QTableView��ʹ���Ҽ��˵��������ø�����
    ui->TVInsuranceall->setContextMenuPolicy(Qt::CustomContextMenu);

}

SellerOper::~SellerOper()
{
    delete ui;
    send_delete(infosend);
    reci_delete(inforeci);
    insuranceget_delete(insuranceget);
    insuranceall_delete(insuranceall);
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



void SellerOper::on_PBSendDel_clicked()
{
    int curRow=ui->TVSend->currentIndex().row(); //ѡ����
    QAbstractItemModel *modessl = ui->TVSend->model();
    QModelIndex indextemp = modessl->index(curRow,3);//������һ�е�������,i����Ҫȡֵ���е��±�
    //�����һ����Ԫ���ֵ��tostring()----ok
    QVariant datatemp = modessl->data(indextemp);
    Info i(datatemp.toInt());
    i.del(SENDDEL);
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}

void SellerOper::on_PBSendClear_clicked()
{
    Current_user_send_Info_clear();
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}


void SellerOper::on_PBReciDel_clicked()
{
    int curRow=ui->TVReci->currentIndex().row(); //ѡ����
    QAbstractItemModel *modessl = ui->TVReci->model();
    QModelIndex indextemp = modessl->index(curRow,3);//������һ�е�������,i����Ҫȡֵ���е��±�
    //�����һ����Ԫ���ֵ��tostring()----ok
    QVariant datatemp = modessl->data(indextemp);
    Info i(datatemp.toInt());
    int message = i.del(RECIDEL);
    if(message==INFO_DEL_FAILED_NO_PERMISSION)
        QMessageBox::critical(0, "ERROR",
                    QString("THIS IS A BROADCAST,ONLY SENDER CAN DELETE IN SEND TABLE!"), QMessageBox::Cancel);
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}


void SellerOper::on_PBReciClear_clicked()
{
    Current_user_reci_Info_clear();
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}

void SellerOper::on_PBSend_clicked()
{
    QString strsend = ui->LESend->text();
    QString contentsend = ui->TESend->toPlainText();
    User u(strsend);
    int message = u.query();
    if(message==QUERY_NO_USER&&strsend!=NULL)
        QMessageBox::critical(0, "ERROR",
                    "NO SUCH USER!", QMessageBox::Cancel);
    else if(strsend!=NULL)
    {
        Info i(get_currentuser(),message,contentsend);
        i.add();
        QMessageBox::information(0, "SUCCESS",
                    QString("A MESSAGE HAS BEEN SENT TO %1!").arg(strsend), QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(0, "ERROR",
                    "RECI IS EMPTY!", QMessageBox::Cancel);
    }
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}



void SellerOper::on_PBSET_clicked()
{
    int rownum = ui->TVInsuranceget->model()->rowCount();
    if(rownum==0)
    {
        QMessageBox::critical(0, "ERROR",
                    "NO INSURANCE ADD!", QMessageBox::Cancel);
        return;
    }
    for(int row=0;row<rownum;row++)
    {
        QAbstractItemModel *modessl = ui->TVInsuranceall->model();
        QModelIndex indextemp = modessl->index(row,3);
        QVariant datatemp = modessl->data(indextemp);
        PolicyInsurance p(PolicyId,datatemp.toInt());
        p.add();
    }
    PolicyId = -1;
    insuranceget_init();
    insuranceget_renew(insuranceget);
    ui->TVInsuranceget->setModel(insuranceget);
    ui->TVInsuranceget->setColumnWidth(3,0);
    insuranceall_renew(insuranceall);
    ui->TVInsuranceall->setModel(insuranceall);
    ui->TVInsuranceall->setColumnWidth(3,0);
    ui->LECard->clear();
    ui->LEName->clear();
    ui->LENum->clear();
    ui->LEPolicynum->clear();
    ui->SBPrice->setValue(100000);
    ui->DSBDiscount->setValue(0);
    ui->DEStart->setDate(QDate::currentDate());
    QMessageBox::information(0, "SUCCESS",
                QString("INSURANCE HAS ADDED!"), QMessageBox::Ok);

    //Ԥ��Ԥ��

    //
    ui->tabWidget->setCurrentIndex(0);
}

void SellerOper::on_PBAdd_clicked()
{
    int curRow=ui->TVInsuranceall->currentIndex().row(); //ѡ����
    QAbstractItemModel *modessl = ui->TVInsuranceall->model();
    QModelIndex indextemp = modessl->index(curRow,3);//������һ�е�������,i����Ҫȡֵ���е��±�
    //�����һ����Ԫ���ֵ��tostring()----ok
    QVariant datatemp = modessl->data(indextemp);

    insuranceget_insert(datatemp.toInt());

    insuranceget_renew(insuranceget);
    ui->TVInsuranceget->setModel(insuranceget);
    ui->TVInsuranceget->setColumnWidth(3,0);
    insuranceall_renew(insuranceall);
    ui->TVInsuranceall->setModel(insuranceall);
    ui->TVInsuranceall->setColumnWidth(3,0);
}

void SellerOper::on_PBDel_clicked()
{
    int curRow=ui->TVInsuranceget->currentIndex().row(); //ѡ����
    QAbstractItemModel *modessl = ui->TVInsuranceget->model();
    QModelIndex indextemp = modessl->index(curRow,3);//������һ�е�������,i����Ҫȡֵ���е��±�
    //�����һ����Ԫ���ֵ��tostring()----ok
    QVariant datatemp = modessl->data(indextemp);

    insuranceget_remove(datatemp.toInt());

    insuranceget_renew(insuranceget);
    ui->TVInsuranceget->setModel(insuranceget);
    ui->TVInsuranceget->setColumnWidth(3,0);
    insuranceall_renew(insuranceall);
    ui->TVInsuranceall->setModel(insuranceall);
    ui->TVInsuranceall->setColumnWidth(3,0);
}

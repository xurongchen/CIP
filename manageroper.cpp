#include "manageroper.h"
#include "ui_manageroper.h"
#include "dboperation.h"
#include "currentuser.h"
#include "information.h"
#include "policyinsurance.h"
#include "insurance.h"
#include "detail.h"
#include "pop.h"
#include <QString>
#include <string>
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include <QLineEdit>
#include <QMessageBox>

ManagerOper::ManagerOper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManagerOper)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromLocal8Bit("���������ۺϹ���ƽ̨"));

    ui->tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px solid #e8f3f9;background:  transparent; }"); //����tab����͸��
    ui->tabWidget->findChildren<QTabBar*>().at(0)->hide(); //tab����������
    flag = 0;
    flag2 = 0;
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

    ui->tabWidget->setCurrentIndex(0);

    ui->SBCostfxed->setRange(0,1000000);
    ui->SBCostfxed->setSingleStep(100);
    ui->SBCostfxed->setValue(0);
    ui->SBCostfxed->setPrefix(QString::fromLocal8Bit("�� "));
    ui->SBCostfxed->setSuffix(QString::fromLocal8Bit(" Ԫ"));
    ui->SBCostfxed->setSpecialValueText(QString::fromLocal8Bit("�޹̶�����"));

    ui->DSBCostfloat->setRange(0, 100); // ��Χ
    ui->DSBCostfloat->setDecimals(3);  // ����
    ui->DSBCostfloat->setSingleStep(0.01); // ����
    ui->DSBCostfloat->setSuffix(QString::fromLocal8Bit(" %"));
    ui->DSBCostfloat->setValue(0);  // ��ǰֵ
    ui->DSBCostfloat->setSpecialValueText(QString::fromLocal8Bit("�޸�������"));  // �����ı�ֵ

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




    insurancelist=NULL;
    //infosend
    insurancelist_renew(insurancelist);
    ui->TVInsurance->setModel(insurancelist);

    //�����п��ɱ䶯��������ͨ������϶������п�
    ui->TVInsurance->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->TVInsurance->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    //ui->TVInsurance->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    ui->TVInsurance->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    ui->TVInsurance->horizontalHeader()->setResizeMode(3, QHeaderView::ResizeToContents);
    ui->TVInsurance->horizontalHeader()->setResizeMode(4, QHeaderView::ResizeToContents);
    //���ñ��ĸ��еĿ��ֵ
    ui->TVInsurance->setColumnWidth(0,70);
    ui->TVInsurance->setColumnWidth(1,80);
    ui->TVInsurance->setColumnWidth(2,190);
    ui->TVInsurance->setColumnWidth(3,100);
    ui->TVInsurance->setColumnWidth(4,100);
    ui->TVInsurance->setColumnWidth(5,0);
    //Ĭ����ʾ��ͷ���������ò����۵Ļ������ǿ��Խ�����
    ui->TVInsurance->verticalHeader()->hide();
    //����ѡ��ʱΪ����ѡ��
    ui->TVInsurance->setSelectionBehavior(QAbstractItemView::SelectRows);

    //���ñ��ĵ�ԪΪֻ�����ԣ������ܱ༭
    ui->TVInsurance->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //���������QTableView��ʹ���Ҽ��˵��������ø�����
    ui->TVInsurance->setContextMenuPolicy(Qt::CustomContextMenu);

    insuranceget_init();

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

ManagerOper::~ManagerOper()
{
    delete ui;
    insurancelist_delete(insurancelist);
}

void ManagerOper::mousePressEvent(QMouseEvent *e) //���������
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
void ManagerOper::mouseMoveEvent(QMouseEvent *e) //�����������ƶ�
{
    if(flag == 0) return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    //qDebug() << last.rx() << ".." << last.ry();
    move(x()+dx, y()+dy);
}
void ManagerOper::mouseReleaseEvent(QMouseEvent *e) //����ͷ�
{
    judge.setX(0), judge.setY(0);
    flag = 0;
}

void ManagerOper::on_PBReturn_clicked()
{
    flag2 = 1;
    this->close();
}

void ManagerOper::on_PBMin_clicked()
{
    this->showMinimized();
}
bool ManagerOper::closeWidget()
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

void ManagerOper::closeEvent(QCloseEvent *)
{
    //�˳�ϵͳ
    if(flag2 != 1)
    {
        qDebug() << "exit";
        QApplication::quit();
    }
}

void ManagerOper::on_PBView_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void ManagerOper::on_PBAdd_clicked()
{
    insurancelist_renew(insurancelist);
    ui->TVInsurance->setModel(insurancelist);
    ui->tabWidget->setCurrentIndex(1);
}

void ManagerOper::on_PBMessage_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void ManagerOper::on_PBSend1_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void ManagerOper::on_PBSend2_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void ManagerOper::on_PBSend3_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void ManagerOper::on_PBMy1_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void ManagerOper::on_PBMy2_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void ManagerOper::on_PBMy3_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void ManagerOper::on_PBGet1_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void ManagerOper::on_PBGet2_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void ManagerOper::on_PBGet3_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

void ManagerOper::on_PBMessage2_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

void ManagerOper::on_PBAdd2_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void ManagerOper::on_PBView2_clicked()
{
    insurancelist_renew(insurancelist);
    ui->TVInsurance->setModel(insurancelist);
    ui->tabWidget->setCurrentIndex(0);
}

void ManagerOper::on_PBSend_clicked()
{
    QString strsend = ui->LESend->text();
    QString contentsend = ui->TESend->toPlainText();
    User u(strsend);
    int message = u.query();
    if(message==QUERY_NO_USER&&strsend!=NULL)
        //QMessageBox::critical(0, "ERROR",
        //            "NO SUCH USER!", QMessageBox::Cancel);
        show_word(QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���û������ڣ�"));
    else if(strsend!=NULL)
    {
        Info i(get_currentuser(),message,contentsend);
        i.add();
        //QMessageBox::information(0, "SUCCESS",
        //            QString("A MESSAGE HAS BEEN SENT TO %1!").arg(strsend), QMessageBox::Ok);
        //show_word(QString::fromLocal8Bit("�ɹ���"), QString::fromLocal8Bit("һ����Ϣ�Ѿ����͸���%1!").arg(strsend));
        show_word(QString::fromLocal8Bit("�ɹ���"), QString::fromLocal8Bit("��Ϣ���ͳɹ���"));
    }
    else
    {
        Info i(get_currentuser(),-1,contentsend);
        i.add();
        //QMessageBox::information(0, "SUCCESS",
        //            QString("A MESSAGE HAS BEEN BROADCAST!"), QMessageBox::Ok);
        show_word(QString::fromLocal8Bit("�ɹ���"), QString::fromLocal8Bit("��Ϣ�㲥�ɹ���"));
    }
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}

void ManagerOper::on_PBSendDel_clicked()
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

void ManagerOper::on_PBSendClear_clicked()
{
    Current_user_send_Info_clear();
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}

void ManagerOper::on_PBReciDel_clicked()
{
    int curRow=ui->TVReci->currentIndex().row(); //ѡ����
    QAbstractItemModel *modessl = ui->TVReci->model();
    QModelIndex indextemp = modessl->index(curRow,3);//������һ�е�������,i����Ҫȡֵ���е��±�
    //�����һ����Ԫ���ֵ��tostring()----ok
    QVariant datatemp = modessl->data(indextemp);
    Info i(datatemp.toInt());
    int message = i.del(RECIDEL);
    if(message==INFO_DEL_FAILED_NO_PERMISSION)
        //QMessageBox::critical(0, "ERROR",
        //            QString("THIS IS A BROADCAST,ONLY SENDER CAN DELETE IN SEND TABLE!"), QMessageBox::Cancel);
    show_word(QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�㲥��Ϣֻ���ɷ�����ɾ����"));
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}

void ManagerOper::on_PBReciClear_clicked()
{
    Current_user_reci_Info_clear();
    send_renew(infosend);
    ui->TVSend->setModel(infosend);
    ui->TVSend->setColumnWidth(3,0);
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);
    ui->TVReci->setColumnWidth(3,0);
}

void ManagerOper::on_PBDele_clicked()
{
    QString judge = ui->LEInsurancename->text();
    if(judge == "")
    {
        show_word(QString::fromLocal8Bit("����"), QString::fromLocal8Bit("����������Ϊ�գ�"));
        return ;
    }
    Insurance i(ui->LEInsurancename->text(),
                int(ui->SBCostfxed->value()),
                ui->DSBCostfloat->value());
    i.add();
    //QMessageBox::information(0, "SUCCESS",
    //           QString("ADD INSURANCE SUCCESS!"), QMessageBox::Cancel);
    show_word(QString::fromLocal8Bit("�ɹ���"), QString::fromLocal8Bit("�±�����ӳɹ���"));
    ui->LEInsurancename->clear();
    ui->SBCostfxed->setValue(0);
    ui->DSBCostfloat->setValue(0);

    insuranceall_renew(insuranceall);
    ui->TVInsuranceall->setModel(insuranceall);
    ui->TVInsuranceall->setColumnWidth(3,0);
}

void ManagerOper::get_title(QString word)
{
    ui->ManTitle->setText(word);
}

void ManagerOper::on_PBAdd_3_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}


void ManagerOper::on_PBLook_2_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}


void ManagerOper::on_PBAdd_2_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}


void ManagerOper::on_PBLook_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}


void ManagerOper::on_PBDel_clicked()
{
    int curRow=ui->TVInsuranceall->currentIndex().row(); //ѡ����
    QAbstractItemModel *modessl = ui->TVInsuranceall->model();
    QModelIndex indextemp = modessl->index(curRow,3);//������һ�е�������,i����Ҫȡֵ���е��±�
    //�����һ����Ԫ���ֵ��tostring()----ok
    QVariant datatemp = modessl->data(indextemp);
    Insurance i(datatemp.toInt());
    i.del();
    insuranceall_renew(insuranceall);
    ui->TVInsuranceall->setModel(insuranceall);
    ui->TVInsuranceall->setColumnWidth(3,0);

}

void ManagerOper::on_PBDetail_clicked()
{
    int curRow=ui->TVInsurance->currentIndex().row(); //ѡ����
    if(curRow==-1)
        return;
    QAbstractItemModel *modessl = ui->TVInsurance->model();
    QModelIndex indextemp = modessl->index(curRow,5);//������һ�е�������,i����Ҫȡֵ���е��±�
    //�����һ����Ԫ���ֵ��tostring()----ok
    QVariant datatemp = modessl->data(indextemp);
    call_detail(datatemp.toInt());
}

void ManagerOper::on_PBSerch_clicked()
{
    QString str = ui->LESearch->text();
    insurancelist_search(insurancelist,str);
    ui->TVInsurance->setModel(insurancelist);
    ui->TVInsurance->setColumnWidth(5,0);
}

void ManagerOper::on_PBFinshserch_clicked()
{
    insurancelist_renew(insurancelist);
    ui->TVInsurance->setModel(insurancelist);
    ui->TVInsurance->setColumnWidth(5,0);
}

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
    ui->tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px solid #e8f3f9;background:  transparent; }"); //设置tab背景透明
    ui->tabWidget->findChildren<QTabBar*>().at(0)->hide(); //tab标题栏隐藏
    flag = 0;
    flag2 = 0;
    PolicyId = -1;
    this->setFixedSize(720,445); //设置固定大小
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint); //隐藏标题栏及最小化可见

    //圆角
    QBitmap objBitmap(size());
    QPainter painter(&objBitmap);
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(this->rect(),11,11);
    setMask(objBitmap);

    //连接关闭按钮和关闭动画
    connect(ui->PBClose, SIGNAL(clicked()), this, SLOT(closeWidget()));

    ui->DEStart->setMinimumDate(QDate::currentDate().addDays(0));
    ui->DEStart->setMaximumDate(QDate::currentDate().addDays(365));  // +365天
    ui->DEStart->setCalendarPopup(true);  // 日历弹出
    //ui->DEStart->setCalendarWidget();

    ui->tabWidget->setCurrentIndex(0);

    QRegExp regExp_ID_Card("^((1[1-5])|(2[1-3])|(3[1-7])|(4[1-6])|(5[0-4])|(6[1-5])|71|(8[12])|91)\\d{4}((19\\d{2}(0[13-9]|1[012])(0[1-9]|[12]\\d|30))|(19\\d{2}(0[13578]|1[02])31)|(19\\d{2}02(0[1-9]|1\\d|2[0-8]))|(19([13579][26]|[2468][048]|0[48])0229))\\d{3}(\\d|X|x)?$");
    ui->LECard->setValidator(new QRegExpValidator(regExp_ID_Card,this));

    QRegExp regExp_Car_Number(QString::fromLocal8Bit("^[京津沪渝冀豫云辽黑湘皖鲁新苏浙赣鄂桂甘晋蒙陕吉闽贵粤青藏川宁琼使领A-Z]{1}[A-Z]{1}[A-Z0-9]{4}[A-Z0-9挂学警港澳]{1}$"));
    ui->LENum->setValidator(new QRegExpValidator(regExp_Car_Number,this));

    ui->SBPrice->setRange(5000, 50000000);  // 范围
    ui->SBPrice->setSingleStep(100000); // 步长
    ui->SBPrice->setValue(100000);  // 当前值
    ui->SBPrice->setPrefix(QString::fromLocal8Bit("￥ "));  // 前缀
    ui->SBPrice->setSuffix(QString::fromLocal8Bit(" 元"));  // 后缀

    ui->DSBDiscount->setRange(0, 1); // 范围
    ui->DSBDiscount->setDecimals(3);  // 精度
    ui->DSBDiscount->setSingleStep(0.01); // 步长
    //ui->DSBDiscount->setSuffix(QString::fromLocal8Bit(" %"));
    ui->DSBDiscount->setValue(0);  // 当前值
    ui->DSBDiscount->setSpecialValueText(QString::fromLocal8Bit("无折扣"));  // 特殊文本值


    infosend=NULL;
    inforeci=NULL;
    //infosend
    send_renew(infosend);
    ui->TVSend->setModel(infosend);

    //设置列宽不可变动，即不能通过鼠标拖动增加列宽
    ui->TVSend->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->TVSend->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    //ui->TVSend->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    ui->TVSend->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    //ui->TVSend->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    //设置表格的各列的宽度值
    ui->TVSend->setColumnWidth(0,70);
    ui->TVSend->setColumnWidth(1,80);
    ui->TVSend->setColumnWidth(2,190);
    ui->TVSend->setColumnWidth(3,0);

    //默认显示行头，如果你觉得不美观的话，我们可以将隐藏
    ui->TVSend->verticalHeader()->hide();
    //设置选中时为整行选中
    ui->TVSend->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->TVSend->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //如果你用在QTableView中使用右键菜单，需启用该属性
    ui->TVSend->setContextMenuPolicy(Qt::CustomContextMenu);


    //inforeci
    reci_renew(inforeci);
    ui->TVReci->setModel(inforeci);

    //设置列宽不可变动，即不能通过鼠标拖动增加列宽
    ui->TVReci->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->TVReci->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    //ui->TVReci->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    ui->TVReci->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    //ui->TVReci->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    //设置表格的各列的宽度值
    ui->TVReci->setColumnWidth(0,70);
    ui->TVReci->setColumnWidth(1,80);
    ui->TVReci->setColumnWidth(2,190);
    ui->TVReci->setColumnWidth(3,0);

    //默认显示行头，如果你觉得不美观的话，我们可以将隐藏
    ui->TVReci->verticalHeader()->hide();
    //设置选中时为整行选中
    ui->TVReci->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->TVReci->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //如果你用在QTableView中使用右键菜单，需启用该属性
    ui->TVReci->setContextMenuPolicy(Qt::CustomContextMenu);


    insuranceget=NULL;
    //insuranceget
    insuranceget_renew(insuranceget);
    ui->TVInsuranceget->setModel(insuranceget);

    //设置列宽不可变动，即不能通过鼠标拖动增加列宽
    ui->TVInsuranceget->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->TVInsuranceget->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    //ui->TVInsuranceget->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    ui->TVInsuranceget->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    //ui->TVInsuranceget->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    //设置表格的各列的宽度值
    ui->TVInsuranceget->setColumnWidth(0,100);
    ui->TVInsuranceget->setColumnWidth(1,50);
    ui->TVInsuranceget->setColumnWidth(2,50);
    ui->TVInsuranceget->setColumnWidth(3,0);

    //默认显示行头，如果你觉得不美观的话，我们可以将隐藏
    ui->TVInsuranceget->verticalHeader()->hide();
    //设置选中时为整行选中
    ui->TVInsuranceget->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->TVInsuranceget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //如果你用在QTableView中使用右键菜单，需启用该属性
    ui->TVInsuranceget->setContextMenuPolicy(Qt::CustomContextMenu);\


    insuranceall=NULL;
    //insuranceall
    insuranceall_renew(insuranceall);
    ui->TVInsuranceall->setModel(insuranceall);

    //设置列宽不可变动，即不能通过鼠标拖动增加列宽
    ui->TVInsuranceall->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->TVInsuranceall->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    //ui->TVInsuranceall->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    ui->TVInsuranceall->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    //ui->TVInsuranceall->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    //设置表格的各列的宽度值
    ui->TVInsuranceall->setColumnWidth(0,100);
    ui->TVInsuranceall->setColumnWidth(1,50);
    ui->TVInsuranceall->setColumnWidth(2,50);
    ui->TVInsuranceall->setColumnWidth(3,0);

    //默认显示行头，如果你觉得不美观的话，我们可以将隐藏
    ui->TVInsuranceall->verticalHeader()->hide();
    //设置选中时为整行选中
    ui->TVInsuranceall->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->TVInsuranceall->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //如果你用在QTableView中使用右键菜单，需启用该属性
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

void SellerOper::mousePressEvent(QMouseEvent *e) //鼠标点击界面
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
void SellerOper::mouseMoveEvent(QMouseEvent *e) //界面跟随鼠标移动
{
    if(flag == 0) return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    //qDebug() << last.rx() << ".." << last.ry();
    move(x()+dx, y()+dy);
}
void SellerOper::mouseReleaseEvent(QMouseEvent *e) //鼠标释放
{
    judge.setX(0), judge.setY(0);
    flag = 0;
}

bool SellerOper::closeWidget()
{
   //界面动画，改变透明度的方式消失1 - 0渐变
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
    //退出系统
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
    int curRow=ui->TVSend->currentIndex().row(); //选中行
    QAbstractItemModel *modessl = ui->TVSend->model();
    QModelIndex indextemp = modessl->index(curRow,3);//遍历第一行的所有列,i是你要取值的列的下标
    //这个是一个单元格的值。tostring()----ok
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
    int curRow=ui->TVReci->currentIndex().row(); //选中行
    QAbstractItemModel *modessl = ui->TVReci->model();
    QModelIndex indextemp = modessl->index(curRow,3);//遍历第一行的所有列,i是你要取值的列的下标
    //这个是一个单元格的值。tostring()----ok
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

    //预留预览

    //
    ui->tabWidget->setCurrentIndex(0);
}

void SellerOper::on_PBAdd_clicked()
{
    int curRow=ui->TVInsuranceall->currentIndex().row(); //选中行
    QAbstractItemModel *modessl = ui->TVInsuranceall->model();
    QModelIndex indextemp = modessl->index(curRow,3);//遍历第一行的所有列,i是你要取值的列的下标
    //这个是一个单元格的值。tostring()----ok
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
    int curRow=ui->TVInsuranceget->currentIndex().row(); //选中行
    QAbstractItemModel *modessl = ui->TVInsuranceget->model();
    QModelIndex indextemp = modessl->index(curRow,3);//遍历第一行的所有列,i是你要取值的列的下标
    //这个是一个单元格的值。tostring()----ok
    QVariant datatemp = modessl->data(indextemp);

    insuranceget_remove(datatemp.toInt());

    insuranceget_renew(insuranceget);
    ui->TVInsuranceget->setModel(insuranceget);
    ui->TVInsuranceget->setColumnWidth(3,0);
    insuranceall_renew(insuranceall);
    ui->TVInsuranceall->setModel(insuranceall);
    ui->TVInsuranceall->setColumnWidth(3,0);
}

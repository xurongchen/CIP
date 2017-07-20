#include "detail.h"
#include "ui_detail.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QBitmap>
#include <QPainter>

Detail::Detail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Detail)
{
    ui->setupUi(this);
    insurancelist=NULL;

    setWindowTitle(QString::fromLocal8Bit("汽车保险综合管理平台"));

    flag = 0;
    flag2 = 0;
    this->setFixedSize(420,520); //设置固定大小
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint); //隐藏标题栏及最小化可见

    //圆角
    QBitmap objBitmap(size());
    QPainter painter(&objBitmap);
    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(this->rect(),11,11);
    setMask(objBitmap);

}

Detail::~Detail()
{
    delete ui;
    if(insurancelist!=NULL)
        delete insurancelist;
}

void Detail::set_policyid(int _policyid)
{
    policyid = _policyid;
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec(QString("select * from PolicyList where Id=%1").arg(policyid));
    query.first();

    int fixtol = 0;
    double floattol = 0.0;

    ui->LBPolicyId->setText(query.value(1).toString());
    ui->LBCarNumber->setText(query.value(4).toString());
    ui->LBCarstyle->setText(query.value(5).toString());
    ui->LBCarValue->setText(query.value(6).toString());
    int carvalue = query.value(6).toInt();
    ui->LBDiscount->setText(query.value(9).toString());
    double discount = query.value(9).toDouble();
    ui->LBInsured->setText(query.value(2).toString());
    ui->LBInsuredDocument->setText(query.value(3).toString());
    ui->LBPolicytime->setText(query.value(7).toString());
    ui->LBStarttime->setText(query.value(8).toString());
    QSqlQuery tmpquery(db);
    tmpquery.exec(QString("select * from UserList where Id=%1").arg(query.value(10).toInt()));
    tmpquery.first();
    ui->LBSeller->setText(tmpquery.value(1).toString());


    if(insurancelist!=NULL)
        delete insurancelist;
    insurancelist = new QStandardItemModel();
    insurancelist->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("name")));
    insurancelist->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("fixedcost")));
    insurancelist->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("floatcost")));
    query.exec(QString("select * from PolicyInsuranceRelationList where PolicyId=%1").arg(policyid));
    //query.first();
    int nowline = 0;
    while(query.next())
    {
        tmpquery.exec(QString("select * from InsuranceList where Id=%1").arg(query.value(2).toInt()));
        tmpquery.first();
        insurancelist->setItem(nowline, 0, new QStandardItem(tmpquery.value(1).toString()));
        insurancelist->setItem(nowline, 1, new QStandardItem(tmpquery.value(2).toString()));
        fixtol+=tmpquery.value(2).toInt();
        insurancelist->setItem(nowline, 2, new QStandardItem(tmpquery.value(3).toString()));
        floattol+=tmpquery.value(3).toDouble();
        nowline++;
    }
    insurancelist->sort(1, Qt::AscendingOrder);
    ui->TVPolicycontent->setModel(insurancelist);
    int sumcost = int((1-discount)*(fixtol + int(floattol*carvalue*0.01)));
    ui->LBCost->setText(QString::number(sumcost, 10));


    ui->LBPolicyId->adjustSize();
    ui->LBCarNumber->adjustSize();
    ui->LBCarstyle->adjustSize();
    ui->LBCarValue->adjustSize();
    ui->LBDiscount->adjustSize();
    ui->LBInsured->adjustSize();
    ui->LBInsuredDocument->adjustSize();
    ui->LBPolicytime->adjustSize();
    ui->LBStarttime->adjustSize();
    ui->LBSeller->adjustSize();
    ui->LBCost->adjustSize();

    //设置列宽不可变动，即不能通过鼠标拖动增加列宽
    ui->TVPolicycontent->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->TVPolicycontent->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    //ui->TVPolicycontent->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    ui->TVPolicycontent->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    //设置表格的各列的宽度值
    ui->TVPolicycontent->setColumnWidth(0,200);
    ui->TVPolicycontent->setColumnWidth(1,50);
    ui->TVPolicycontent->setColumnWidth(2,50);
    //默认显示行头，如果你觉得不美观的话，我们可以将隐藏
    ui->TVPolicycontent->verticalHeader()->hide();
    //设置选中时为整行选中
    ui->TVPolicycontent->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->TVPolicycontent->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //如果你用在QTableView中使用右键菜单，需启用该属性
    ui->TVPolicycontent->setContextMenuPolicy(Qt::CustomContextMenu);

    this->exec();
}

void call_detail(int policyid)
{
    Detail d;
    d.set_policyid(policyid);
}

void Detail::on_PBOk_clicked()
{
    this->close();
}

void Detail::mousePressEvent(QMouseEvent *e) //鼠标点击界面
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
void Detail::mouseMoveEvent(QMouseEvent *e) //界面跟随鼠标移动
{
    if(flag == 0) return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    //qDebug() << last.rx() << ".." << last.ry();
    move(x()+dx, y()+dy);
}
void Detail::mouseReleaseEvent(QMouseEvent *e) //鼠标释放
{
    judge.setX(0), judge.setY(0);
    flag = 0;
}

void Detail::on_PBClose_clicked()
{
    this->close();
}

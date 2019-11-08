#include "frmmain.h"
#include "ui_frmmain.h"
#include "qmessagebox.h"
#include "qfile.h"

frmMain::frmMain(QWidget *parent) : QWidget(parent), ui(new Ui::frmMain)
{
    ui->setupUi(this);
    this->initForm();
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::initForm()
{
    QStringList min;
    min << "1" << "5" << "10" << "20" << "30";
    for (int i = 1; i <= 24; i++) {
        min << QString::number(i * 60);
    }

    ui->cboxMin->addItems(min);
    ui->cboxMin->setCurrentIndex(1);
    ui->dateEdit->setDate(QDate::currentDate());

    for (int i = 5; i <= 150; i = i + 5) {
        ui->cboxCount->addItem(QString("%1").arg(i));
    }
}

QString frmMain::getXorEncryptDecrypt(const QString &data, char key)
{
    //采用异或加密,也可以自行更改算法
    QByteArray buffer = data.toLatin1();
    int size = buffer.size();
    for (int i = 0; i < size; i++) {
        buffer[i] = buffer.at(i) ^ key;
    }
    qDebug() << data;
    qDebug() << buffer;

    return QLatin1String(buffer);
}

void frmMain::on_btnOk_clicked()
{
    bool useDate = ui->ckDate->isChecked();
    bool useRun = ui->ckRun->isChecked();
    bool useCount = ui->ckCount->isChecked();

    if (!useDate && !useRun && !useCount) {
        if (QMessageBox::question(this, "询问", "确定要生成没有任何限制的密钥吗?") != QMessageBox::Yes) {
            return;
        }
    }

    QString strDate = ui->dateEdit->date().toString("yyyy-MM-dd");
    QString strRun = ui->cboxMin->currentText();
    QString strCount = ui->cboxCount->currentText();
    QString key = QString("%1|%2|%3|%4|%5|%6").arg(useDate).arg(strDate).arg(useRun).arg(strRun).arg(useCount).arg(strCount);

    QFile file(QApplication::applicationDirPath() + "/key.db");
    file.open(QFile::WriteOnly | QIODevice::Text);
    file.write(getXorEncryptDecrypt(key, 110).toLatin1());
    file.close();
    QMessageBox::information(this, "提示", "生成密钥成功,将 key.db 文件拷贝到对应目录即可!");
}

void frmMain::on_btnClose_clicked()
{
    this->close();
}

#include "mainwindow.hxx"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QRegExp>
#include <QRegExpValidator>
#include <QDir>
#include <QDebug>
#include <QTextStream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    parse();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parse()
{
    QStringList allWord;
    QDir dir;
    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    QStringList list = dir.entryList();
    foreach(QString file, list) {
        if (file.contains("txt") || file.contains("TXT") ) {
            QFile f(file);
            if (f.open(QIODevice::ReadOnly)) {
                QTextStream stream(&f);
                stream.setCodec("GBK");
                QString text = stream.readAll();
                text.replace("-\n", "");
                QStringList wordList =  text.split(QRegExp("\\s+"),QString::SkipEmptyParts);
                allWord.append(wordList);
            } else {
                qDebug() << "cannot open file:" << file;
            }
        }

    }
    if (allWord.isEmpty())
        return;

    allWord.removeDuplicates();
    for (int i = 0; i < allWord.size(); ++i) {
        QString &text = allWord[i];
        QString temp = text;
        text = text.toLower();
        text.remove(QRegExp("[^a-zA-Z\-]"));

    }
    allWord.removeDuplicates();
    allWord.removeOne(QString(""));
    allWord.sort();
    qDebug() << allWord.size();
    QFile outFile("wordlist.txt");
    if (!outFile.open(QIODevice::WriteOnly)) {
        qDebug() << "cannot open wordlist.txt";
        return ;
    }
    QTextStream out(&outFile);
    foreach (QString word, allWord) {
        out << word << endl;
    }
    out << flush;
    qDebug() << "Ok";
    //http://media.shanbay.com/audio/us/content.mp3

}

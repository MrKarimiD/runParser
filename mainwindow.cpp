#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addr_button_clicked()
{
    QString fileAddress = QFileDialog::getOpenFileName(this,tr("Select Your run File"), "/home", tr("Text Files (*.runs)"));
    ui->selected_textEdit->append(fileAddress);
    files.append(fileAddress);
}

void MainWindow::on_process_button_clicked()
{
   // srand (time(NULL));
    ui->log_textEdit->append("Processing Starts...");
    for(int i = 0; i < files.size(); i++)
    {
        QStringList list1 = files.at(i).split("/");
        QString tmp = "File '";
        tmp.append(list1.last());
        tmp.append("' is processing...");
        ui->log_textEdit->append(tmp);

        QFile file(files.at(i));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString errorText = "File '";
            errorText.append(list1.last());
            errorText.append("' is not available");
            ui->log_textEdit->append(errorText);
            continue;
        }

        QTextStream in(&file);
        QString data = in.readAll();
        QStringList textParts = data.split("------------------------------------------------------------------\n");
        QStringList coincidenceMatrix = textParts.last().split("\n\n");

        QStringList alphabets;
        alphabets<<"A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J";

        QString outputName;
        QStringList outputNameLists = list1.last().split("-");
        for(int n = 0; n < outputNameLists.size(); n++)
        {
            outputName.append(outputNameLists.at(n));
            if( n != outputNameLists.size()-1 ) outputName.append("_");
        }
        outputName.remove(".runs");
        outputName.append(".m");

        QFile output_file(outputName);
        if (!output_file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QString errorText = "File '";
            errorText.append(outputName);
            errorText.append("' cannot be created");
            ui->log_textEdit->append(errorText);
            continue;
        }

        QTextStream out(&output_file);
        out << "clc\n";
        out << "clear all\n\n";
        //out <<"close all\n\n";

        for(int j = 1; j < coincidenceMatrix.size(); j++)
        {
            out <<alphabets.at(j-1)<<"=[";
            QStringList lines = coincidenceMatrix.at(j).split("\n");
            if( j == 1) problemSize = lines.size() - 1;
            for(int k = 1; k < lines.size(); k++)
            {
                out<< lines.at(k);
                if( k == lines.size()-1 )
                    out<<"];\n";
                else
                    out<<"\n";
            }
        }
        out << "x = 1:"<<problemSize<<";\n";
        out << "y = 1:"<<problemSize<<";\n";
        out << "[xx,yy] = meshgrid(x,y);\n";
        out << "figure();\n";
        for(int j = 1; j < coincidenceMatrix.size(); j++)
        {
            out<<"subplot(1,"<<coincidenceMatrix.size()-1<<","<<j<<");\n";
            out<<"surf(xx,yy,"<<alphabets.at(j-1)<<");\n";
            out<<"title('Model"<<j<<"');\n";
        }
        QString outputLog = outputName;
        outputLog.append(" is created.");
        ui->log_textEdit->append(outputLog);
        output_file.close();
    }
    ui->log_textEdit->append("Process finished!");
}

void MainWindow::on_clear_button_clicked()
{
    files.clear();
    ui->selected_textEdit->clear();
}

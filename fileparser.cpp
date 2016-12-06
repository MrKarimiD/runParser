#include "fileparser.h"

fileParser::fileParser(QObject *parent) :
    QObject(parent)
{
}

void fileParser::clearList()
{
    this->desiredFiles.clear();
}

void fileParser::sepratingFiles(const QString &parameter)
{
    QDir path(parameter);
    QDirIterator it(parameter, QDirIterator::Subdirectories);
    int numberOfFiles = 0;

    while (it.hasNext()) {
        numberOfFiles++;
        QString fileName = it.next();
        if( fileName.endsWith(".runs") )
        {
            qDebug() << fileName;
            int percent = double(numberOfFiles/path.count())*100;
            emit progressResult(fileName,percent);
            desiredFiles.append(fileName);
        }
    }
    emit fetching_finished();
}

void fileParser::processingFiles()
{
    emit processingResult("Processing Starts...",0);
    for(int i = 0; i < desiredFiles.size(); i++)
    {
        QStringList list1 = desiredFiles.at(i).split("/");
        QString tmp = "File '";
        tmp.append(list1.last());
        tmp.append("' is processing...");
        int percent = ((double)i/desiredFiles.length())*100;
        emit processingResult(tmp,percent);

        //qDebug()<<"file: "<<desiredFiles.at(i);
        QFile file(desiredFiles.at(i));
        //qDebug()<<"percent: "<<percent;
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString errorText = "File '";
            errorText.append(list1.last());
            errorText.append("' is not available");
            emit processingResult(errorText,0);
            continue;
        }

        emit processingResult("Reading "+list1.last(),percent);
        QTextStream in(&file);
        QString data = in.readAll();
        QStringList textParts = data.split("------------------------------------------------------------------\n");
        QStringList coincidenceMatrix = textParts.last().split("\n\n");

        emit processingResult("Generating mFile for "+list1.last(),percent);
        QStringList alphabets;
        alphabets<<"A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J";

        QString outputName;
        for(int m = 0; m < list1.size()-1;m++)
        {
            outputName.append(list1.at(m));
            outputName.append("/");
        }
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
            emit processingResult(errorText,0);
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
        emit processingResult(outputLog,percent);
        output_file.close();
    }
    emit processing_finished();
}

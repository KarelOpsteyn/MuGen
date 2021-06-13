#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <string>
#include <QString>
#include <exception>

#include "song.h"
#include "exception.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString tempomask = "D00";      //mask for tempo input
    ui->lineEdit_tempo->setInputMask(tempomask);
    QString lengthmask = "D000";    //mask for length input
    ui->lineEdit_length->setInputMask(lengthmask);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_generate_clicked()   //Generate entire song from scratch
{
    //Get vars from GUI
    bool ok;    //to check tempo input
    int Tempo = ui->lineEdit_tempo->displayText().toInt(&ok);
        if(!ok){
            ui->lineEdit_out->setText("Tempo not set correctly.");
        }

    bool ok2;   //to check length input
    int Length = ui->lineEdit_length->displayText().toInt(&ok2);
        if(!ok2){
            ui->lineEdit_out->setText("Length not set correctly.");
        }

    QString Filename = ui->lineEdit_filename->displayText();

    int Mode;
    switch(ui->comboBox_mode->currentIndex()){
        case 0: Mode = true;
                break;
        case 1: Mode = false;
                break;
        }

    int Key = 60 + ui->comboBox_key->currentIndex();

    //Start generation
    if(ok && ok2){  //if get vars successful
        bool gensuccess = true;
        ui->lineEdit_out->setText("Generation started");
        Song newsong(Length, Key, Mode, Tempo);
        try{
            newsong.generate();                                 //generate song
        }catch(Exception_GradeProgression &e){
            ui->lineEdit_out->setText(e.what());
            gensuccess = false;

        }catch(Exception_GradeInitialisation &e){
            ui->lineEdit_out->setText(e.what());
            gensuccess = false;
        }
        if(gensuccess){
            ui->lineEdit_out->setText("Generation completed");
            newsong.midiExport(Filename.toStdString());         //Export to midi
            ui->lineEdit_out->setText("Exported to "+Filename);
        }
    }
}

void MainWindow::on_pushButton_solve_clicked()  //generate song based on a given bassline
{
    //Get vars from GUI
    QString inputfilename = ui->lineEdit_filename_import->displayText();

    QString outputfilename = ui->lineEdit_filename_export->displayText();

    int Mode;

    switch(ui->comboBox_mode_2->currentIndex()){
        case 0: Mode = true;
                break;
        case 1: Mode = false;
                break;
    }

    int Key = 60 + ui->comboBox_key_2->currentIndex();

    //Start generation
    bool gensuccess = true;
    ui->lineEdit_out->setText("program started");
    Song newsong(Key, Mode);
    newsong.midiImportbass(inputfilename.toStdString());    //Import bassline
    ui->lineEdit_out->setText("Import successfull");
    try{
    newsong.solve_bassproblem();                            //Solve basschords
    }catch(Exception_GradeProgression &e){  //catch and display errors
        ui->lineEdit_out->setText(e.what());
        gensuccess = false;
    }catch(Exception_GradeInitialisation &e){
        ui->lineEdit_out->setText(e.what());
        gensuccess = false;
    }
    if(gensuccess){
        ui->lineEdit_out->setText("Generation Successfull");
        newsong.midiExport(outputfilename.toStdString());   //Export to midi
        ui->lineEdit_out->setText("Exported to "+outputfilename);
    }
}

#include "application.h"
#include <QFileDialog>
#include <QDir>
#include <QIntValidator>
#include <QMessageBox>

#include <cassert>

#include "ui_application.h"
// At the moment we only process bitmap (.bmp) files
// Resource used for creating bmp file:
// https://ricardolovelace.com/blog/creating-bitmap-images-with-c-on-windows/
#include "ImageHandler.h"

Application::Application(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Application)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QIntValidator);
    ui->stackedWidget->setCurrentIndex(0);
}

Application::~Application()
{
    delete ui;
}

void Application::on_mode_readBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Application::on_mode_writeBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Application::on_mode_AboutBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Application::on_input_ImageDir_clicked()
{
    QString filter = "Bitmap file (*.bmp) ;; All Files (*.*)";
    inputImageRead = QFileDialog::getOpenFileName(this, "Select the image", QDir::currentPath(), filter);

    if (!inputImageRead.size())  return;

    ui->textBrowser->setPlainText(inputImageRead);
}

void Application::on_output_TxtFile_clicked()
{
    QString filter = "Text file (*.txt) ;; All Files (*.*)";
    outputTextRead = QFileDialog::getOpenFileName(this, "Select the text file", QDir::currentPath(), filter);

    if (!outputTextRead.size())  return;

    ui->textBrowser_2->setPlainText(outputTextRead);
}

void Application::on_lineEdit_editingFinished()
{
    bufferLengthRead = ui->lineEdit->text();
}

void Application::on_input_ImageDir1_clicked()
{
    QString filter = "Bitmap file (*.bmp) ;; All Files (*.*)";
    inputImageWrite = QFileDialog::getOpenFileName(this, "Select the image", QDir::currentPath(), filter);

    if (!inputImageWrite.size())  return;

    ui->textBrowser_9->setPlainText(inputImageWrite);
}

void Application::on_input_TxtFile_clicked()
{
    QString filter = "Text file (*.txt) ;; All Files (*.*)";
    inputTextWrite = QFileDialog::getOpenFileName(this, "Select the text file", QDir::currentPath(), filter);

    if (!inputTextWrite.size())  return;

    ui->textBrowser_10->setPlainText(inputTextWrite);
}

void Application::on_output_ImageDir_clicked()
{
    outputDirWrite = QFileDialog::getExistingDirectory(this, "Select the folder", QDir::homePath());

    if (!outputDirWrite.size())  return;

    ui->textBrowser_11->setPlainText(outputDirWrite);
}


void Application::on_exec_Read_clicked()
{
    if (!inputImageRead.size())
    {
        QMessageBox::critical(this, "Error", "<FONT COLOR='#ffffff'>Input Image file not specified.</FONT>");
        return;
    }

    if (!outputTextRead.size())
    {
        QMessageBox::critical(this, "Error", "<FONT COLOR='#ffffff'>Output Text File not specified.</FONT>");
        return;
    }

    if (!bufferLengthRead.size())
    {
        QMessageBox::critical(this, "Error", "<FONT COLOR='#ffffff'>Data length not specified.</FONT>");
        return;
    }

    // Read the image
    ReadMode();
    QMessageBox::information(this, "Sucess", "<FONT COLOR='#ffffff'>Data successfully written in text file.</FONT>");
}


void Application::on_exec_Write_clicked()
{
    if (!inputImageWrite.size())
    {
        QMessageBox::critical(this, "Error", "<FONT COLOR='#ffffff'>Input Image file not specified.</FONT>");
        return;
    }

    if (!inputTextWrite.size())
    {
        QMessageBox::critical(this, "Error", "<FONT COLOR='#ffffff'>Input Text File not specified.</FONT>");
        return;
    }

    if (!outputDirWrite.size())
    {
        QMessageBox::critical(this, "Error", "<FONT COLOR='#ffffff'>Output directory not specified.</FONT>");
        return;
    }

    // Write the image
}

void Application::ReadMode()
{
    std::string imageFileName = inputImageRead.toStdString();
    std::string textFileName = outputTextRead.toStdString();

    //std::replace(fileName.begin(), fileName.end(), '/', '\\');

    FILE* test2;
    fopen_s(&test2, textFileName.c_str(), "wb");
    assert(test2);

    size_t dataSize = 0;
    unsigned char* colorChannelData = Bitmap::ReadImage(imageFileName.c_str(), dataSize);

    size_t readBuffLength = bufferLengthRead.toULongLong();

    if (readBuffLength == 0 || readBuffLength > dataSize)
    {
        readBuffLength = dataSize;
    }

    // Because we increment colorIndex 4 times and 4 chunks of 2 bit make up 1 8-bit.
    // In short, it is incremented per pixel per color channel
    readBuffLength = ((readBuffLength / 4) * 4) / 4;

    size_t colorIndex = 0;
    // Process data and add it to separate txt file
    for (size_t i = 0; i < readBuffLength; i++)
    {
        // First character from embedData is read
        unsigned char bitChunk1 = colorChannelData[colorIndex];
        colorIndex++;
        unsigned char bitChunk2 = colorChannelData[colorIndex];
        colorIndex++;
        unsigned char bitChunk3 = colorChannelData[colorIndex];
        colorIndex++;
        unsigned char bitChunk4 = colorChannelData[colorIndex];
        colorIndex++;

        // Store necessary data in only last 2 bits and left shift to make them a character
        bitChunk1 = (bitChunk1 & 0b00000011) << 6;
        bitChunk2 = (bitChunk2 & 0b00000011) << 4;
        bitChunk3 = (bitChunk3 & 0b00000011) << 2;
        bitChunk4 = (bitChunk4 & 0b00000011);

        // Combine these bits now
        unsigned char character = (((bitChunk1 | bitChunk2) | bitChunk3) | bitChunk4);

        fwrite(&character, sizeof(unsigned char), 1, test2);
        //fprintf(test2, "%c", character);
    }

    fclose(test2);
}


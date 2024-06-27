#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Application;
}
QT_END_NAMESPACE

class Application : public QMainWindow
{
    Q_OBJECT

public:
    Application(QWidget *parent = nullptr);
    ~Application();

private slots:
    void on_mode_readBtn_clicked();

    void on_mode_writeBtn_clicked();

    void on_mode_AboutBtn_clicked();

    void on_input_ImageDir_clicked();

    void on_output_TxtFile_clicked();

    void on_input_ImageDir1_clicked();

    void on_input_TxtFile_clicked();

    void on_output_ImageDir_clicked();

    void on_lineEdit_editingFinished();

    void on_exec_Read_clicked();

    void on_exec_Write_clicked();

private:
    void ReadMode();
    void WriteMode();

private:
    Ui::Application *ui;

    QString inputImageRead = "";
    QString outputTextRead = "";
    QString bufferLengthRead = "";

    QString inputImageWrite = "";
    QString inputTextWrite = "";
    QString outputDirWrite = "";
};
#endif // APPLICATION_H

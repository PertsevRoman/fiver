#include <string>
#include <vector>
#include <exception>

#ifndef __MainWindow_h__
#define __MainWindow_h__

#include <QMainWindow>
#include <QButtonGroup>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QImageReader>
#include <QProgressBar>
#include <QLabel>

#include "DataModel.h"
#include "DocumentsAnalyser.h"

#include "optionsdialog.h"
#include "aboutdialog.h"

#include "codes.h"

class DataModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow  *ui;

    //Загрузчик изображения
    QNetworkAccessManager *imageNam;

    //Изображение для мастера заполнения
    QPixmap currentPixmap;

    //Недостающие виджеты
    QProgressBar    *progress;
    QLabel          *procInfo;
    QLabel          *dbInfo;

    DataModel*          mysqlDataModel;
    DocumentsAnalyser*  docAnalyser;

    OptionsDialog   *opts;
    AboutDialog     *about;

    std::pair<std::string, std::vector<std::string> > currentImageData;

    //Оценки тональности
    QMap<int, QString> voteDefends;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //Изменение размера
    void resizeEvent(QResizeEvent *);

    //Возвращает текущую оценку
    int getCurrentMark();

private:
    //Инициализирующие методы
    void init();
    void sets();
    void conn();

    void imageResize();

public slots:
    /**
     * @brief recieveMessage Принимает сообщение
     * @param err Код сообщения
     * @param msg Сообщение
     */
    void recieveMessage(Errors::ImgErrorCode err, QString msg);
    /**
     * @brief downloadProgress Отслеживает состояние загрузки ресурсов
     * @param bytes Пришло байт
     * @param total Всего байт
     */
    void downloadProgress(qint64 bytes, qint64 total);
    /**
     * @brief optionsChanged Вызывается при смене настроек
     */
    void stateChanged();

private slots:
    /**
     * @brief vocActionPerformed выбор действия для словаря
     */
    void vocActionPerformed();
    /**
     * @brief toneActionPerformed выбор действия для оценки тональности
     */
    void toneActionPerformed();
    /**
     * @brief viewImageInfo Загрузка изображения
     */
    void loadImage();
    /**
     * @brief imageLoaded Отображение изображения
     */
    void imageLoaded(QNetworkReply* reply);
    /**
     * @brief dbCheck Проверка соединения в базой данных
     */
    void dbCheck(bool ch);
    /**
     * @brief voteValChanged Установка слайдера оценки
     * @param val Значение
     */
    void voteValChanged(int val);
};

#endif

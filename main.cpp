#include <QApplication>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Создаем главное окно
    QWidget window;
    window.setWindowTitle("Telegram Opener");
    window.setFixedSize(300, 200);
    
    // Создаем кнопку
    QPushButton button("Открыть Telegram", &window);
    button.setGeometry(75, 75, 150, 50);
    
    // Подключаем обработчик нажатия
    QObject::connect(&button, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("https://t.me/chillrok"));
    });
    
    window.show();
    return app.exec();
}
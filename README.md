# Начало
Всем привет! Наша команда создала свой музыковой сервис, по функционалу похожий на небезызвестный всем Spotify. 
Наше приложение не поддерживает воспроизведение музыки, но позволяет пользователям просматривать, искать и анализировать плейлисты и тексты песен. А также создание собственных плейлистов и еще кое-что)
Приложение построено с использованием Qt Creator.

## Наши уникальные features

1) Аутентификация пользователей: Регистрация и вход с использованием имени и пароля каждого нового пользователя, а также сохранение этих данных в формате хэширования.
2) Просмотр плейлистов: Просмотр плейлистов по жанрам.
3) Поиск треков: Поиск треков в плейлистах с подсказками.
4) Парсинг текстов песен: Получение текстов песен для выбранных треков с сайта genius.com 
5) Избранные треки: Управление и просмотр избранных треков в собственном плейлисте.
6) Кластеризация: пользователю будут показаны рекомендованные треки.

## Необходимые компоненты

Убедитесь, что у вас установлены следующие компоненты и приложения:

- Qt Creator
- Компилятор C++

### Установка приложения

1. Главным шагом является клонирование нашего [репозитория](https://github.com/Texvss/Spotify_project), после чего проект откроется в вашем QT Creator



2. Добавьте файлы ресурсов:

Убедитесь, что следующий файл добавлен в систему ресурсов Qt (.qrc файл):

playlist_2010to20222Mish.csv - это основной датасет. Остальные 2 файла нужно скачать на устройство и скопировать их путь, после чего вставить пути в проект. Это файлы draft.db и run.sh. На данном этапе draft.db и run.sh нужно запросить напрямую у авторов приложения.


#### Структура проекта

 1) main.cpp: Точка входа в приложение, выполняет начальную настройку и главный цикл обработки событий.
 2) mainwindow.cpp / mainwindow.h: Главное окно приложения, содержит основной пользовательский интерфейс и логику для просмотра плейлистов и получения текстов песен.
 3) login.cpp / login.h: Обработка аутентификации пользователей (вход и регистрация).
 4) lyrics.cpp / lyrics.h: Управление отображением полученных текстов песен.
 5) spotify.cpp / spotify.h: Обработка данных, связанных с функционалом, похожим на Spotify (например, фильтрация треков по жанру).
 6) trackview.cpp / trackview.h: Управление отображением треков в плейлисте.
 7) liked.cpp / liked.h: Управление избранными треками пользователя.







##### Не стесняйтесь настраивать этот файл README в соответствии с конкретными деталями и требованиями вашего проекта.
```


Этот файл README.md даст пользователям полное представление о проекте, его функционале, структуре и том, как его установить и использовать.


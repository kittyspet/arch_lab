workspace {
    name "Сайт конференции"
    description "Простая система управления своими конференциями и размещение докладов в них"

    # включаем режим с иерархической системой идентификаторов
    !identifiers hierarchical
    
    # Модель архитектуры
    model {

        # Настраиваем возможность создания вложенных груп
        properties { 
            structurizr.groupSeparator "/"
        }
        

        # Описание компонент модели
        user = person "Пользователь сайта конференции"
        conf_web = softwareSystem "Сайт конференций" {
            description "Позволяет пользователям создавать и добавлять доклады в конференцию, а также получать информацию о докладах и других пользователях"

            web_browser = container "Web Browser" {
                description "Позволяет пользователю управлять конференцией через свой веб-браузер"
            }
	
	api_app = container "API Application" {
                description "Осуществляет операции, запрашиваемые пользователем"
            }


                       group "Слой данных" {
                user_database = container "User Database" {
                    description "База данных с пользователями"
                    technology "PostgreSQL 15"
                    tags "database"
                }

                report_database = container "Report Database" {
                    description "База данных докладов"
                    technology "PostgreSQL 15"
                    tags "database"
                }

                conference_database = container "Conference Database" {
                    description "База данных конференций"
                    technology "PostgreSQL 15"
                    tags "database"
                }
            }
          api_app -> user_database "Получение/изменение данных о пользователе" "TCP 5432"
          api_app -> report_database "Получение/изменение данных о докладе" "TCP 5432"
          api_app -> conference_database "Получение/изменение данных о конференции" "TCP 5432"


            

            user -> web_browser "Управление конференцией" "HTTPS:443"
            web_browser -> api_app "Создание/изменение/получение данных" "REST/HTTP :80"

        }

        user -> conf_web "Управление конференциями" "HTTPS:443"
        

        deploymentEnvironment "Production" {
            deploymentNode "User’s Computer" {
                technology "Microsoft Windows or Apple macOS"
                deploymentNode "Web Browser" {
                    technology "Chrome, Firefox, Safari, Edge, Yandex"
                    containerInstance conf_web.web_browser
                                            }
            }

            DeploymentNode "Conf-api Server"  {
                technology "Ubuntu 22.04 LTS"
DeploymentNode "Apache Tomcat" {
    technology "Apache Tomcat 10.x"
containerInstance conf_web.api_app

}
}

   DeploymentNode "Conf-db Server" {
    technology "Ubuntu 22.04 LTS" 
DeploymentNode "Postgres" {
    technology "PostgreSQL 15"
containerInstance conf_web.user_database
containerInstance conf_web.report_database
containerInstance conf_web.conference_database

}




            }
            
        }
    }

    views {
        themes default

        properties { 
            structurizr.tooltips true
        }


        !script groovy {
            workspace.views.createDefaultViews()
            workspace.views.views.findAll { it instanceof com.structurizr.view.ModelView }.each { it.enableAutomaticLayout() }
        }

        dynamic conf_web "UC01" "Создание нового пользователя" {
            autoLayout
            user -> conf_web.web_browser "Создать нового пользователя"
            conf_web.web_browser -> conf_web.api_app "Вызов создания нового пользователя (POST /user)"
            conf_web.api_app -> conf_web.user_database "Сохранить данные о новом пользователе" 
        }

        dynamic conf_web "UC02" "Поиск пользователя по логину" {
            autoLayout
            user -> conf_web.web_browser "Найти пользователя по логину"
            conf_web.web_browser -> conf_web.api_app " Вызов поиска пользователя по логину (GET /user) "
            conf_web.api_app -> conf_web.user_database "Найти пользователя по логину" 
        }

        dynamic conf_web "UC03" "Поиск пользователя по маске имени и фамилии" {
            autoLayout
            user -> conf_web.web_browser "Найти пользователя по маске имени и фамилии"
            conf_web.web_browser -> conf_web.api_app "Вызов поиска пользователя по маске имени и фамилии (GET /user) "
            conf_web.api_app -> conf_web.user_database "Найти пользователя по маске имени и фамилии" 
        }

dynamic conf_web "UC04" "Создание нового доклада" {
            autoLayout
            user -> conf_web.web_browser "Создать новый доклад"
            conf_web.web_browser -> conf_web.api_app "Вызов создания нового доклада (POST /report)"
            conf_web.api_app -> conf_web.report_database "Сохранить данные о новом докладе" 
        }

dynamic conf_web "UC05" "Получение списка всех докладов" {
            autoLayout
            user -> conf_web.web_browser "Получить список всех докладов"
            conf_web.web_browser -> conf_web.api_app "Вызов списка всех докладов (GET /report)"
            conf_web.api_app -> conf_web.report_database "Получить список всех докладов" 
        }
dynamic conf_web "UC06" "Добавление доклада в конференцию" {
            autoLayout
            user -> conf_web.web_browser "Добавить доклад в конференцию"
            conf_web.web_browser -> conf_web.api_app "Запрос добавления доклада в конференцию (POST /report)"
            conf_web.api_app -> conf_web.report_database "Найти доклад по id" 
            conf_web.api_app -> conf_web.conference_database "Сохранить данные о докладе" 

        }

dynamic conf_web "UC07" "Получение списка докладов в конференции" {
            autoLayout
            user -> conf_web.web_browser " Получить список докладов в конференции "
            conf_web.web_browser -> conf_web.api_app "Запрос списка докладов в конференции (GET /report) "
            conf_web.api_app -> conf_web.report_database "Найти доклад по id" 
            conf_web.api_app -> conf_web.conference_database "Получить данные о докладах в конференции" 

        }


        styles {
            element "database" {
                shape cylinder
            }
        }
    }
}

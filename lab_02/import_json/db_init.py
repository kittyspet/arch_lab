import pandas as pd
from sqlalchemy import create_engine, MetaData, Table, Column, Integer, String, Index, DateTime
from sqlalchemy.sql import func

engine = create_engine("postgresql+psycopg2://root:root@lab_02_postgres/archdb", echo = False)

# Создаем объект MetaData
metadata = MetaData()


users_table = Table(
    'users', metadata,
    Column('id', Integer, primary_key=True),
    Column('first_name', String),
    Column('last_name', String),
    Column('email', String),
    Column('login', String),
    Column('password', String),
    extend_existing=True
)

#index1 = Index('ind', users_table.c.first_name)

confs_table = Table(
    'confs', metadata,
    Column('id', Integer, primary_key=True),
    Column('conf_name', String),
    Column('owner', Integer),
    Column('password', String),
    extend_existing=True
)

#index2 = Index('ind', confs_table.c.conf_name)

reports_table = Table(
    'reports', metadata,
    Column('id', Integer, primary_key=True),
    Column('report_name', String),
    Column('conf_id', Integer),
    Column('text', String),
    extend_existing=True
)

#index3 = Index('ind', reports_table.c.report_name)

# Создаем таблицу в базе данных (если ее нет)
metadata.create_all(engine)


# Загружаем данные из JSON
df1 = pd.read_json(r"import_json/JsonUser.json")
df2 = pd.read_json(r"import_json/JsonConf.json")
df3 = pd.read_json(r"import_json/JsonReport.json")

# Добавляем столбец login
df1['login'] = df1['email'].apply(lambda x: x.split('@')[0])

# Сохраняем данные в таблицу 'users' в базе данных
df1.to_sql("users", con=engine, if_exists='append', index=False)
df2.to_sql("confs", con=engine, if_exists='append', index=False)
df3.to_sql("reports", con=engine, if_exists='append', index=False)

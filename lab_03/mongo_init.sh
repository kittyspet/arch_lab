export DB_HOST=lab_03_postgres
export DB_PORT=5432
export DB_LOGIN=root
export DB_PASSWORD=root
export DB_DATABASE=archdb

mongoimport --uri 'mongodb://lab03_mongo:27017/arch' --collection confs --file import_json/ConfsJson.json --jsonArray
mongoimport --uri 'mongodb://lab03_mongo:27017/arch' --collection reports --file import_json/ReportsJson.json --jsonArray

python3 -m venv ./env &&
source ./env/bin/activate &&
pip3 install -r import_json/requirements.txt &&
python3 "import_json/db_init.py"


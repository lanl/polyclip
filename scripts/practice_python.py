import sqlite3
def query(db):
	database = sqlite3.connect(db)
	cursor = database.cursor()
	cursor.execute(f"SELECT * FROM 'NVTX_EVENTS' WHERE text = 'POPULATING CLIPS';")
	tables = cursor.fetchall()
	print(tables)
# 	for table in tables:
# 		table_name = table[0]
# 		cursor.execute(f"SELECT * FROM {table_name}");
# 		records = cursor.fetchall()
# #		print(records)
# 		for record in records:
# 			print(record)
#
	cursor.close()
	database.close()
	return tables

database = "../cmake-build-debug/report6.sqlite"
tables = query(database)
print(tables)

import sqlite3
import sys

if len(sys.argv) < 2:
	print("Provide the sqlite file as an argument.")
	exit(1)
file_name = sys.argv[1]

list_of_annotations = ['POPULATING CLIPPED PARTS', 'CLIPPING BELOW CELLS', 'MESH: GPU-TO-CPU TRANSFER', 'CLIPPED PART: GPU-TO-CPU TRANSFER']


def query(db):
	database = sqlite3.connect(db)
	cursor = database.cursor()

	for annotation in list_of_annotations:
		cursor.execute(f"SELECT end-start AS 'duration' FROM 'NVTX_EVENTS' WHERE text = '{annotation}';")
		print(cursor.fetchall())
	# print("Description!")
	# print(cursor.description)
	# field_names = [i[0] for i in cursor.description]
	# 	tables = cursor.fetchall()
	# 	start = tables[0][0]
	# 	end = tables[0][1]
	# 	with open(annotation + '.txt', 'a') as file:
	# 		time = end - start
	# 		file.write(str(time) + "\n")

	cursor.close()
	database.close()


query(file_name)

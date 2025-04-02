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
		cursor.execute(f"SELECT start, end FROM 'NVTX_EVENTS' WHERE text = '{annotation}';")
	# print("Description!")
	# print(cursor.description)
	# field_names = [i[0] for i in cursor.description]
		tables = cursor.fetchall()
		start = tables[0][0]
		end = tables[0][1]
		with open(annotation + '.txt', 'a') as file:
			time = end - start
			file.write(str(time) + "\n")



	# print(start)
	# print(end)
	# print("POPULATING CLIPPED PARTS - Diff is " + str(end - start) + " nanoseconds\n")

	# cursor.execute(f"SELECT start, end FROM 'NVTX_EVENTS' WHERE text = 'MESH: CPU-TO-GPU Transfer';")
	# tables = cursor.fetchall()
	# start = tables[0][0]
	# end = tables[0][1]
	#
	# print(start)
	# print(end)

	# cursor.execute(f"SELECT start, end FROM 'NVTX_EVENTS' WHERE text = 'CLIPPING BELOW CELLS';")
	# tables = cursor.fetchall()
	# start = tables[0][0]
	# end = tables[0][1]
	#
	# print(start)
	# print(end)
	# print("CLIPPING BELOW CELLS - Diff is " + str(end - start) + " nanoseconds\n")
	#
	#
	# cursor.execute(f"SELECT start, end FROM 'NVTX_EVENTS' WHERE text = 'MESH: GPU-TO-CPU TRANSFER';")
	# tables = cursor.fetchall()
	# start = tables[0][0]
	# end = tables[0][1]
	#
	# print(start)
	# print(end)
	# print("MESH: GPU-TO-CPU TRANSFER - Diff is " + str(end - start) + " nanoseconds\n")
	#
	# cursor.execute(f"SELECT start, end FROM 'NVTX_EVENTS' WHERE text = 'CLIPPED PART: GPU-TO-CPU TRANSFER';")
	# tables = cursor.fetchall()
	# start = tables[0][0]
	# end = tables[0][1]
	#
	# print(start)
	# print(end)
	# print("CLIPPED PART: GPU-TO-CPU TRANSFER - Diff is " + str(end - start) + " nanoseconds\n")

	cursor.close()
	database.close()
	return tables

# database = "../cmake-build-debug/report6.sqlite"
tables = query(file_name)
#print(tables)

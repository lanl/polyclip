import sqlite3
import sys
import matplotlib.pyplot as plt
if len(sys.argv) < 2:
	print("Provide the sqlite file as an argument.")
	exit(1)
file_name = sys.argv[1]

sql_files = ["inmesh_1.sqlite",
			 "inmesh_2.sqlite",
			 "inmesh_3.sqlite",
			 "inmesh_4.sqlite",
			 "inmesh_5.sqlite"]

dictionary_values = {}
list_of_annotations = [
	'TOTAL RUNTIME',
	'GENERATING MESH',
	'INIT LINE INTERFACE',
	'CLIPPING BELOW CELLS',
	'MESH: GPU-TO-CPU TRANSFER',
	'CLIPPED PART: GPU-TO-CPU TRANSFER'
]



def query(db, basename):
	database = sqlite3.connect(db)
	cursor = database.cursor()

	list_of_values = []
	for annotation in list_of_annotations:
		cursor.execute(f"SELECT end-start AS 'duration' FROM 'NVTX_EVENTS' WHERE text = '{annotation}';")
		table = cursor.fetchall()
		list_of_values.append(table[0][0])



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
	return list_of_values


def get_avg_time():
	pass


def main():
	for file in sql_files:
		database = sqlite3.connect(file)
		cursor = database.cursor()
		temp_dictionary = {}
		for annotation in list_of_annotations:
			cursor.execute(f"SELECT end-start AS 'duration' FROM 'NVTX_EVENTS' WHERE text = '{annotation}';")
			table = cursor.fetchall()
			print(table)
			print("Annotation - " + annotation)
			temp_dictionary[annotation] = table[0][0]
			if file in dictionary_values:
				dictionary_values[file].append(temp_dictionary)
			else:
				dictionary_values[file] = [temp_dictionary]

		aggregate_values = {}
		for key in list_of_annotations:
			aggregate_values[key] = 0

		for dict in dictionary_values.values():
			for entry in dict:
				for key, value in entry.items():
					aggregate_values[key] += value

		total_runtime = aggregate_values["TOTAL RUNTIME"]
		labels = []
		sizes = []
		remaining_runtime = 0
		for annotation in list_of_annotations:
			if annotation != "TOTAL RUNTIME":
				value = aggregate_values[annotation]
				labels.append(f"{annotation} ({(value / total_runtime) * 100}%)")
				sizes.append(value/total_runtime)
				remaining_runtime += value

		other_time = total_runtime - remaining_runtime

		labels.append(f"Other ({(other_time / total_runtime) * 100}%)")
		sizes.append(other_time/total_runtime)

		plt.figure(figsize=(7,7))
		plt.pie(sizes, labels=labels, autopct=lambda p: f'{p:.1f}%', startangle = 90)
		plt.title("Runtime analysis for Inmesh")
		plt.axis("equal")
		plt.tight_layout()
		plt.savefig("inmesh.png")
		plt.close()


main()
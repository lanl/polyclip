import sqlite3
import sys
import matplotlib
matplotlib.use('Agg')

import matplotlib.pyplot as plt
if len(sys.argv) < 2:
	print("Provide the gmvfile base name")
	exit(1)
file_name = sys.argv[1]

# sql_files = ["inmesh_1.sqlite",
# 			 "inmesh_2.sqlite",
# 			 "inmesh_3.sqlite",
# 			 "inmesh_4.sqlite",
# 			 "inmesh_5.sqlite"]
sql_files = [f"{file_name}_{i}.sqlite" for i in range(1, 6)]
dictionary_values = {}
list_of_annotations = [
	# 'TOTAL RUNTIME',
	'GENERATING MESH',
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


def generate_pie_chart():
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
		total_runtime = 0
		for key in list_of_annotations:
			aggregate_values[key] = 0

		for dict in dictionary_values.values():
			for entry in dict:
				for key, value in entry.items():
					aggregate_values[key] += value
					total_runtime += value

		labels = []
		sizes = []
		remaining_runtime = 0
		for annotation in list_of_annotations:
			value = aggregate_values[annotation]
			labels.append(f"{annotation}")
			sizes.append(value/total_runtime)
			remaining_runtime += value

		plt.figure(figsize=(25,25))
		plt.pie(sizes, labels=labels, autopct=lambda p: f'{p:.1f}%', startangle = 90)
		plt.title("Runtime analysis for Inmesh")
		plt.axis("equal")
		plt.tight_layout()
		plt.savefig("inmesh.png")
		plt.close()

def generate_bar_chart():
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
		total_runtime = 0
		for key in list_of_annotations:
			aggregate_values[key] = 0

		for dict in dictionary_values.values():
			for entry in dict:
				for key, value in entry.items():
					aggregate_values[key] += value
					total_runtime += value

		labels = []
		values = []
		for annotation in list_of_annotations:
			value = aggregate_values[annotation]
			labels.append(f"{annotation}")
			values.append(value/total_runtime)

		plt.figure(figsize=(14,8))
		plt.bar(labels, values, color='red')
		plt.title("Runtime analysis for Inmesh")
		plt.ylabel("Runtime")
		plt.xticks(rotation=45, ha='right')
		plt.tight_layout()
		plt.savefig("inmesh_bar.png")
		plt.close()


def main():
	generate_pie_chart()
	generate_bar_chart()




main()
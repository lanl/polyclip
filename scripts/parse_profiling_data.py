# (c) 2025. Triad National Security, LLC. All rights reserved.
# This program was produced under U.S. Government contract 89233218CNA000001
# for Los Alamos National Laboratory (LANL), which is operated by Triad National
# Security, LLC for the U.S. Department of Energy/National Nuclear Security
# Administration. All rights in the program are reserved by Triad National
# Security, LLC, and the U.S. Department of Energy/National Nuclear Security
# Administration. The Government is granted for itself and others acting on its
# behalf a nonexclusive, paid-up, irrevocable worldwide license in this material
# to reproduce, prepare derivative works, distribute copies to the public,
# perform publicly and display publicly, and to permit others to do so.

import sqlite3
import os
import sys
import matplotlib
matplotlib.use('Agg')

import matplotlib.pyplot as plt
if len(sys.argv) < 2:
	print("Provide the gmvfile base name")
	exit(1)
file_name = sys.argv[1]
end_index = int(sys.argv[2]) if len(sys.argv) > 2 else 5

sql_files = [f"{file_name}_{i}.sqlite" for i in range(1, end_index + 1)]
dictionary_values = {}
list_of_annotations = [
	# 'TOTAL RUNTIME',
	'GENERATING MESH',
	'CLIPPING BELOW CELLS',
	'MESH: GPU-TO-CPU TRANSFER',
	'CLIPPED PART: GPU-TO-CPU TRANSFER'
]


def generate_pie_chart():
	for file in sql_files:
		# print(file)
		database = sqlite3.connect(file)
		cursor = database.cursor()
		temp_dictionary = {}
		for annotation in list_of_annotations:
			cursor.execute(f"SELECT end-start AS 'duration' FROM 'NVTX_EVENTS' WHERE text = '{annotation}';")
			table = cursor.fetchall()
			# print(table)
			# print("Annotation - " + annotation)
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
		base_name = os.path.basename(file_name)
		output_path = f"output/images/{base_name}_pie.png"
		plt.savefig(output_path)
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
			values.append(value/end_index)

		plt.figure(figsize=(14,8))
		plt.bar(labels, values, color='red')
		plt.title("Runtime analysis for Inmesh")
		plt.ylabel("Runtime")
		plt.xticks(rotation=45, ha='right')
		plt.tight_layout()
		base_name = os.path.basename(file_name)
		output_path = f"output/images/{base_name}_bar.png"
		plt.savefig(output_path)
		plt.close()

def process_single_file(file):
	database = sqlite3.connect(file)
	cursor = database.cursor()
	annotation_durations = {}

	total_runtime = 0
	for annotation in list_of_annotations:
		cursor.execute(f"SELECT end-start AS 'duration' FROM 'NVTX_EVENTS' WHERE text = ?;", (annotation,))
		table = cursor.fetchall()
		if table:
			duration = table[0][0]
		else:
			duration = 0
		annotation_durations[annotation] = duration
		total_runtime += duration

	labels = []
	sizes = []
	for annotation, duration in annotation_durations.items():
		if duration > 0:
			labels.append(f"{annotation} ({duration:.2f}s)")
			sizes.append(duration)

	# Pie Chart
	plt.figure(figsize=(10, 10))
	plt.pie(sizes, labels=labels, autopct=lambda p: f'{p:.1f}%', startangle=90)
	plt.title(f"Single File Runtime: {os.path.basename(file)}")
	plt.axis("equal")
	plt.tight_layout()
	os.makedirs("output/images", exist_ok=True)
	plt.savefig(f"output/images/{os.path.splitext(os.path.basename(file))[0]}_pie.png")
	plt.close()

	# Bar Chart
	plt.figure(figsize=(14, 8))
	plt.bar(annotation_durations.keys(), [v / total_runtime for v in annotation_durations.values()], color='blue')
	plt.title(f"Single File Bar Chart: {os.path.basename(file)}")
	plt.ylabel("Fraction of Total Runtime")
	plt.xticks(rotation=45, ha='right')
	plt.tight_layout()
	plt.savefig(f"output/images/{os.path.splitext(os.path.basename(file))[0]}_bar.png")
	plt.close()



def main():
	generate_pie_chart()
	generate_bar_chart()




main()

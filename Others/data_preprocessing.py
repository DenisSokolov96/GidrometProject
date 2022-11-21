import os
import re
import sys
import csv


# Данный скрипт предобрабатывает данные показаний метеостанций с сайта rp5.ru для дальнейшей загрузки в базу данных PostgreSQL
path_to_files = "/home/user/Desktop/Кокшаров/hydrosystem-ERA/source_parsing/Data (rp5)/" # Путь до данных, скачанных с rp5.ru
new_path_to_files = "/home/user/Desktop/Кокшаров/hydrosystem-ERA/source_parsing/Data (rp5) new/" # Путь до места сохранения обработанных файлов

#for filename in os.listdir(new_path_to_files):
#    os.remove(new_path_to_files + filename)
#os.rmdir(new_path_to_files)

try:
    os.mkdir(new_path_to_files)
except FileExistsError as e:
    pass

for filename in os.listdir(path_to_files):
    meteostation_id = filename.split(".")[0]
    new_filename = meteostation_id + ".csv"

    used_fieldnames = ["id", "Местное время", "T", "U", "DD", "RRR", "Q", "sss"] # Значений для параметра Q в этом источнике (rp5.ru) нет

    with open(path_to_files + filename) as input_csvfile:
        info = input_csvfile.readlines()
        for i in range(6):
            info.pop(0)
        info[0] = info[0].replace(re.findall("\"(Местное время .*?)\".*", info[0])[0], "Местное время")

        csv_reader = csv.DictReader(info, delimiter=";")

    with open(new_path_to_files + new_filename, "w") as output_csvfile:
        csv_writer = csv.DictWriter(output_csvfile, delimiter=";", fieldnames=used_fieldnames)
        csv_writer.writeheader()
        for row in csv_reader:
            for key in [None, "Po", "P", "Pa", "Ff", "ff10", "ff3", "N", "WW", "W1", "W2", "Tn", "Tx", "Cl", "Nh", "H", "Cm", "Ch", "VV", "Td", "tR", "E", "Tg", "E'"]:
                row.pop(key)
            row["id"] = meteostation_id
            csv_writer.writerow(row)
    




#print(pathlib.Path.cwd()) # Может пригодиться

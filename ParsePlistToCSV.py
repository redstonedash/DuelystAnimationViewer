import xml.etree.ElementTree as ET
import os
from pathlib import Path
dir_path = os.path.dirname(os.path.realpath(__file__))
print(dir_path)
for file in os.listdir(dir_path):
    if file.endswith(".plist"):
        c_code = ''
        name = Path(file).stem
        sprite_sheet_path = os.path.join(dir_path, file)
        mytree = ET.parse(sprite_sheet_path)
        root = mytree.getroot();
        frames_list = root[0][1].findall('key')
        dict_list = root[0][1].findall('dict')
        last_anim = ''
        for frame in frames_list:
            frame_name = frame.text
            animation_name = frame_name[len(name):-7]
            #frame_number = frame_name[-7:-4]
            ix = frames_list.index(frame)
            frame_data = dict_list[ix].find("string").text
            if animation_name != last_anim:
                if last_anim == '':
                    c_code = c_code + animation_name
                else:
                    c_code = c_code +  "\n" + animation_name
            last_anim = animation_name
            c_code = c_code + "," + frame_data
        with open(f"{name}.txt", "w") as file:
            c_code = c_code.replace("{", "").replace("}", "")
            file.write(c_code)
print("File written successfully")
input("Press Enter to continue...")

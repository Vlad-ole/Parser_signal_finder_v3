'''
Created on 14 ���. 2019 �.

@author: oleynikov
'''

import subprocess

path_common = 'D:\\git_repositories\\Parser_signal_finder_v3\\'
sub_path = "Parser_signal_finder_v3\\Release\\"
program_name = "Parser_signal_finder_v3.exe"
full_name = path_common + sub_path + program_name
print(full_name)

#th_list = [30,35,40,45,50,55,60,65,70,75,80]
th_list = [100,120,140,160,180,200,300,400,500,600,700,800,900,1000]

for x in th_list:
    subprocess.call([full_name, str(x)])


HIRA_ROOT=/home/backlight/Workspace/HIRA

input_source_absolute_path=$(pwd)/$1

if (( $# > 1 )); then
input_source_absolute_path=$1
fi

input_source_directory=${input_source_absolute_path%/*}
input_source_filename=${input_source_absolute_path##*/}
input_source_filename_prefix=${input_source_filename%.*}
input_source_filename_suffix=${input_source_filename##*.}

output_source_absolute_path=${input_source_directory}/${input_source_filename_prefix}_submission.cpp
output_binary_absolute_path=${input_source_directory}/${input_source_filename_prefix}.bin

# echo 1 ${input_source_absolute_path}
# echo 2 ${input_source_directory}
# echo 3 ${input_source_filename}
# echo 4 ${input_source_filename_prefix}
# echo 5 ${input_source_filename_suffix}
# echo 6 ${output_source_absolute_path}

gcc ${input_source_absolute_path} -E -DPREPROCESSING -I${HIRA_ROOT} -o ${output_source_absolute_path}
sed -i '1i\#include <bits/stdc++.h>' ${output_source_absolute_path}

g++ ${output_source_absolute_path} -O2 -std=c++17 -o ${output_binary_absolute_path}

cp ${output_source_absolute_path} /home/backlight/submission.cpp

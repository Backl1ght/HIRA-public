input_absolute_path=$(pwd)/$1
input_directory=${input_absolute_path%/*}
input_filename=${input_absolute_path##*/}
input_filename_prefix=${input_filename%.*}
input_filename_suffix=${input_filename##*.}

output_absolute_path=${input_directory}/${input_filename_prefix}.i

# echo 1 ${input_absolute_path}
# echo 2 ${input_directory}
# echo 3 ${input_filename}
# echo 4 ${input_filename_prefix}
# echo 5 ${input_filename_suffix}
# echo 6 ${output_absolute_path}

gcc ${input_absolute_path} -E -DPREPROCESSING  -o ${output_absolute_path}
sed -i '1i\#include <bits/stdc++.h>' ${output_absolute_path}

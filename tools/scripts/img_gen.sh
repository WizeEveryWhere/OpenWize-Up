#!/bin/bash 

#*******************************************************************************
function help_me
{
cat << EOF
#######################################################
***             Build tha bootstrap                 ***
mkdir -p _bootstrap_build
cd _bootstrap_build
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_CFG=Bootstrap ../. 
make -j4
make install

#######################################################
***          Build the application + nvm            ***
mkdir -p _build
cd _build
cmake -DCMAKE_BUILD_TYPE=Debug -DGENERATE_PARAM=ON -DBUILD_NVM_BINARY=ON ../.
make -j4
make install

#######################################################
***            Generate Img 0 or 1                  ***
From "OpenWize-Up" directory

# TODO : It will be integrate in the git repo ....
# export PATH=\$PATH:\$(pwd)/tools/scripts
# ...but for now just call the local script

To generate img_0.bin
./img_gen.sh ./_install

To generate img_1.bin
./img_gen.sh ./_install 1

EOF
}

#*******************************************************************************
declare -A info=(
    #---   
    [bootstrap_file]=x
    [_B_org_]=x
    [_I0_org_]=x
    [_I0_size_]=x
    [_I1_org_]=x
    [_I1_size_]=x
    [_magic_I0_]=x
    [_magic_I1_]=x
    [_magic_dead_]=deadc0de
    #[_magic_dead_]=DEADC0DE
    #[__end_prog]=x
    #---
);
search="_B_org_ _I0_org_ _I0_size_ _I1_org_ _I1_size_ _magic_I0_ _magic_I1_";
img_id=0;

declare -A mLocal=(
    #---   
    [app_file]=x
    [app_size]=x
    [img_file]=x
    [img_size]=x
    [header_size]=512
    [le_hex_sz]=x
    [le_magic]=x
    [le_magic_dead]=x    
    #---
);

# ---------------------------------------------------
function get_part_address 
{
    local cmd="arm-none-eabi-nm ${info[bootstrap_file]}";
    
    # check variable
    if [[ ${info[bootstrap_file]} == "" ]]
    then
        echo "The bottstrap elf file is missing...";
        exit 0;
    fi
    
    # check if file exist
    if [[ ! -f ${info[bootstrap_file]} ]]
    then
        echo "File \"${info[bootstrap_file]}\" not found";
        exit 0;
    fi
    
    # get info
    for k in ${!info[@]}
    do
        res=($($cmd | grep "$k"));
        if [[ "${res[0]}" != "" ]]
        then 
            info[$k]=${res[0]};
            #printf "%-15s= 0x%s\n" $k ${info[$k]};
        else
            x=0;
            #printf "%-15s= not found \n" $k;
        fi
    done
}

# ---------------------------------------------------
function get_app_info
{
    # check variable
    if [[ ${mLocal[app_file]} == "" ]]
    then
        echo "The application binary file is missing...";
        exit 0;
    fi

    # check if file exist
    if [[ ! -f ${mLocal[app_file]} ]]
    then
        echo "File \"${mLocal[app_file]}\" not found";
        exit 0;
    fi

    # get file size
    app_size=$(stat -c %s ${mLocal[app_file]});
    mLocal[app_size]=${app_size};
    
    # add header size (512 bytes)
    app_size=$(( ${app_size} + ${mLocal[header_size]} )); 
    
    # add magic dead size (4 bytes)
    app_size=$(( ${app_size} + 4 )); 
    mLocal[img_size]=${app_size};
    
    # convert in LE
    hex_sz=$(printf "%08x\n" "${app_size}");
    le_hex_sz=$( echo ${hex_sz} | rev | dd conv=swab 2>/dev/null);
    mLocal[le_hex_sz]=${le_hex_sz};
    
    
    # create header
    local img_magic=_magic_I${img_id}_;
    le_magic=$( echo ${info[${img_magic}]} | rev | dd conv=swab 2>/dev/null);
    mLocal[le_magic]=${le_magic};
    
    le_magic_dead=$( echo ${info[_magic_dead_]} | rev | dd conv=swab 2>/dev/null);
    mLocal[le_magic_dead]=${le_magic_dead};
            
    # get in/out directory
    local app_dir=$(dirname ${mLocal[app_file]});
    # set output filename
    mLocal[img_file]=${app_dir}/"img_${img_id}.bin";
}

# ---------------------------------------------------
function img_bin_gen
{
    local img_file=${mLocal[img_file]};
    
    # write header
    echo "-> Write in file ${img_file}";
    # ---
    local seek=0;
    echo "   -> Add magic ${mLocal[le_magic]}";
    echo ${mLocal[le_magic]} | xxd -r -p | dd of=${img_file} obs=4 count=1 conv=notrunc 2>/dev/null ;

    # ---
    seek=$(( ${seek} + 1 ));
    echo "   -> Add size ${mLocal[le_hex_sz]}";
    echo ${mLocal[le_hex_sz]} | xxd -r -p | dd of=${img_file} obs=4 count=1 seek=${seek} conv=notrunc 2>/dev/null ;


    # ---
    padding=FFFFFFFF;
    #nb_padding=$(( 32 / 4 - 2))
    nb_padding=$(( ${mLocal[header_size]} / 4 - 2))
    echo "   -> Add padding ${nb_padding} x ${padding} ";
    for (( c=0; c<${nb_padding}; c++ ))
    do 
        seek=$(( ${seek} + 1 ));
        echo ${padding} | xxd -r -p | dd of=${img_file} obs=4 count=1 seek=${seek} conv=notrunc 2>/dev/null ;
    done

    # write bin
    fName=$(basename ${mLocal[app_file]});
    seek=$(( ${seek} + 1 ));
    echo "   -> Add bin ${fName}";
    dd if=${mLocal[app_file]} of=${img_file} obs=4 seek=${seek} 2>/dev/null ;

    # write magic dead
    seek=$(( ${mLocal[img_size]} - 4  )); 
    echo "   -> Add magic dead ${mLocal[le_magic_dead]}";
    echo ${mLocal[le_magic_dead]} | xxd -r -p | dd of=${img_file} obs=1 count=1 seek=${seek} conv=notrunc 2>/dev/null ;
}

#*******************************************************************************

function show_me_input
{
    echo "************************";
    echo "-> Bootstrap file : ${info[bootstrap_file]}";
    for k in ${!info[@]}
    do
        if [[ $k != bootstrap_file ]]
        then
            printf "%-15s= 0x%s" $k ${info[$k]};
            
            if [[ $k == _magic_I${img_id}_ ]]
            then
                printf " * " $k ${info[$k]};
            fi
            if [[ $k == _I${img_id}_org_ ]]
            then
                printf " * " $k ${info[$k]};
            fi
            printf "\n";
        fi
    done
    
    echo "************************";
    echo "-> App file : ${mLocal[app_file]}";
    echo "-> App size : ${mLocal[app_size]}";
    echo "-> Header size : ${mLocal[header_size]}";
    echo "-> Magic word (LE) : 0x ${mLocal[le_magic]}";
    echo "-> Magic dead (LE) : 0x ${mLocal[le_magic_dead]}";    
    echo "-> Image file (out) : ${mLocal[img_file]}";
    echo "-> Image size : ${mLocal[img_size]} (0x ${mLocal[le_hex_sz]})";
    echo "************************";
}

# ---------------------------------------------------
function show_me_output
{
    local img_file=${mLocal[img_file]};
    echo "************************";
    # Show res
    echo "   *** result  ****"
    local show_me=0;
    
    echo "   -> At start of file"
    hexdump -n64 -s${show_me} -e '"%08_ax" 16/1 " %02x" "\n"' ${img_file};
    echo ""

    echo "   -> Between header and app "
    show_me=$(( ${mLocal[header_size]} - 16 )) ;
    hexdump -n32 -s${show_me} -e '"%08_ax" 16/1 " %02x" "\n"' ${img_file};
    echo ""

    echo "   -> At end of file "
    show_me=$(( ${mLocal[img_size]} / 16 )) ;
    show_me=$(( ${show_me} * 16 - 16)) ;
    hexdump -s${show_me} -e '"%08_ax" 16/1 " %02x" "\n"' ${img_file};
    echo ""
}

# ---------------------------------------------------
function show_me_next
{
local img_address=_I${img_id}_org_;
cat << EOF
1. Mass Erase Flash 
2. Load "${mLocal[img_file]}" at address 0x${info[${img_address}]}
3. Load nvm_area_strip file
4. Load "${info[bootstrap_file]}" (or .bin file) at address 0x${info[_B_org_]}
EOF
}

#*******************************************************************************
if [[ $1 == "" ]]
then
    help_me;
    exit 0;
fi

# Get dir where bootstrap elf and app bin files are installed
install_dir=$1;
if [[ ! -d ${install_dir} ]]
then
    echo "Directory \"${install_dir}\" doesn't exist!!!";
    exit 0;
fi

# Get part id (if any, otherwise the default is part 0)
if [[ $2 != "" ]]
then
    if [[ $2 == 1 ]]
    then
        img_id=1;
    else
        img_id=0;
    fi
fi

# ---------------------------------------------------
info[bootstrap_file]="${install_dir}/bootstrap.elf";
mLocal[app_file]="${install_dir}/App_WizeUp.bin";

get_part_address;
get_app_info;
show_me_input;

img_bin_gen;
show_me_output;

show_me_next;

# ---------------------------------------------------
 

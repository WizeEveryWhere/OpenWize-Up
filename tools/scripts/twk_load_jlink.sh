#!/bin/bash 

# Default options
PROG_TOOL=JLinkExe

APP_DIR="../_install/bin";

BOOT_BIN=${APP_DIR}"/bootstrap.bin";
IMG_0_BIN=${APP_DIR}"/img_0.bin"
IMG_1_BIN=${APP_DIR}"/img_1.bin"

BOOT_ADDRESS=0x08000000
IMG_0_ADDRESS=0x0802c000
IMG_1_ADDRESS=0x08056000


if [[ -z "${DEVICE}" ]]
then
    DEVICE=STM32L451CE
else
    echo "Get environement DEVICE variable";
fi
ITF=SWD
SPEED=4000
echo "${DEVICE} device is selected";

TEMP_FILE=./temp_cmdFile.jlink

function build_ConnectFile()
{
cat <<- EOF > ${TEMP_FILE}
si ${ITF}
speed ${SPEED}
RSetType 0
EOF
}

function build_CmdFile_start()
{

# --- Load img_0
bin_file=${IMG_0_BIN}
bin_address=${IMG_0_ADDRESS}

cat <<- EOF >> ${TEMP_FILE}
loadbin ${bin_file}, ${bin_address}, noreset
verifybin ${bin_file}, ${bin_address}

EOF
# --- load img_1
bin_file=${IMG_1_BIN}
bin_address=${IMG_1_ADDRESS}

cat <<- EOF >> ${TEMP_FILE}
loadbin ${bin_file}, ${bin_address}, noreset
verifybin ${bin_file}, ${bin_address}

EOF
}

function build_CmdFile_end()
{

# --- Load bootstrap
bin_file=${BOOT_BIN}
bin_address=${BOOT_ADDRESS}

cat <<- EOF >> ${TEMP_FILE}
loadbin ${bin_file}, ${bin_address}, noreset
verifybin ${bin_file}, ${bin_address}

EOF
# --- Reboot
cat <<- EOF >> ${TEMP_FILE}
reset
go
EOF
}

function rm_CmdFile()
{
    rm -f ${TEMP_FILE};
    echo "";
}

function do_it()
{
    ${PROG_TOOL} -device ${DEVICE} -CommandFile ${TEMP_FILE};
}

function usage(){
    echo "Usage :"
    echo "$(basename $0) [option]"
    echo "  -p prog_dir : Program the board with app file in prog_dir";
    echo "";
    echo "Optionaly env. variable : "
    echo "DEVICE : select the MCU device";
    echo "    WizeUp board : STM32L451CE (default)";
    echo "";
}


if [[ "$#" -ge 1 ]];
then
    case $1 in
    "-p")
        APP_DIR=$2;
        if [[ "${APP_DIR}" == "" ]]
        then 
            echo "Error! Give an application file (binary) to download on the board.";
            usage;
            exit;
        fi
        
        BOOT_BIN=${APP_DIR}"/bootstrap.bin";
        IMG_0_BIN=${APP_DIR}"/img_0.bin";
        IMG_1_BIN=${APP_DIR}"/img_1.bin";
        
        bin_file=${BOOT_BIN}
        if [[ ! -f  ${bin_file} ]]
        then 
            echo "Error! \"${bin_file}\" file not found.";
            exit;
        fi
        
        bin_file=${IMG_0_BIN}
        if [[ ! -f  ${bin_file} ]]
        then 
            echo "Error! \"${bin_file}\" file not found.";
            exit;        
        fi
        
        bin_file=${IMG_1_BIN}
        if [[ ! -f  ${bin_file} ]]
        then 
            echo "Error! \"${bin_file}\" file not found.";
            exit;
        fi
        
        build_ConnectFile;
        echo "erase" >> ${TEMP_FILE};
        build_CmdFile_start;
        build_CmdFile_end;
        echo "q" >> ${TEMP_FILE};
        do_it;
        rm_CmdFile;
        ;;

    *)
        echo "Unknown command";
        usage;
        ;;
    esac
else
    echo "At least 1 parameters has to be given";
    usage;
fi

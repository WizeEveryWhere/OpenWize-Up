#!/bin/bash

mDev="";
mfield="";
afield="";
elogger=0;

def_mfield="0A0B";
def_afield="674523018900";
def_elogger=0;

wtime=0.01;

#*******************************************************************************
# env. variable : export IS_VERBOSE=1;
function SendAt()
{
    local cmd=$1;
    local info=$2;
    
    echo -e "${cmd}\r" > $mDev; sleep ${wtime};
    if [[ ${IS_VERBOSE} == 1 ]]
    then
        echo "echo -e '${cmd}\r' > $mDev";
    else
        if [[ "${info}" != "" ]]
        then
            printf "${info} : ";
        fi
        echo "${cmd}";
    fi
}

#*******************************************************************************
res=1;
function SendCommissioning()
{
    _get_commissioning_args_ "$@";
    if [[ $res == 1 ]]
    then
        return;
    fi
    
    local MField=${mfield};
    local AField=${afield};
    local elogger=${elogger};

    #echo "${mDev} ${MField} ${AField} ${elogger}";
    #return;
    
    local mKenc='0F0E0D0C0B0A09080706050403020100';
    local mKmac='596B25B5574F288CB0AB986407201770';
    local mKencId=1; # decimal
    local mNetwId=9; # decimal
    
    SendAt "ATIDENT=\$${MField},\$${AField}" "IDENT";
    SendAt "ATKENC=${mKencId},\$${mKenc}"    "KENC";
    SendAt "ATPARAM=\$28,${mKencId}"         "CIPH_CURRENT_KEY";
    SendAt "ATKMAC=\$${mKmac}"               "KMAC";
    SendAt "ATPARAM=\$2A,${mNetwId}"         "NetwId";
    SendAt 'ATPARAM=$30,1'                   "PING_RX_DELAY";
    SendAt 'ATPARAM=$31,3'                   "PING_RX_LENGTH";
    SendAt 'ATPARAM=$18,1'                   "EXCH_RX_DELAY";
    SendAt 'ATPARAM=$19,8'                   "EXCH_RX_LENGTH" ;
    SendAt 'ATPARAM=$1A,1'                   "EXCH_RESPONSE_DELAY";
    SendAt 'ATPARAM=$08,100'                 "RF_UPLINK_CHANNEL";
    SendAt 'ATPARAM=$09,120'                 "RF_DOWNLINK_CHANNEL";
    SendAt 'ATPARAM=$0A,0'                   "RF_UPLINK_MOD";
    SendAt 'ATPARAM=$0B,0'                   "RF_DOWNLINK_MOD";

    if [[ ${elogger} == 1 ]]
    then 
        # Enable the Logger
        SendAt 'ATPARAM=$FD,$FF' "LOGGER_TIME_OPT";
        SendAt 'ATPARAM=$FE,$FF' "LOGGER_LEVEL";
    else
        # Disable the Logger
        SendAt 'ATPARAM=$FD,$00' "LOGGER_TIME_OPT";
        SendAt 'ATPARAM=$FE,$00' "LOGGER_LEVEL";
    fi

    SendAt 'AT&W' "Store to NVM";
    sleep 0.1;
    SendAt 'ATZ' "Reboot";
}

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function usage()
{
    local prg=$1;
    
    if [[ ${elogger} == 0 ]]
    then
        logger_state="disable";
    else
        logger_state="enable";
    fi

    if [[ ${sourced} == 0 ]]
    then
cat << EOF
Usage : ${prg} -d dev [ -m MF ] [ -a AF ] [ -e ]

With : 
   -d dev : device (e.g. /dev/ttyUSB0)";
EOF
    else
cat << EOF
Usage : ${prg} [ -m MF ] [ -a AF ] [ -e ]

With : 
EOF
    fi
cat << EOF
   -m MF  : set the Manufacturer Field (default : ${mfield})\n";
   -a AF  : set the Address Field (default : ${afield})\n";
   -e     : Enable the Logger (default : ${logger_state})";
   
   -h     : this help
    
EOF
}

function _set_default_()
{
    afield=${def_afield};
    mfield=${def_mfield};
    elogger=${def_elogger};
}

function _get_commissioning_args_()
{
    local input="${@}";
    local option="hem:a:";
    local cnt_mandatory=1;
    res=0;
    
    if [[ -z ${KEEP_LAST} ]]
    then
        _set_default_;
    fi
    
    if [[ ${sourced} == 0 ]]
    then
        option+=":d:";
    else 
        ((cnt_mandatory-=1)); 
    fi
       
    OPTIND=1;
    while getopts ${option} arg ${input};
    do
        case $arg in
            e)
                elogger=1;
                ;;
            a)
                afield=${OPTARG};
                if [[ ${#afield} != 12 ]]
                then
                    echo "...bad size for AField : 6 bytes required.";
                    res=1;
                fi
                ;;
            m)
                mfield=${OPTARG};
                if [[ ${#mfield} != 4 ]]
                then
                    echo "...bad size for MField : 2 bytes required.";
                    res=1;
                fi
                ;;
            d)
                mDev=${OPTARG};
                ((cnt_mandatory-=1));
                ;;
            h|*)
                usage $prg;
                res=1;
                ;;
        esac
    done
    
    if [[ ${cnt_mandatory} != 0 ]]
    then
        echo "...missing argument...!!!";
        usage $prg;
        res=1;
    fi
}

#*******************************************************************************

(return 0 2>/dev/null) && sourced=1 || sourced=0

_set_default_;

if [[ ${sourced} == 1 ]]
then
    if [[ $# -ge 1 ]]
    then
        export mDev=$1;
    else 
        echo "Usage : source $(basename ${BASH_SOURCE[0]}) /dev/ttyDev";
        echo ""
        echo "Example : source $(basename ${BASH_SOURCE[0]}) /dev/ttyUSB0";
        echo ""
        return;
    fi
else 
    SendCommissioning "$@";
fi
#*******************************************************************************

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

declare -A paramsId=(
    [RF_UPLINK_CHANNEL]=08   
    [RF_DOWNLINK_CHANNEL]=09 
    [RF_UPLINK_MOD]=0A       
    [RF_DOWNLINK_MOD]=0B 
    [EXCH_RX_DELAY]=18       
    [EXCH_RX_LENGTH]=19      
    [EXCH_RESPONSE_DELAY]=1A 
    [CIPH_CURRENT_KEY]=28    
    [CLOCK_CURRENT_EPOC]=20 
    [L6NetwIdSelected]=2A    
    [PING_RX_DELAY]=30       
    [PING_RX_LENGTH]=31 
    [LOGGER_LEVEL]=FD        
    [LOGGER_TIME_OPT]=FE
);

declare -A atCmd=(
    # Read / Write
    [ATIDENT]='?|=$MField,$AField|Get/Set device identification'
    [ATPARAM]='=$id?|=$id,$value|Get/Set device parameter'
    [ATFC]='=$id?|=$id,$v1 [,$v2] [,$v3]|Get/Set Radio module configuration'
    # Write only
    [ATKMAC]='|=$kmac|Set the KMAC (as big endian, only the first 16 bytes)'
    [ATKENC]='|=$kid,$kenc|Set the KENC (as big endian, only the first 16 bytes) (kid in 1 to 14)'
    [ATSEND]='|=$l6app,$message|Send a DATA message'
    # Exec only
    [ATPING]='||Launch a PING/PONG session'
    [AT\&W]='||Write to flash'
    [AT\&F]='||Restore to Factory'
    [ATQ]='||Go to sleep'
    [ATZ]='||Reboot'
    [ATI]='||Get manufacturer info'
);

function _HelpAT_usage_ {
cat << EOF
Usage : 
   HelpAT          : Display all available AT command
   HelpAT -a       : Display all available AT command with details
   HelpAT ATcmd    : Display help on this particular AT command
   HelpAT -a ATcmd : Display help on this particular AT command with details
   
  SendAt 'ATcmd<formated string param>' (with '') : Send ATcmd
          - e.g.: SendAt 'ATPARAM=\$30,1'

EOF
}

function _HelpAT_display_()
{
    local details=$1;
    local req=$2;

    local mkeys='';

    local rHelp='';
    local wHelp='';
    local sHelp='';
       
    mkeys=(${req});
    if [[ ! -v atCmd[${req}] ]];
    then
        mkeys=(${!atCmd[@]});
        echo "*** Available AT command ***"
    fi
   
    #echo "${mkeys[@]}";

    IFS=$'|';
    for k in ${mkeys[@]}
    do
        read -r rHelp wHelp sHelp <<< "${atCmd[${k}]}"
        printf "%-7s : %s\n" ${k} ${sHelp};
        if [[ ${details} == 1 ]]
        then
            if [[ ${rHelp} != "" ]]
            then
                echo "   - R : ${k}${rHelp} ";
            fi
            
            if [[ ${wHelp} != "" ]]
            then
                echo "   - W : ${k}${wHelp} ";
            fi
        fi
    done
    unset IFS;
}

function HelpAT()
{  
    local input="${@}";
    local option="ha";
    
    local cmd="";
    local details=0;
    
    OPTIND=1;
    while getopts ${option} arg ${input};
    do
        case $arg in
            a)
                details=1;
                ;;
            ?|h|*)
                _HelpAT_usage_;
                return;
                ;;
        esac
    done
    shift $(expr $OPTIND - 1 );
    #echo "${details} : $1"
    if [[ "$1" == "" ]]
    then
        _HelpAT_usage_;
    fi
    _HelpAT_display_ ${details} $1;
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

    # echo "${mDev} ${MField} ${AField} ${elogger}";
    # return;

    local mKenc='0F0E0D0C0B0A09080706050403020100';
    local mKmac='596B25B5574F288CB0AB986407201770';
    local mKencId=1; # decimal
    local mNetwId=9; # decimal

    SendAt "ATI" "...for wake-up";
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
function GetCommissioning()
{
    SendAt 'ATIDENT?'     "IDENT";
    SendAt 'ATPARAM=$28?' "CIPH_CURRENT_KEY";
    SendAt 'ATPARAM=$2A?' "NetwId";
    SendAt 'ATPARAM=$30?' "PING_RX_DELAY";
    SendAt 'ATPARAM=$31?' "PING_RX_LENGTH";
    SendAt 'ATPARAM=$18?' "EXCH_RX_DELAY";
    SendAt 'ATPARAM=$19?' "EXCH_RX_LENGTH" ;
    SendAt 'ATPARAM=$1A?' "EXCH_RESPONSE_DELAY";
    SendAt 'ATPARAM=$08?' "RF_UPLINK_CHANNEL";
    SendAt 'ATPARAM=$09?' "RF_DOWNLINK_CHANNEL";
    SendAt 'ATPARAM=$0A?' "RF_UPLINK_MOD";
    SendAt 'ATPARAM=$0B?' "RF_DOWNLINK_MOD";
    SendAt 'ATPARAM=$FD?' "LOGGER_TIME_OPT";
    SendAt 'ATPARAM=$FE?' "LOGGER_LEVEL";
}

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function usage_main()
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
   -m MF  : set the Manufacturer Field (default : ${mfield})";
   -a AF  : set the Address Field (default : ${afield})";
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
                usage_main $prg;
                res=1;
                ;;
        esac
    done

    if [[ ${cnt_mandatory} != 0 ]]
    then
        echo "...missing argument...!!!";
        usage_main $prg;
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

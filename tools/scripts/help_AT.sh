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
    [ATSTAT]='?|=$v|Get/Clr network statistics'
    [ATIDENT]='?|=$MField,$AField|Get/Set device identification'
    [ATPARAM]='=$id?|=$id,$value|Get/Set device parameter'
    [ATFC]='=$id?|=$id,$v1 [,$v2] [,$v3]|Get/Set Radio module configuration'
    [ATTEST]='?|=$v|Get/Set the test mode'
    # Read only
    [ATUID]='?||Get CPU unique identifier'
    [ATCCLK]='?||Get the current EPOCH with milisecond extend'
    # Write only
    [ATKEY]='|=$kid,$key|Set the KEY (as big endian, 16 or 32 bytes) (kid in 1 to 19)'
    # Exec only
    [AT]='||Attention'
    [ATI]='||Get manufacturer info'
    [ATZ0]='||Reset all register, calibration, clock and Reboot'
    [ATZ1]='||Reboot'
    [ATQ]='||Go to sleep'
    [AT\&F]='||Restore to Factory'
    [AT\&W]='||Write to flash'
    # Session
    [ATSEND]='|=$l6app,$message|Send a DATA message'
    #[ATADMANN]='|=$Err|Response to an ADM ANN_DOWNLOAD'
    [ATPING]='||Launch a PING/PONG session'
    # Local update
    #[ATANN]='|=$SesId,$KeyId,$Announce|Request for local update'
    #[ATBLK]='|=$SesId,$BlkId,$Blk|Tranfert a FW block'
    #[ATUPD]='|=$SesId|Apply the update FW'
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
    
    sorted_keys=($(echo ${mkeys[@]} | tr ' ' '\n' | sort | tr '\n' ' '));
    
    #echo "${mkeys[@]}";
    #echo "${sorted_keys[@]}";
    
    IFS=$'|';
    #for k in ${mkeys[@]}
    for k in ${sorted_keys[@]}
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

    SendAt "AT"                              "...for wake-up";
    SendAt "ATI"                             "Show HW info";
    SendAt "ATIDENT=\$${MField},\$${AField}" "IDENT";
    
    SendAt 'ATPARAM=$30,1'                   "PING_RX_DELAY";
    SendAt 'ATPARAM=$31,3'                   "PING_RX_LENGTH";
    SendAt 'ATPARAM=$18,1'                   "EXCH_RX_DELAY";
    SendAt 'ATPARAM=$19,8'                   "EXCH_RX_LENGTH" ;
    SendAt 'ATPARAM=$1A,1'                   "EXCH_RESPONSE_DELAY";
    SendAt 'ATPARAM=$D0,$FF'                 "EXTEND_FLAGS";

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

    #SendAt 'AT&W' "Store to NVM";
    sleep 0.1;
    #SendAt 'ATZ' "Reboot";
}

#*******************************************************************************
function GetCommissioning()
{
    SendAt "AT"           "...for wake-up";
    SendAt 'ATIDENT?'     "IDENT";
    SendAt 'ATPARAM=$28?' "CIPH_CURRENT_KEY"; # Default setting in FW
    SendAt 'ATPARAM=$2A?' "NetwId"; # Default setting in FW
    SendAt 'ATPARAM=$30?' "PING_RX_DELAY";
    SendAt 'ATPARAM=$31?' "PING_RX_LENGTH";
    SendAt 'ATPARAM=$18?' "EXCH_RX_DELAY";
    SendAt 'ATPARAM=$19?' "EXCH_RX_LENGTH" ;
    SendAt 'ATPARAM=$1A?' "EXCH_RESPONSE_DELAY";
    SendAt 'ATPARAM=$08?' "RF_UPLINK_CHANNEL"; # Default setting in FW
    SendAt 'ATPARAM=$09?' "RF_DOWNLINK_CHANNEL"; # Default setting in FW
    SendAt 'ATPARAM=$0A?' "RF_UPLINK_MOD"; # Default setting in FW
    SendAt 'ATPARAM=$0B?' "RF_DOWNLINK_MOD"; # Default setting in FW
    SendAt 'ATPARAM=$D0?' "EXTEND_FLAGS";
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

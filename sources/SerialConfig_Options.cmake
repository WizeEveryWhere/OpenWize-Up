#-------------------------------------------------------------------------------
# Serial (UART) default settings

# -----------------------------------
# baud

# If not defined here, FW will we use the hard-coded SERIAL_BAUD=115200
# Possible values are standard baud rate values from 1200 to 4M.
# But it has never been tested for value greater than 115200.
add_option(NAME SERIAL_BAUD      C_DEFINE DISPLAY)

# -----------------------------------
# swap

# If not defined here, FW will we use the hard-coded SERIAL_SWAP=0
# The goal is to swap RX and TX pin. May be not really usable except for 
# UART4 which has a "wake-up" ability on its "standard" TX pin.
add_option(NAME SERIAL_SWAP      C_DEFINE DISPLAY)

# -----------------------------------
# timeout

# If not defined here, FW will we use the hard-coded SERIAL_TX_TIMEOUT=2000
# Timeout is given in ms.
add_option(NAME SERIAL_TX_TIMEOUT      C_DEFINE DISPLAY)

# If not defined here, FW will we use the hard-coded SERIAL_RX_TIMEOUT=0xFFFFFFFF
# Timeout is given in ms.
add_option(NAME SERIAL_RX_TIMEOUT      C_DEFINE DISPLAY)

#-------------------------------------------------------------------------------
# Logger serial default settings
# -----------------------------------
# baud

# If not defined here, FW will we use the hard-coded LOGGER_BAUD=115200
# Possible values are standard baud rate values from 1200 to 4M.
# But it has never been tested for value greater than 115200.  
add_option(NAME LOGGER_BAUD      C_DEFINE DISPLAY)

# -----------------------------------
# swap

# If not defined here, FW will we use the hard-coded LOGGER_SWAP=0
# The goal is to swap RX and TX pin. May be not really usable except for 
# UART4 which has a "wake-up" ability on its "standard" TX pin.
add_option(NAME LOGGER_SWAP      C_DEFINE DISPLAY)

# -----------------------------------
# timeout

# If not defined here, FW will we use the hard-coded LOGGER_TX_TIMEOUT=2000
# Timeout is given in ms.
add_option(NAME LOGGER_TX_TIMEOUT      C_DEFINE DISPLAY)

# If not defined here, FW will we use the hard-coded LOGGER_RX_TIMEOUT=0xFFFFFFFF
# Timeout is given in ms.
add_option(NAME LOGGER_RX_TIMEOUT      C_DEFINE DISPLAY)

#-------------------------------------------------------------------------------
# Console (or COM) serial default settings
# -----------------------------------
# baud

# If not defined here, FW will we use the hard-coded CONSOLE_BAUD=115200
# Possible values are standard baud rate values from 1200 to 4M.
# But it has never been tested for value greater than 115200.  
add_option(NAME CONSOLE_BAUD      C_DEFINE DISPLAY)

# -----------------------------------
# swap

# If not defined here, FW will we use the hard-coded CONSOLE_SWAP=0
# The goal is to swap RX and TX pin. May be not really usable except for 
# UART4 which has a "wake-up" ability on its "standard" TX pin.
add_option(NAME CONSOLE_SWAP      C_DEFINE DISPLAY)

# -----------------------------------
# timeout

# If not defined here, FW will we use the hard-coded CONSOLE_TX_TIMEOUT=2000
# Timeout is given in ms.
add_option(NAME CONSOLE_TX_TIMEOUT      C_DEFINE DISPLAY)

# If not defined here, FW will we use the hard-coded CONSOLE_RX_TIMEOUT=5000
# Timeout is given in ms.
add_option(NAME CONSOLE_RX_TIMEOUT      C_DEFINE DISPLAY)

#-------------------------------------------------------------------------------

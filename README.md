# Welcome to the rfidconv wiki!

Rfidconv is simple C utility, that converts between several RFID formats - final RFID representations, that are stored in DBs of some third party apps. We now support only few apps (RFID format is determined by the app), that are mostly origin from Czech production and which we are using in high school envirnoment. These apps including: user print management software (SafeQ by YSoft), dinning room evidence (ProVIS by VIS Plzeň) and student attendance software (Alpus by TOMST).

The program is simple as possible for end user who is mostly expected to be system administrator of environment where this utility can help to improve administration of theese systems. The utility can be invoked for example from a batch script in process of adding user to the environment etc.

The real format of RFID is described by concrete app and is most evidently seen in the DB of that system. There is also some detailed description of the formats, but is available only in Czech language [here](http://wiki.gymlit.cz/rfid).

`Usage: RfidConv.exe <src format> <dst format> <rfid> [default cust_id (byte in hex)]`

Supported formats are:

* safeq
* provis
* alpus
* alpusdec (dst only)
